/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *			* Redistributions of source code must retain the above copyright notice, this
 *				list of conditions and the following disclaimer.
 *
 *			* Redistributions in binary form must reproduce the above copyright notice,
 *				this list of conditions and the following disclaimer in the documentation
 *				and/or other materials provided with the distribution.
 *
 *			* Neither the name of the copyright holder nor the names of its
 *				contributors may be used to endorse or promote products derived from
 *				this software without specific prior written permission.
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
*/

#include "cntd.h"

#ifdef X86_64
static int cpu_id = 0;
static int fd_msr = 0;

/*
static uint64_t read_msr(int offset)
{
    uint64_t msr;

	if(fd_msr == 0)
	{
		fprintf(stderr, "Error: <countdown> MSR-SAFE driver is not initialized!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pread(fd_msr, &msr, sizeof(msr), offset) != sizeof(msr))
    {
        fprintf(stderr, "Error: <countdown> rdmsr: CPU %d cannot read MSR 0x%x\n", cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return msr;
}
*/

static void write_msr(int offset, uint64_t value)
{
	if(fd_msr == 0)
	{
		fprintf(stderr, "Error: <countdown> MSR-SAFE driver is not initialized!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pwrite(fd_msr, &value, sizeof(value), offset) != sizeof(value))
    {
        fprintf(stderr, "Error: <countdown> wrmsr: CPU %d cannot write MSR 0x%x\n", cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
}
#endif

HIDDEN void set_pstate(int pstate)
{
#ifdef X86_64
	write_msr(IA32_PERF_CTL, pstate << 8);
#endif
}

HIDDEN void set_max_pstate()
{
	if(cntd->user_pstate[MAX] != NO_CONF)
		set_pstate(cntd->user_pstate[MAX]);
	else
		set_pstate(cntd->sys_pstate[MAX]);
}

HIDDEN void set_min_pstate()
{
	if(cntd->user_pstate[MIN] != NO_CONF)
		set_pstate(cntd->user_pstate[MIN]);
	else
		set_pstate(cntd->sys_pstate[MIN]);
}

HIDDEN void pm_init()
{
#ifdef X86_64
	int errno;
	char msr_path[STRING_SIZE];
	cpu_id = sched_getcpu();

	if(cntd->force_msr)
		snprintf(msr_path, STRING_SIZE, MSR_FILE, cpu_id);
	else
		snprintf(msr_path, STRING_SIZE, MSRSAFE_FILE, cpu_id);

    fd_msr = open(msr_path, O_RDWR);
    if (fd_msr < 0)
    {
    	if(errno == ENXIO)
    		fprintf(stderr, "Error: <countdown> No CPU %d\n", cpu_id);
    	else if(errno == EIO)
    		fprintf(stderr, "Error: <countdown> CPU %d doesn't support MSR-SAFE\n", cpu_id);
  		else
        	fprintf(stderr, "Error: <countdown> Failed to open %s\n", msr_path);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
#endif
}

HIDDEN void pm_finalize()
{
#ifdef X86_64
	close(fd_msr);
	fd_msr = 0;
#endif
}
