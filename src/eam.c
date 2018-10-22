/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *		* Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 * 
 *      * Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *      * Neither the name of the copyright holder nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Author: Daniele Cesarini, University of Bologna
 * Date: 24.08.2018
*/

#include "cntd.h"

CNTD_Call_t *callback_call;

void eamo_load_maps(char *output_dir)
{
	char eamo_file[STRING_SIZE];
	FILE *fd = NULL;
	struct stat sb;
	uint64_t file_size;

	if(stat(output_dir, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        sprintf(eamo_file, "%s/cntd_eamo_%d.bin", output_dir, cntd->rank->my_rank);
		if(access(eamo_file, F_OK) != -1)
		{
			fd = fopen(eamo_file, "rb");
			if(fd == NULL)
			{
				fprintf(stderr, "Error: <countdown> Failed to open energy-aware MPI oracle file!\n");
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
		else
		{
			fprintf(stderr, "Error: <countdown> Access denied to the energy-aware MPI oracle file!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
    }
    else
    {
        fprintf(stderr, "Error: <countdown> No energy-aware MPI oracle directory specified!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    fseek(fd, 0L, SEEK_END);
    file_size = ftell(fd);
    rewind(fd);

    cntd->eamo_map = (CNTD_EAMO_t *) malloc(file_size);
    fread(cntd->eamo_map, file_size, 1, fd);

    fclose(fd);

    cntd->eamo_count = 0;
}

void eamo_sched_next_conf(CNTD_Call_t *call, int when)
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

void eam_call_back(int signum)
{
	callback_call->flag_eam = TRUE;
	
	// Write lowest p-state
	write_pstate(cntd->rank->min_pstate);
}

void eam(CNTD_Call_t *call, int when)
{
	struct itimerval timer = {{0}};

	callback_call = call;

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
		if(call->flag_eam)
			write_pstate(cntd->rank->max_pstate);
	}
}
