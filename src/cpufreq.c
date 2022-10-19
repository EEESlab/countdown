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

HIDDEN void init_cpufreq() {
	char filename[STRING_SIZE];
	read_str_from_file(SCALING_GOVERNOR, cntd->scaling_governor);
	if (!(strcmp(cntd->scaling_governor, "userspace")))
		cntd->userspace_governor = 1;
	cntd->policy_limits_freq_fd[0] = open_file(CPUINFO_MAX_FREQ, O_RDONLY);
	cntd->policy_limits_freq_fd[1] = open_file(CPUINFO_MIN_FREQ, O_RDONLY);
	snprintf(filename        ,
             STRING_SIZE     ,
             SCALING_MAX_FREQ,
             cntd->rank->cpu_id);
	cntd->policy_limits_freq_fd[2] = open_file(filename, O_RDWR);
	snprintf(filename        ,
             STRING_SIZE     ,
             SCALING_MIN_FREQ,
             cntd->rank->cpu_id);
	cntd->policy_limits_freq_fd[3] = open_file(filename, O_RDWR);

	int scaling_setspeed_flags = O_RDONLY;
	if (cntd->userspace_governor)
		scaling_setspeed_flags = O_RDWR;
	snprintf(filename        ,
             STRING_SIZE     ,
             SCALING_SETSPEED,
             cntd->rank->cpu_id);
	cntd->policy_limits_freq_fd[4] = open_file(filename,
											   scaling_setspeed_flags);
}

HIDDEN void finalize_cpufreq() {
	int i;

	for (i = 0; i < 5; i++)
		close(cntd->policy_limits_freq_fd[i]);
}

