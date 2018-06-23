#include "cntd.h"


void open_msr(char *dev)
{
	if(cntd->fd_msr < 0)
	{
	    if(access(dev, R_OK) != 0){
	        fprintf(stderr, "[COUNTDOWN ERROR] Access denied to %s! Did you install MSR/MSR_SAFE? Do you have access permissions?\n", dev);
	        exit(EXIT_FAILURE);
	    }

	    cntd->fd_msr = open(dev, O_RDWR);
	    if (cntd->fd_msr < 0)
	    {
	    	if(errno == ENXIO)
	    		fprintf(stderr, "[COUNTDOWN ERROR] open_msr %s: No CPU %d\n", dev, cntd->cpu_id);
	    	else if(errno == EIO)
	    		fprintf(stderr, "[COUNTDOWN ERROR] open_msr %s: CPU %d doesn't support MSRs\n", dev, cntd->cpu_id);
	  		else
	        	fprintf(stderr, "[COUNTDOWN ERROR] open_msr %s: Open fail!\n", dev);
	        
	        exit(EXIT_FAILURE);
	    }
	}
}

void close_msr()
{
	if(cntd->fd_msr > 0)
	{
    	close(cntd->fd_msr);
    	cntd->fd_msr = -1;
    }
}

uint64_t read_msr(int offset)
{
    uint64_t msr;

    if (pread(cntd->fd_msr, &msr, sizeof(msr), offset) != sizeof(msr))
    {
        fprintf(stderr, "[COUNTDOWN ERROR] rdmsr: CPU %d cannot read MSR 0x%x\n", cntd->cpu_id, offset);
        exit(EXIT_FAILURE);
    }

    return msr;
}

void write_msr(int offset, uint64_t value)
{
    if (pwrite(cntd->fd_msr, &value, sizeof(value), offset) != sizeof(value))
    {
        fprintf(stderr, "[COUNTDOWN ERROR] wrmsr: CPU %d cannot write MSR 0x%x\n", cntd->cpu_id, offset);
        exit(EXIT_FAILURE);
    }
}

uint32_t diff_32(uint32_t end, uint32_t start)
{
	if(end >= start)
		return end - start;
	else
    {
        uint32_t max = 0;
        max--;
		return (max - start) + end;
    }
}

uint64_t diff_48(uint64_t end, uint64_t start)
{
	if(end >= start)
		return end - start;
	else
		return (((uint64_t) MAX_48_BIT_UNSIGN) - start) + end;
}

uint64_t diff_64(uint64_t end, uint64_t start)
{
	if(end >= start)
		return end - start;
	else
    {
        uint64_t max = 0;
        max--;
		return (max - start) + end;
    }
}

int world_rank_2_local_rank(int rank, CNTD_Group_t* group)
{
	int i;
	for(i = 0; i < group->size; i++)
		if(rank == group->world_ranks[i])
			return i;
	return -1;
}

struct timespec time_sum(struct timespec par_1, struct timespec par_2)
{
	struct timespec temp;
	temp.tv_sec = par_1.tv_sec + par_2.tv_sec;
    temp.tv_nsec = par_1.tv_nsec + par_2.tv_nsec;
    if (temp.tv_nsec >= 1E9) {
        temp.tv_nsec -= 1E9;
        temp.tv_sec++;
    }
    return temp;
}

struct timespec time_diff(struct timespec par_1, struct timespec par_2)
{
	struct timespec temp;
	if ((par_1.tv_nsec - par_2.tv_nsec) < 0) {
		temp.tv_sec = par_1.tv_sec - par_2.tv_sec - 1;
		temp.tv_nsec = 1E9 + par_1.tv_nsec - par_2.tv_nsec;
	} else {
		temp.tv_sec = par_1.tv_sec - par_2.tv_sec;
		temp.tv_nsec = par_1.tv_nsec - par_2.tv_nsec;
	}
	return temp;
}

double timespec2double(struct timespec timing)
{
    return (double) timing.tv_sec + ((double) timing.tv_nsec / 1.0E9);
}

void remove_spaces(char* str)
{
  char* i = str;
  char* j = str;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

int mkpath(char *dir, mode_t mode)
{
    char tmp[STRING_SIZE];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    strncpy(tmp, dir, sizeof(tmp));
    len = strlen(tmp);
    if(len >= sizeof(tmp))
    {
        return -1;
    }

    /* if present, remove trailing slash */
    if(tmp[len - 1] == '/')
    {
        tmp[len - 1] = 0;
    }

    /* recursive mkdir */
    for(p = tmp + 1; *p; p++)
    {
        if(*p == '/')
        {
            *p = 0;
            /* test path */
            if(stat(tmp, &sb) != 0)
            {
                /* path does not exist - create directory */
                if(mkdir(tmp, mode) < 0)
                {
                    return -1;
                }
            }
            else if(!S_ISDIR(sb.st_mode))
            {
                /* not a directory */
                return -1;
            }
            *p = '/';
        }
    }
    /* test path */
    if(stat(tmp, &sb) != 0)
    {
        /* path does not exist - create directory */
        if(mkdir(tmp, mode) < 0)
        {
            return -1;
        }
    }
    else if(!S_ISDIR(sb.st_mode))
    {
        /* not a directory */
        return -1;
    }
    return 0;
}

int read_current_pstate()
{
	uint64_t curr_pstate = read_msr(IA32_PERF_STATUS);
	return (int) (curr_pstate >> 8) & 0xFFFF;
}

int read_target_pstate()
{
	uint64_t target_pstate = read_msr(IA32_PERF_CTL);
	return (int) (target_pstate >> 8) & 0xFFFF;
}

void write_pstate(int pstate)
{
	write_msr(IA32_PERF_CTL, pstate << 8);
}

void reset_pstate()
{
	write_pstate(arch->maximum_pstate);
}

int read_tstate()
{
	uint64_t tstate = read_msr(IA32_CLOCK_MODULATION);
	return (int) (tstate >> 1) & 0x7;
}

void write_tstate(int tstate)
{
	if(tstate == 0)
		write_msr(IA32_CLOCK_MODULATION, 0x0);
	else
		write_msr(IA32_CLOCK_MODULATION, (tstate << 1) | 0x10);
}

void reset_tstate()
{
	write_tstate(0);
}