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

static void read_energy_pkg(uint64_t *energy_pkg)
{
	int i;
	char energy_str[STRING_SIZE];
	
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		read_str_from_file(cntd->energy_pkg_file[i], energy_str);
		energy_pkg[i] = strtoul(energy_str, NULL, 10);
	}
}

static void read_energy_dram(uint64_t *energy_dram)
{
	int i;
	char energy_str[STRING_SIZE];
	
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		read_str_from_file(cntd->energy_dram_file[i], energy_str);
		energy_dram[i] = strtoul(energy_str, NULL, 10);
	}
}

#ifdef CNTD_ENABLE_CUDA
static void read_energy_gpu(uint64_t *energy_gpu)
{
	int i;
	unsigned long long e;

	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		if(nvmlDeviceGetTotalEnergyConsumption(cntd->gpu[i], &e))
		{
			fprintf(stderr, "Error: <countdown> Failed to read energy consumption from GPU number %d'!\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		energy_gpu[i] = (uint64_t) e * 1E3;
	}
}
#endif

static double timing[2] = {0};
HIDDEN void event_sample(MPI_Type_t mpi_type, int phase)
{
	if(phase == START)
	{
		timing[START] = read_time();
		if(mpi_type == __MPI_INIT)
		{
			cntd->cpu.exe_time[START] = timing[START];
			cntd->node.exe_time[START] = timing[START];
		}
		else
			cntd->cpu.app_time += timing[START] - timing[END];
	}
	else if(phase == END)
	{
		timing[END] = read_time();

		double mpi_time = timing[END] - timing[START];
		cntd->cpu.mpi_time += mpi_time;
		cntd->cpu.mpi_type_time[mpi_type] += mpi_time;
		cntd->cpu.mpi_type_cnt[mpi_type]++;

		if(mpi_type == __MPI_FINALIZE)
		{
			cntd->cpu.exe_time[END] = timing[END];
			cntd->node.exe_time[END] = timing[END];
		}
	}
}

HIDDEN void time_sample(int sig, siginfo_t *siginfo, void *context)
{
	static int init = FALSE;
	static int flip = 0;
	static double timing[2];
    static uint64_t energy_pkg[2][MAX_NUM_SOCKETS] = {0};
    static uint64_t energy_dram[2][MAX_NUM_SOCKETS] = {0};
#ifdef CNTD_ENABLE_CUDA
	static uint64_t energy_gpu[2][MAX_NUM_GPUS] = {0};
#endif

	if(init == FALSE)
	{
        timing[flip] = read_time();
		read_energy_pkg(energy_pkg[flip]);
		read_energy_dram(energy_dram[flip]);
#ifdef CNTD_ENABLE_CUDA
		read_energy_gpu(energy_gpu[flip]);
#endif
		init_timeseries_report();
        cntd->num_sampling++;
		init = TRUE;
	}
	else
	{
		int i;
		int prev = flip;
		flip = (flip == 0) ? 1 : 0;
		int curr = flip;

        timing[curr] = read_time();

        // Sample energy
		read_energy_pkg(energy_pkg[curr]);
		read_energy_dram(energy_dram[curr]);
#ifdef CNTD_ENABLE_CUDA
		read_energy_gpu(energy_gpu[curr]);
#endif

        // Energy calculation
        uint64_t energy_pkg_diff[MAX_NUM_SOCKETS], energy_dram_diff[MAX_NUM_SOCKETS], energy_gpu_diff[MAX_NUM_GPUS];

		for(i = 0; i < cntd->node.num_sockets; i++)
		{
			energy_pkg_diff[i] = diff_overflow(
				energy_pkg[curr][i], 
				energy_pkg[prev][i], 
				cntd->energy_pkg_overflow[i]);
            cntd->node.energy_pkg[i] += energy_pkg_diff[i];

			energy_dram_diff[i] = diff_overflow(
				energy_dram[curr][i], 
				energy_dram[prev][i], 
				cntd->energy_dram_overflow[i]);
            cntd->node.energy_dram[i] += energy_dram_diff[i];
		}

#ifdef CNTD_ENABLE_CUDA
		for(i = 0; i < cntd->node.num_gpus; i++)
		{
			energy_gpu_diff[i] = diff_overflow(
				energy_gpu[curr][i], 
				energy_gpu[prev][i], 
				UINT64_MAX);
            cntd->node.energy_gpu[i] += energy_gpu_diff[i];
		}
#endif

		print_timeseries_report(timing[curr], timing[prev], energy_pkg_diff, energy_dram_diff, energy_gpu_diff);
        cntd->num_sampling++;
	}
}
