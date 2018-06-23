#include "cntd.h"

void load_eamo_files(char *output_dir)
{
	char eamo_file[STRING_SIZE];
	FILE *fd = NULL;
	struct stat sb;
	uint64_t file_size;

	if(stat(output_dir, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        sprintf(eamo_file, "%s/cntd_eamo_%d.bin", output_dir, cntd->mpi_rank);
		if(access(eamo_file, F_OK) != -1)
		{
			fd = fopen(eamo_file, "rb");
			if(fd == NULL)
			{
				fprintf(stderr, "[COUNTDOWN ERROR] Failed to open energy-aware MPI oracle file!\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			fprintf(stderr, "[COUNTDOWN ERROR] Access denied to the energy-aware MPI oracle file!\n");
			exit(EXIT_FAILURE);
		}
    }
    else
    {
        fprintf(stderr, "[COUNTDOWN ERROR] No energy-aware MPI oracle directory specified!\n");
		exit(EXIT_FAILURE);
    }

    fseek(fd, 0L, SEEK_END);
    file_size = ftell(fd);
    rewind(fd);

    cntd->eamo_map = (CNTD_EAMO_t *) malloc(file_size);
    fread(cntd->eamo_map, file_size, 1, fd);

    fclose(fd);

    cntd->eamo_count = 0;
}

// Require root or msr_safe access permission!
//*************************
void call_back_eam(int signum)
{
	// Write lowest p-state
	cntd->eam_flag = TRUE;
	write_pstate(arch->minimum_pstate);

}

void eam(CNTD_Call_t *call, int when)
{
	struct itimerval timer = {{0}};

	if(when == START)
	{
		// Start timer
	    timer.it_value.tv_usec = cntd->eam_timeout;
	    setitimer(ITIMER_REAL, &timer, NULL);
	}
	else
	{
		// Reset timer
		setitimer(ITIMER_REAL, &timer, NULL);

		// Restore highest p-state (always force the restore of p-state! -> no atomic operation)
		if(cntd->eam_flag)
		{
			write_pstate(arch->maximum_pstate);
			cntd->eam_flag = FALSE;
		}
	}
}

void sched_next_eamo_conf(CNTD_Call_t *call, int when)
{
	if(call->idx == cntd->eamo_map[cntd->eamo_count].next_call && 
		when == cntd->eamo_map[cntd->eamo_count].when)
	{
		// Check and write p-state
		uint64_t next_pstate = cntd->eamo_map[cntd->eamo_count].next_pstate;
		if(next_pstate != cntd->eamo_curr_pstate)
		{
			write_pstate(next_pstate);
			cntd->eamo_curr_pstate = next_pstate;
		}
		
		// Check and write t-state
		uint64_t next_tstate = cntd->eamo_map[cntd->eamo_count].next_tstate;
		if(next_tstate != cntd->eamo_curr_tstate)
		{
			write_tstate(next_tstate);
			cntd->eamo_curr_tstate = next_tstate;
		}

		// Increment EAMO map counter
		cntd->eamo_count++;
	}
}
//*************************
