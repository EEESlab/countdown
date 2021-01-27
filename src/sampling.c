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

static void read_energy(uint64_t *energy_pkg, uint64_t *energy_dram)
{
	int i;
	char energy_str[STRING_SIZE];
	
	for(i = 0; i < cntd->num_sockets; i++)
	{
		read_str_from_file(cntd->energy_pkg_name[i], energy_str);
		energy_pkg[i] = strtoul(energy_str, NULL, 10);

		read_str_from_file(cntd->energy_dram_name[i], energy_str);
		energy_dram[i] = strtoul(energy_str, NULL, 10);
	}
}

void make_sample(int sig, siginfo_t *siginfo, void *context)
{
	static int init = FALSE;
	static int flip = 0;
    static uint64_t energy_pkg[2][NUM_SOCKETS];
    static uint64_t energy_dram[2][NUM_SOCKETS];
    static double timing[2];

	if(init == FALSE)
	{
        timing[flip] = read_time();
		read_energy(energy_pkg[flip], energy_dram[flip]);
		init = TRUE;
	}
	else
	{
		int i;
		int prev = flip;
		flip = (flip == 0) ? 1 : 0;
		int curr = flip;

        // Sample energy
		read_energy(energy_pkg[curr], energy_dram[curr]);

        // Energy calculation
        uint64_t energy_diff;
        uint64_t energy_pkg_sum = 0;
        uint64_t energy_dram_sum = 0;

		for(i = 0; i < cntd->num_sockets; i++)
		{
			energy_diff = diff_overflow(
				energy_pkg[curr][i], 
				energy_pkg[prev][i], 
				cntd->energy_pkg_overflow[i]);
            energy_pkg_sum += energy_diff;
            cntd->energy_pkg[i] += energy_diff;

			energy_diff = diff_overflow(
				energy_dram[curr][i], 
				energy_dram[prev][i], 
				cntd->energy_pkg_overflow[i]);
            energy_dram_sum += energy_diff;
            cntd->energy_dram[i] += energy_diff;
		}
	}
}