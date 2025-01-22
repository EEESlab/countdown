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
#include <stdlib.h>

#ifdef INTEL
HIDDEN uint64_t read_msr(int offset)
{
	uint64_t msr;

	if (cntd->msr_fd == 0) {
		fprintf(stderr,
			"Error: <COUNTDOWN-node:%s-rank:%d> MSR-SAFE driver is not initialized!\n",
			cntd->node.hostname, cntd->rank->world_rank);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	if (pread(cntd->msr_fd, &msr, sizeof(msr), offset) != sizeof(msr)) {
		fprintf(stderr,
			"Error: <COUNTDOWN-node:%s-rank:%d> rdmsr: CPU %d cannot read MSR 0x%x\n",
			cntd->node.hostname, cntd->rank->world_rank,
			cntd->rank->cpu_id, offset);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	return msr;
}

HIDDEN void write_msr(int offset, uint64_t value)
{
	if (cntd->msr_fd == 0) {
		fprintf(stderr,
			"Error: <COUNTDOWN-node:%s-rank:%d> MSR-SAFE driver is not initialized!\n",
			cntd->node.hostname, cntd->rank->world_rank);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	if (pwrite(cntd->msr_fd, &value, sizeof(value), offset) !=
	    sizeof(value)) {
		fprintf(stderr,
			"Error: <COUNTDOWN-node:%s-rank:%d> wrmsr: CPU %d cannot write MSR 0x%x\n",
			cntd->node.hostname, cntd->rank->world_rank,
			cntd->rank->cpu_id, offset);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}
#endif

HIDDEN void set_freq(int freq_khz)
{
	if (!cntd->enable_eam_analysis) {
		if (cntd->use_cpufreq) {
			if (cntd->userspace_governor) {
				if (pwrite(cntd->scaling_setspeed_fd, &freq_khz,
					   sizeof(freq_khz),
					   0) != sizeof(freq_khz)) {
					fprintf(stderr,
						"Error: <COUNTDOWN-node:%s-rank:%d> Failed to write to file: %s\n",
						cntd->node.hostname,
						cntd->rank->world_rank,
						SCALING_SETSPEED);
					PMPI_Abort(MPI_COMM_WORLD,
						   EXIT_FAILURE);
				}
			} else {
				// write both max and min freq
				if (pwrite(cntd->scaling_max_freq_fd, &freq_khz,
					   sizeof(freq_khz),
					   0) != sizeof(freq_khz)) {
					fprintf(stderr,
						"Error: <COUNTDOWN-node:%s-rank:%d> Failed to write to file: %s\n",
						cntd->node.hostname,
						cntd->rank->world_rank,
						SCALING_MAX_FREQ);
					PMPI_Abort(MPI_COMM_WORLD,
						   EXIT_FAILURE);
				}
				if (pwrite(cntd->scaling_min_freq_fd, &freq_khz,
					   sizeof(freq_khz),
					   0) != sizeof(freq_khz)) {
					fprintf(stderr,
						"Error: <COUNTDOWN-node:%s-rank:%d> Failed to write to file: %s\n",
						cntd->node.hostname,
						cntd->rank->world_rank,
						SCALING_MIN_FREQ);
					PMPI_Abort(MPI_COMM_WORLD,
						   EXIT_FAILURE);
				}
			}
		} else {
#if (defined INTEL || defined AMD)
			// If CPUFREQ is not enabled, we need to write the P-state to the
			// MSR.
			int pstate;
			int written_pstate;
			int offset = IA32_PERF_CTL;
			pstate = freq_khz / 1.0E5;
			written_pstate = (pstate << 8) & 0xFF00;
			if (cntd->hwp_usage) {
				offset = IA32_HWP_REQUEST;
				/*
			  This is needed to write also \"Minimum_Performance\" field of this
			  HWP-state (bits 7-0), as well as \"Maximum_Performance\" one, which
			  is composed by bits 15-8. Both of them are written to the same value,
			  to disable, AT THE MOMENT, all hardware optimizations.
			*/
				written_pstate = (pstate & 0xFF) |
						 ((pstate << 8) & 0xFF00);
			}
			write_msr(offset, written_pstate);
#else
			fprintf(stderr,
				"Error: <COUNTDOWN-node:%s-rank:%d> CPUFREQ is not enabled, and no MSR is available!\n",
				cntd->node.hostname, cntd->rank->world_rank);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
#endif
		}
	}
}

HIDDEN void set_user_max_freq()
{
	if (cntd->user_freq_khz[MAX] == NO_CONF)
		set_sys_max_freq();
	else
		set_freq(cntd->user_freq_khz[MAX]);
}

HIDDEN void set_user_min_freq()
{
	if (cntd->user_freq_khz[MIN] == NO_CONF)
		set_sys_min_freq();
	else
		set_freq(cntd->user_freq_khz[MIN]);
}

HIDDEN void set_sys_max_freq()
{
	//#ifdef HWP_AVAIL
	//	if (cntd->hwp_usage) {
	//		set_min_epp();
	//		set_min_aw();
	//	}
	//#endif
	set_freq(cntd->sys_freq_khz[MAX]);
}

HIDDEN void set_sys_min_freq()
{
	//#ifdef HWP_AVAIL
	//	if (hwp_usage) {
	//		set_max_epp();
	//		set_min_aw();
	//	}
	//#endif
	set_freq(cntd->sys_freq_khz[MIN]);
}

HIDDEN int get_maximum_turbo_frequency()
{
	int world_rank, max_pstate;
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (!cntd->enable_eam_analysis) {
#ifdef INTEL
		if (!cntd->use_cpufreq) {
			int offset = MSR_TURBO_RATIO_LIMIT;
			if (cntd->hwp_usage)
				offset = IA32_HWP_CAPABILITIES;
			max_pstate = (int)(read_msr(offset) & 0xFF);

			return max_pstate * 1.0E5;
		}
#endif
		char max_freq_value[STRING_SIZE];

		char filename[STRING_SIZE];
		if (cntd->userspace_governor)
			snprintf(filename, STRING_SIZE, SCALING_SETSPEED,
				 cntd->rank->cpu_id);
		else
			snprintf(filename, STRING_SIZE, CPUINFO_MAX_FREQ);
		if (read_str_from_file(filename, max_freq_value) < 0) {
			fprintf(stderr,
				"Error: <COUNTDOWN-node:%s-rank:%d> Failed to read file: %s\n",
				hostname, world_rank, CPUINFO_MAX_FREQ);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		long int max_freq = strtol(max_freq_value, NULL, 10);
		return (int)max_freq;
	}
}

HIDDEN int get_minimum_frequency()
{
	if (cntd->hwp_usage) {
		int offset;
		int min_pstate;

		offset = IA32_HWP_CAPABILITIES;

		min_pstate = (int)((read_msr(offset) >> 24) & 0xFF);

		// Return the minimum p-state in MHz
		return min_pstate * 1.0E5;
	}
	int world_rank;
	char min_freq_value[STRING_SIZE];
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (read_str_from_file(CPUINFO_MIN_FREQ, min_freq_value) < 0) {
		fprintf(stderr,
			"Error: <COUNTDOWN-node:%s-rank:%d> Failed to read file: %s\n",
			hostname, world_rank, CPUINFO_MIN_FREQ);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	long int min_freq = strtol(min_freq_value, NULL, 10);
	return (int)min_freq;
}

HIDDEN void pm_init()
{
	if (!cntd->enable_eam_analysis && !cntd->use_cpufreq) {
#if (defined INTEL || defined AMD)
		int world_rank, errno, cpu_id;
		char msr_path[STRING_SIZE];
		char hostname[STRING_SIZE];

		gethostname(hostname, sizeof(hostname));
		PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		// If CPUFREQ is defined, I don't need to use msr
		cpu_id = sched_getcpu();
		if (cntd->force_msr)
			snprintf(msr_path, STRING_SIZE, MSR_FILE, cpu_id);
		else
			snprintf(msr_path, STRING_SIZE, MSRSAFE_FILE, cpu_id);

		// Try to open the MSR file
		cntd->msr_fd = open(msr_path, O_RDWR);
		if (cntd->msr_fd < 0) {
			if (errno == ENXIO)
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> No CPU %d\n",
					hostname, world_rank, cpu_id);
			else if (errno == EIO)
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> CPU %d doesn't support MSR-SAFE\n",
					hostname, world_rank, cpu_id);
			else
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> Failed to open %s\n",
					hostname, world_rank, msr_path);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

#ifdef HWP_AVAIL
		uint64_t pstate;

		pstate = read_msr(IA32_PM_ENABLE);

		if (pstate)
			cntd->hwp_usage = 1;
		else
			fprintf(stdout,
				"Warning: HWP-States available, but not usable.\n");
#endif
#endif
	}
}

HIDDEN void pm_finalize()
{
	if (cntd->enable_eam || cntd->enable_eam_slack) {
		if (!cntd->enable_eam_analysis) {
			set_sys_max_freq();
			if (!cntd->use_cpufreq)
				close(cntd->msr_fd);
		}
	}
}
