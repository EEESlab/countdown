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

HIDDEN void print_final_report()
{
	int i, j;
	int world_rank, world_size, local_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_size(cntd->comm_local_masters, &local_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Datatype cpu_type = get_mpi_datatype_cpu();
	MPI_Datatype node_type = get_mpi_datatype_node();
	
	CNTD_CPUInfo_t cpuinfo[world_size];
	CNTD_NodeInfo_t nodeinfo[local_size];

	PMPI_Gather(&cntd->cpu, 1, cpu_type, 
		cpuinfo, 1, cpu_type, 
		0, MPI_COMM_WORLD);
	if(cntd->iam_local_master)
	{
		PMPI_Gather(&cntd->node, 1, node_type, 
			nodeinfo, 1, node_type, 
			0, cntd->comm_local_masters);
	}

	if(world_rank == 0)
	{
		float tot_energy_sys = 0;
		float tot_energy_pkg = 0;
		float tot_energy_dram = 0;
		float tot_energy_gpu = 0;

		float exe_time = nodeinfo[0].exe_time[END] - nodeinfo[0].exe_time[START];
		for(i = 0; i < local_size; i++)
		{
#ifdef POWER9
			tot_energy_sys += nodeinfo[i].energy_sys;
#endif
            for(j = 0; j < nodeinfo[i].num_sockets; j++)
			{
				tot_energy_pkg += nodeinfo[i].energy_pkg[j];
#if defined(INTEL) || defined(POWER9)
				tot_energy_dram += nodeinfo[i].energy_dram[j];
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
				tot_energy_gpu += nodeinfo[i].energy_gpu[j];
#endif
			}
#ifdef NVIDIA_GPU
			for(j = 0; j < nodeinfo[i].num_gpus; j++)
				tot_energy_gpu += nodeinfo[i].energy_gpu[j];
#endif
		}

		double app_time = 0;
		double mpi_time = 0;
		uint64_t mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double mpi_type_time[NUM_MPI_TYPE] = {0};
		for(i = 0; i < world_size; i++)
		{
			app_time += cpuinfo[i].app_time;
			mpi_time += cpuinfo[i].mpi_time;
			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				if(cpuinfo[i].mpi_type_cnt[j] > 0)
				{
					mpi_type_cnt[j]++;
					mpi_type_time[j] += cpuinfo[i].mpi_type_time[j];
				}
			}
		}

		printf("######################################################\n");
		printf("##################### COUNTDOWN ######################\n");
		printf("######################################################\n");
		printf("EXE time: %.3f sec\n", exe_time);
		printf("##################### ENERGY #########################\n");
		printf("PKG : %10.0f J\n", tot_energy_pkg);
#if defined(INTEL) || defined(POWER9)
		printf("DRAM: %10.0f J\n", tot_energy_dram);
#endif
#if defined(POWER9) || defined(NVIDIA_GPU)
		printf("GPU : %10.0f J\n", tot_energy_gpu);
#endif
#ifdef POWER9
		printf("SYS : %10.0f J\n", tot_energy_sys);
#endif
		printf("##################### AVG POWER ######################\n");
		printf("PKG : %10.2f W\n", tot_energy_pkg / exe_time);
#if defined(INTEL) || defined(POWER9)
		printf("DRAM: %10.2f W\n", tot_energy_dram / exe_time);
#endif
#if defined(POWER9) || defined(NVIDIA_GPU)
		printf("GPU : %10.2f W\n", tot_energy_gpu / exe_time);
#endif
#ifdef POWER9
		printf("SYS : %10.2f W\n", tot_energy_sys / exe_time);
#endif
		printf("##################### MPI TIMING #####################\n");
		printf("APP time: %10.3f sec - %6.2f%%\n", app_time, (app_time/(app_time+mpi_time))*100.0);
		printf("MPI time: %10.3f sec - %6.2f%%\n", mpi_time, (mpi_time/(app_time+mpi_time))*100.0);
		printf("##################### MPI REPORTING ##################\n");
		for(j = 0; j < NUM_MPI_TYPE; j++)
		{
			if(mpi_type_cnt[j] > 0)
			{
				printf("%s: %d - %.3f Sec - %.2f%%\n", 
					mpi_type_str[j]+2, 
					mpi_type_cnt[j], 
					mpi_type_time[j], 
					(mpi_type_time[j]/mpi_time)*100.0);
			}
		}
		printf("######################################################\n");
	}
}

static FILE *timeseries_fd;
HIDDEN void init_timeseries_report()
{
	int i;
	char filename[STRING_SIZE];

	snprintf(filename, STRING_SIZE, "%s/%s.csv", cntd->log_dir, cntd->node.hostname);
	timeseries_fd = fopen(filename, "w");
	if(timeseries_fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed create time-series file '%s'!\n", filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Time sample
	fprintf(timeseries_fd, "time-sample");

	// Energy
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";energy-pkg-%d", i);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";energy-dram-%d", i);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";energy-gpus-socket-%d", i);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
		fprintf(timeseries_fd, ";energy-gpu-%d", i);
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";energy-sys");
#endif

	// Power
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";power-pkg-%d", i);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";power-dram-%d", i);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";power-gpus-socket-%d", i);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
		fprintf(timeseries_fd, ";power-gpu-%d", i);
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";power-sys\n");
#endif
}

HIDDEN void finalize_timeseries_report()
{
	fclose(timeseries_fd);
}

HIDDEN void print_timeseries_report(double time_curr, double time_prev, float energy_sys, float *energy_pkg, float *energy_dram, float *energy_gpu)
{
	int i;
	double sample_duration = time_curr - time_prev;

	// Time sample
	fprintf(timeseries_fd, "%.3f", time_curr - cntd->node.exe_time[START]);

	// Energy
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";%.2f", energy_pkg[i]);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";%.2f", energy_dram[i]);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";%.2f", energy_gpu[i]);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
		fprintf(timeseries_fd, ";%.2f", energy_gpu[i]);
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";%.2f", energy_sys);
#endif

	// Power
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";%.2f", energy_pkg[i] / sample_duration);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";%.2f", energy_dram[i] / sample_duration);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";%.2f", 
			energy_gpu[i] / sample_duration);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";%.2f", 
			energy_gpu[i] / sample_duration);
	}
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";%.2f\n", energy_sys / sample_duration);
#endif
}
