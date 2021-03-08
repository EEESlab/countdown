/*
 * Copyright (c), CINECA, UNIBO, and ETH Zurich
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
*/

#include "cntd.h"

#ifdef INTEL
/*
static uint64_t read_msr(int offset)
{
    uint64_t msr;

	if(cntd->msr_fd == 0)
	{
		fprintf(stderr, "Error: <countdown> MSR-SAFE driver is not initialized!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pread(cntd->msr_fd, &msr, sizeof(msr), offset) != sizeof(msr))
    {
        fprintf(stderr, "Error: <countdown> rdmsr: CPU %d cannot read MSR 0x%x\n", cntd->cpu.cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return msr;
}
*/

static void write_msr(int offset, uint64_t value)
{
	if(cntd->msr_fd == 0)
	{
		fprintf(stderr, "Error: <countdown> MSR-SAFE driver is not initialized!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pwrite(cntd->msr_fd, &value, sizeof(value), offset) != sizeof(value))
    {
        fprintf(stderr, "Error: <countdown> wrmsr: CPU %d cannot write MSR 0x%x\n", cntd->cpu.cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
}
#endif

HIDDEN void set_pstate(int pstate)
{
	if(cntd->enable_eam_freq == ENABLE_FREQ)
	{
#ifdef INTEL
		write_msr(IA32_PERF_CTL, pstate << 8);
#endif
	}
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
	if(cntd->enable_eam_freq == ENABLE_FREQ)
	{
#ifdef INTEL
		int errno;
		char msr_path[STRING_SIZE];

		if(cntd->force_msr)
			snprintf(msr_path, STRING_SIZE, MSR_FILE, cntd->cpu.cpu_id);
		else
			snprintf(msr_path, STRING_SIZE, MSRSAFE_FILE, cntd->cpu.cpu_id);

		cntd->msr_fd = open(msr_path, O_RDWR);
		if (cntd->msr_fd < 0)
		{
			if(errno == ENXIO)
				fprintf(stderr, "Error: <countdown> No CPU %d\n", cntd->cpu.cpu_id);
			else if(errno == EIO)
				fprintf(stderr, "Error: <countdown> CPU %d doesn't support MSR-SAFE\n", cntd->cpu.cpu_id);
			else
				fprintf(stderr, "Error: <countdown> Failed to open %s\n", msr_path);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
#endif
		set_max_pstate();
	}
}

HIDDEN void pm_finalize()
{
	if(cntd->enable_eam_freq == ENABLE_FREQ)
	{
		set_max_pstate();
#ifdef INTEL
		close(cntd->msr_fd);
#endif
	}
}
