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
static uint64_t read_msr(int offset)
{
    uint64_t msr;

	if(cntd->msr_fd == 0)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> MSR-SAFE driver is not initialized!\n",
			cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pread(cntd->msr_fd, &msr, sizeof(msr), offset) != sizeof(msr))
    {
        fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> rdmsr: CPU %d cannot read MSR 0x%x\n", 
			cntd->node.hostname, cntd->rank->world_rank, cntd->rank->cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return msr;
}

HIDDEN void write_msr(int offset, uint64_t value)
{
	if(cntd->msr_fd == 0)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> MSR-SAFE driver is not initialized!\n",
			cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

    if(pwrite(cntd->msr_fd, &value, sizeof(value), offset) != sizeof(value))
    {
        fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> wrmsr: CPU %d cannot write MSR 0x%x\n", 
			cntd->node.hostname, cntd->rank->world_rank, cntd->rank->cpu_id, offset);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
}
#endif

HIDDEN void set_pstate(int pstate)
{
	if(cntd->enable_eam_freq)
	{
#ifdef INTEL
		int offset;
#ifdef CPU_HWP
		offset = IA32_HWP_REQUEST;
		/*
		  This is needed to write also \"Minimum_Performance\" field of this
		  HWP-state (bits 7-0), as well as \"Maximum_Performance\" one, which
		  is composed by bits 15-8. Both of them are written to the same value,
		  to disable, AT THE MOMENT, all hardware optimizations.
		*/
		write_msr(offset, pstate);
		/*
		  This is needed to write also \"Desired_Performance\" field of this
		  HWP-state (bits 23-16), as well as \"Maximum_Performance\" and
		  \"Minimum_Performance\" ones, because we are continuing getting very
		  high fluctuations in frequency values. So, this should convey a hint
		  more aggressive, to the hardware.
		*/
		write_msr(offset, pstate << 16);

#else
		offset = IA32_PERF_CTL;
#endif
		write_msr(offset, pstate << 8);
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

HIDDEN int get_maximum_turbo_frequency()
{
	int world_rank, max_pstate;
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

#ifdef INTEL
	if(cntd->enable_eam_freq) {
		int offset;
#ifdef CPU_HWP
		offset = IA32_HWP_CAPABILITIES;
#else
		offset = MSR_TURBO_RATIO_LIMIT;
#endif
		max_pstate = (int) (read_msr(offset) & 0xFF);
	}
	else
	{
#endif
		char max_pstate_value[STRING_SIZE];
		if(read_str_from_file(CPUINFO_MAX_FREQ, max_pstate_value) < 0)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read file: %s\n", 
				hostname, world_rank, CPUINFO_MAX_FREQ);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		double pstate_double = strtod(max_pstate_value, NULL) / 1.0E5;
		int pstate_int = (int) pstate_double;
		if(pstate_double == (double) pstate_int)
			max_pstate = pstate_int;
		else
			max_pstate = pstate_int + 1;
#ifdef INTEL
	}
#endif

	return max_pstate;
}

HIDDEN int get_minimum_frequency()
{
#ifdef CPU_HWP
	int offset;
	int min_pstate;

	offset = IA32_HWP_CAPABILITIES;

	min_pstate = (int)((read_msr(offset) >> 24)  & 0xFF);

	return min_pstate;
#else
	int world_rank;
	char min_pstate_value[STRING_SIZE];
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(read_str_from_file(CPUINFO_MIN_FREQ, min_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read file: %s\n", 
			hostname, world_rank, CPUINFO_MIN_FREQ);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	return (int) (strtof(min_pstate_value, NULL) / 1.0E5);
#endif
}

HIDDEN void pm_init()
{
	if(cntd->enable_eam_freq)
	{
#ifdef INTEL
		int world_rank, errno;
		char msr_path[STRING_SIZE];
		char hostname[STRING_SIZE];

		gethostname(hostname, sizeof(hostname));
		PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		int cpu_id = cpu_id = sched_getcpu();

		if(cntd->force_msr)
			snprintf(msr_path, STRING_SIZE, MSR_FILE, cpu_id);
		else
			snprintf(msr_path, STRING_SIZE, MSRSAFE_FILE, cpu_id);

		cntd->msr_fd = open(msr_path, O_RDWR);
		if (cntd->msr_fd < 0)
		{
			if(errno == ENXIO)
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> No CPU %d\n", 
					hostname, world_rank, cpu_id);
			else if(errno == EIO)
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> CPU %d doesn't support MSR-SAFE\n", 
					hostname, world_rank, cpu_id);
			else
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to open %s\n", 
					hostname, world_rank, msr_path);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
#endif
	}
}

HIDDEN void pm_finalize()
{
	if(cntd->enable_eam_freq)
	{
		set_max_pstate();
#ifdef INTEL
		close(cntd->msr_fd);
#endif
	}
}
