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
#include <unistd.h>

HIDDEN void init_cpufreq()
{
	char filename[STRING_SIZE];
	char hostname[STRING_SIZE];
	int world_rank;

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	read_str_from_file(SCALING_GOVERNOR, cntd->scaling_governor);
	if (!(strcmp(cntd->scaling_governor, "userspace")))
		cntd->userspace_governor = 1;
	cntd->cpuinfo_max_freq = read_int_from_file(CPUINFO_MAX_FREQ);
	cntd->cpuinfo_min_freq = read_int_from_file(CPUINFO_MIN_FREQ);
	// I need these files to set the frequency only if I'm not in analysis mode
	if (!cntd->enable_eam_analysis) {
		snprintf(filename, STRING_SIZE, SCALING_MAX_FREQ,
			 cntd->rank->cpu_id);
		cntd->scaling_max_freq_fd = open_file(filename, O_RDWR);
		snprintf(filename, STRING_SIZE, SCALING_MIN_FREQ,
			 cntd->rank->cpu_id);
		cntd->scaling_min_freq_fd = open_file(filename, O_RDWR);

		if (cntd->userspace_governor) {
			snprintf(filename, STRING_SIZE, SCALING_SETSPEED,
				 cntd->rank->cpu_id);
			cntd->scaling_setspeed_fd = open_file(filename, O_RDWR);
		} else {
			cntd->scaling_setspeed_fd = -1;
		}
	}
}

HIDDEN void finalize_cpufreq()
{
	close(cntd->scaling_max_freq_fd);
	close(cntd->scaling_min_freq_fd);
	if (cntd->userspace_governor)
		close(cntd->scaling_setspeed_fd);
}
