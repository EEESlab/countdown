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

static FILE *timeseries_fd;

HIDDEN void print_final_report()
{
	int i, j;
	int world_rank, world_size, local_master_size;

    PMPI_Comm_size(MPI_COMM_WORLD, &world_size);
	PMPI_Comm_size(cntd->comm_local_masters, &local_master_size);
    PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Datatype node_type = get_mpi_datatype_node();
	MPI_Datatype cpu_type = get_mpi_datatype_cpu();
	MPI_Datatype gpu_type = get_mpi_datatype_gpu();
	
	CNTD_NodeInfo_t nodeinfo[local_master_size];
	CNTD_CPUInfo_t cpuinfo[world_size];
	CNTD_GPUInfo_t gpuinfo[local_master_size];

	PMPI_Gather(&cntd->cpu, 1, cpu_type, cpuinfo, 1, cpu_type, 0, MPI_COMM_WORLD);
	if(cntd->iam_local_master)
	{
		PMPI_Gather(&cntd->node, 1, node_type, nodeinfo, 1, node_type, 0, cntd->comm_local_masters);
#ifdef NVIDIA_GPU
		PMPI_Gather(&cntd->gpu, 1, gpu_type, gpuinfo, 1, gpu_type, 0, cntd->comm_local_masters);
#endif	
	}

	if(world_rank == 0)
	{
#ifdef POWER9
		double global_energy_sys = 0;
#endif
		double global_energy_pkg = 0;
#if defined(INTEL) || defined(POWER9)
		double global_energy_dram = 0;
#endif
#ifdef POWER9
		double global_energy_gpu_sys = 0;
#endif
#ifdef NVIDIA_GPU
		double global_energy_gpu = 0;
#endif

		double exe_time = nodeinfo[0].exe_time[END] - nodeinfo[0].exe_time[START];

		if(cntd->enable_hw_monitor)
		{
			// Energy reduction
			for(i = 0; i < local_master_size; i++)
			{
#ifdef POWER9
				global_energy_sys += nodeinfo[i].energy_sys;
#endif
            	for(j = 0; j < nodeinfo[i].num_sockets; j++)
				{
					global_energy_pkg += nodeinfo[i].energy_pkg[j];
#if defined(INTEL) || defined(POWER9)
					global_energy_dram += nodeinfo[i].energy_dram[j];
#endif
#ifdef POWER9
					global_energy_gpu_sys += nodeinfo[i].energy_gpu[j];
#endif
				}
#ifdef NVIDIA_GPU
				for(j = 0; j < nodeinfo[i].num_gpus; j++)
					global_energy_gpu += gpuinfo[i].energy[j];
#endif
			}
		}

		double app_time = 0;
		double mpi_time = 0;
		double cntd_mpi_time = 0;
		uint64_t cntd_mpi_cnt = 0;
		uint64_t mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double mpi_type_time[NUM_MPI_TYPE] = {0};
		uint64_t cntd_mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double cntd_mpi_type_time[NUM_MPI_TYPE] = {0};

		for(i = 0; i < world_size; i++)
		{
			app_time += cpuinfo[i].app_time;
			mpi_time += cpuinfo[i].mpi_time;

			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				mpi_type_cnt[j] += cpuinfo[i].mpi_type_cnt[j];
				mpi_type_time[j] += cpuinfo[i].mpi_type_time[j];

				cntd_mpi_type_cnt[j] += cpuinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_type_time[j] += cpuinfo[i].cntd_mpi_type_time[j];

				cntd_mpi_cnt += cpuinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_time += cpuinfo[i].cntd_mpi_type_time[j];
			}
		}

		printf("######################################################\n");
		printf("##################### COUNTDOWN ######################\n");
		printf("######################################################\n");
		printf("EXE time: %.3f sec\n", exe_time);
		if(cntd->enable_hw_monitor)
		{
			printf("##################### ENERGY #########################\n");
			printf("PKG: 	%10.0f J\n", global_energy_pkg);
#if defined(INTEL) || defined(POWER9)
			printf("DRAM: 	%10.0f J\n", global_energy_dram);
#endif
#ifdef NVIDIA_GPU
			printf("GPU: 	%10.0f J\n", global_energy_gpu);
#elif POWER9
			printf("GPU:	%10.0f J\n", global_energy_gpu_sys);
#endif
#ifdef POWER9
			printf("SYS: 	%10.0f J\n", global_energy_sys);
#endif
			printf("##################### AVG POWER ######################\n");
			printf("PKG: 	%10.2f W\n", global_energy_pkg / exe_time);
#if defined(INTEL) || defined(POWER9)
			printf("DRAM: 	%10.2f W\n", global_energy_dram / exe_time);
#endif
#ifdef NVIDIA_GPU
			printf("GPU: 	%10.2f W\n", global_energy_gpu / exe_time);
#elif POWER9
			printf("GPU: 	%10.0f J\n", global_energy_gpu_sys / exe_time);
#endif
#ifdef POWER9
			printf("SYS: 	%10.2f W\n", global_energy_sys / exe_time);
#endif
		}

		printf("##################### MPI TIMING #####################\n");
		printf("APP time: %10.3f sec - %6.2f%%\n", app_time, (app_time/(app_time+mpi_time))*100.0);
		printf("MPI time: %10.3f sec - %6.2f%%\n", mpi_time, (mpi_time/(app_time+mpi_time))*100.0);
		printf("TOT time: %10.3f sec - 100.00%%\n", app_time+mpi_time);
		printf("##################### MPI REPORTING ##################\n");
		uint64_t cntd_impact_cnt = 0;
		double cntd_impact = 0;
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

#ifdef NVIDIA_GPU
		double global_util = 0;
		double global_util_mem = 0;
		double global_temp = 0;
		double global_clock = 0;
		uint64_t world_num_gpus = 0;

		for(i = 0; i < local_master_size; i++)
		{
			for(j = 0; j < nodeinfo[i].num_gpus; j++)
			{
				global_util += (double) gpuinfo[i].util[j] / (double) nodeinfo[i].num_sampling;
				global_util_mem += (double) gpuinfo[i].util_mem[j] / (double) nodeinfo[i].num_sampling;
				global_temp += ((double) gpuinfo[i].temp[j]) / (double) nodeinfo[i].num_sampling;
				global_clock += (double) gpuinfo[i].clock[j] / (double) nodeinfo[i].num_sampling;
			}
			world_num_gpus += nodeinfo[i].num_gpus;
		}
		global_util /= (double) world_num_gpus;
		global_util_mem /= (double) world_num_gpus;
		global_temp /= (double) world_num_gpus;
		global_clock /= (double) world_num_gpus;
		printf("##################### GPU REPORTING ##################\n");
		printf("AVG Utilization:        %4.2f%%\n", global_util);
		printf("AVG Mem Utilization:    %4.2f%%\n", global_util_mem);
		printf("AVG Temperature:        %4.2f C\n", global_temp);
		printf("AVG Frequency:          %4.0f MHz\n", global_clock);
#endif

		if(cntd->enable_cntd || cntd->enable_cntd_slack)
		{
			if(cntd->enable_cntd)
				printf("################## COUNTDOWN REPORTING ###############\n");
			else
				printf("############## COUNTDOWN SLACK REPORTING #############\n");

			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				if(cntd_mpi_type_cnt[j] > 0)
				{
					cntd_impact_cnt += cntd_mpi_type_cnt[j];
					cntd_impact += cntd_mpi_type_time[j];
					printf("%s: %d - %.3f Sec - %.2f%%\n",
						mpi_type_str[j]+2, 
						cntd_mpi_type_cnt[j], 
						cntd_mpi_type_time[j], 
						(cntd_mpi_type_time[j]/mpi_time)*100.0);
				}
			}

			if(cntd->enable_cntd)
				printf("################### COUNTDOWN SUMMARY ################\n");
			else if(cntd->enable_cntd_slack)
				printf("################ COUNTDOWN SLACK SUMMARY #############\n");
			printf("MPIs: %d - %.3f Sec - %.2f%%\n",
				cntd_impact_cnt,
				cntd_impact,
				(cntd_impact/mpi_time)*100.0);
		}

		printf("######################################################\n");
	}
}

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

	// Package
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		// Energy
		fprintf(timeseries_fd, ";energy-pkg-%d", i);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";energy-dram-%d", i);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";energy-gpus-pkg-%d", i);
#endif		
		// Power
		fprintf(timeseries_fd, ";power-pkg-%d", i);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";power-dram-%d", i);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";power-gpus-pkg-%d", i);
#endif
	}

	// GPU info
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
		fprintf(timeseries_fd, ";energy-gpu-%d;power-gpu-%d;util-gpu-%d;util-mem-gpu-%d;temp-gpu-%d;clock-gpu-%d", 
			i, i, i, i, i, i);
#endif	

#ifdef POWER9
	fprintf(timeseries_fd, ";energy-sys;power-sys");
#endif

	// End line
	fprintf(timeseries_fd, "\n");
}

HIDDEN void finalize_timeseries_report()
{
	fclose(timeseries_fd);
}

HIDDEN void print_timeseries_report(
	double time_curr, double time_prev, 
	double energy_sys, double 
	*energy_pkg, double *energy_dram, double *energy_gpu_sys, 
	double *energy_gpu,
	unsigned int *util, unsigned int *util_mem, 
	unsigned int *temp, unsigned int *clock)
{
	int i;
	double sample_duration = time_curr - time_prev;

	// Time sample
	fprintf(timeseries_fd, "%.3f", 
		time_curr - cntd->node.exe_time[START]);

	// Packages
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		// Energy
		fprintf(timeseries_fd, ";%.2f", 
			energy_pkg[i]);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";%.2f", 
			energy_dram[i]);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";%.2f", 
			energy_gpu_sys[i]);
#endif
		// Power
		fprintf(timeseries_fd, ";%.2f", 
			energy_pkg[i] / sample_duration);
#if defined(INTEL) || defined(POWER9)
		fprintf(timeseries_fd, ";%.2f", 
			energy_dram[i] / sample_duration);
#endif
#if defined(POWER9) && !defined(NVIDIA_GPU)
		fprintf(timeseries_fd, ";%.2f", 
			energy_gpu_sys[i] / sample_duration);
#endif		
	}

	// GPU info
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";%.2f;%.2f;%u;%u;%u;%u", 
			energy_gpu[i],
			energy_gpu[i] / sample_duration, 
			util[i], 
			util_mem[i], 
			temp[i], 
			clock[i]);
	}
#endif

	// System
#ifdef POWER9
	fprintf(timeseries_fd, ";%.2f;%.2f", 
		energy_sys, 
		energy_sys / sample_duration);
#endif

	fprintf(timeseries_fd, "\n");
}
