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

int read_current_pstate()
{
	uint64_t curr_pstate;

	read_msr_by_idx(cntd->rank->cpu_id, IA32_PERF_STATUS, &curr_pstate);
	return (int) (curr_pstate >> 8) & 0xFFFF;
}

int read_target_pstate()
{
	uint64_t target_pstate;

	read_msr_by_idx(cntd->rank->cpu_id, IA32_PERF_CTL, &target_pstate);
	return (int) (target_pstate >> 8) & 0xFFFF;
}

void write_pstate(int pstate)
{
	write_msr_by_idx(cntd->rank->cpu_id, IA32_PERF_CTL, pstate << 8);
}

void reset_pstate()
{
	write_msr_by_idx(cntd->rank->cpu_id, IA32_PERF_CTL, cntd->rank->max_pstate);
}

int read_tstate()
{
	uint64_t tstate;

	read_msr_by_idx(cntd->rank->cpu_id, IA32_CLOCK_MODULATION, &tstate);
	return (int) (tstate >> 1) & 0x7;
}

void write_tstate(int tstate)
{
	if(tstate == 0)
		write_msr_by_idx(cntd->rank->cpu_id, IA32_CLOCK_MODULATION, 0x0);
	else
		write_msr_by_idx(cntd->rank->cpu_id, IA32_CLOCK_MODULATION, (tstate << 1) | 0x10);
}

void reset_tstate()
{
	write_tstate(0x0);
}
