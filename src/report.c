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
	int world_size, local_master_size;

    PMPI_Comm_size(MPI_COMM_WORLD, &world_size);
	PMPI_Comm_size(cntd->comm_local_masters, &local_master_size);

	MPI_Datatype node_type = get_mpi_datatype_node();
	MPI_Datatype cpu_type = get_mpi_datatype_rank();
	MPI_Datatype gpu_type = get_mpi_datatype_gpu();
	
	CNTD_NodeInfo_t nodeinfo[local_master_size];
	CNTD_RankInfo_t rankinfo[world_size];
	CNTD_GPUInfo_t gpuinfo[local_master_size];

	PMPI_Gather(cntd->rank, 1, cpu_type, rankinfo, 1, cpu_type, 0, MPI_COMM_WORLD);
	if(cntd->rank->local_rank == 0)
	{
		PMPI_Gather(&cntd->node, 1, node_type, nodeinfo, 1, node_type, 0, cntd->comm_local_masters);
#ifdef NVIDIA_GPU
		PMPI_Gather(&cntd->gpu, 1, gpu_type, gpuinfo, 1, gpu_type, 0, cntd->comm_local_masters);
#endif
	}

	if(cntd->rank->world_rank == 0)
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

		double exe_time = rankinfo[0].exe_time[END] - rankinfo[0].exe_time[START];

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
		double mem_usage = 0;
		uint64_t mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double mpi_type_time[NUM_MPI_TYPE] = {0};
		uint64_t cntd_mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double cntd_mpi_type_time[NUM_MPI_TYPE] = {0};
		double avg_ipc = 0;
		double avg_freq = 0;
		uint64_t global_cycles = 0;
		uint64_t global_inst_ret = 0;
		uint64_t global_perf[MAX_NUM_CUSTOM_PERF] = {0};

		for(i = 0; i < world_size; i++)
		{
			app_time += rankinfo[i].app_time;
			mpi_time += rankinfo[i].mpi_time;
			mem_usage += (rankinfo[i].mem_usage / (double) rankinfo[i].num_sampling);

			if(rankinfo[i].perf[PERF_INST_RET] > 0 && rankinfo[i].perf[PERF_CYCLES] > 0)
			{
				avg_ipc += ((double) rankinfo[i].perf[PERF_INST_RET] / (double) rankinfo[i].perf[PERF_CYCLES]);
#ifdef INTEL
				if(rankinfo[i].perf[PERF_CYCLES_REF] > 0)
					avg_freq += ((double) rankinfo[i].perf[PERF_CYCLES] / (double) rankinfo[i].perf[PERF_CYCLES_REF]) * cntd->nom_freq_mhz;
#else
				avg_freq += ((double) rankinfo[i].perf[PERF_CYCLES] / (exe_time * 1.0E6));
#endif
			}
			global_cycles += rankinfo[i].perf[PERF_CYCLES];
			global_inst_ret += rankinfo[i].perf[PERF_INST_RET];
			for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
				global_perf[j] += rankinfo[i].perf[j];

			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				mpi_type_cnt[j] += rankinfo[i].mpi_type_cnt[j];
				mpi_type_time[j] += rankinfo[i].mpi_type_time[j];

				cntd_mpi_type_cnt[j] += rankinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_type_time[j] += rankinfo[i].cntd_mpi_type_time[j];

				cntd_mpi_cnt += rankinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_time += rankinfo[i].cntd_mpi_type_time[j];
			}
		}
		avg_ipc /= world_size;
		avg_freq /= world_size;

		unsigned int num_cpus = 0;
		unsigned int num_sockets = 0;
		unsigned int num_gpus = 0;
		for(i = 0; i < local_master_size; i++)
		{
			num_cpus += nodeinfo[i].num_cpus;
			num_sockets += nodeinfo[i].num_sockets;
			num_gpus += nodeinfo[i].num_gpus;
		}

		printf("######################################################\n");
		printf("##################### COUNTDOWN ######################\n");
		printf("######################################################\n");
		printf("EXE time: %.3f sec\n", exe_time);
		printf("#################### GENERAL INFO ####################\n");
		printf("Number of MPI Ranks:	%d\n", world_size);
		printf("Number of Nodes:     	%d\n", local_master_size);
		printf("Number of Sockets:     	%u\n", num_sockets);
		printf("Number of CPUs:     	%u\n", num_cpus);
#ifdef NVIDIA_GPU
		printf("Number of GPUs:         %4d\n", num_gpus);
#endif
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

		printf("################## PERFORMANCE INFO ##################\n");
		printf("AVG Memory usage:   	%.2f GB\n", mem_usage);
		printf("AVG IPC:            	%.2f\n", avg_ipc);
		printf("AVG CPU frequency:      %.0f MHz\n", avg_freq);
		printf("Cycles:                 %lu\n", global_cycles);
		printf("Instructions retired:   %lu\n", global_inst_ret);
		for(i = 0; i < MAX_NUM_CUSTOM_PERF; i++)
			if(cntd->perf_fd[i] > 0)
				printf("Perf %d:                 %lu\n", i, global_perf[i]);

#ifdef NVIDIA_GPU
		double global_util = 0;
		double global_util_mem = 0;
		double global_temp = 0;
		double global_clock = 0;

		for(i = 0; i < local_master_size; i++)
		{
			for(j = 0; j < nodeinfo[i].num_gpus; j++)
			{
				global_util += ((double) gpuinfo[i].util[j]) / (double) gpuinfo[i].num_sampling;
				global_util_mem += ((double) gpuinfo[i].util_mem[j]) / (double) gpuinfo[i].num_sampling;
				global_temp += ((double) gpuinfo[i].temp[j]) / (double) gpuinfo[i].num_sampling;
				global_clock += ((double) gpuinfo[i].clock[j]) / (double) gpuinfo[i].num_sampling;
			}
		}
		global_util /= (double) num_gpus;
		global_util_mem /= (double) num_gpus;
		global_temp /= (double) num_gpus;
		global_clock /= (double) num_gpus;
		printf("##################### GPU REPORTING ##################\n");
		printf("AVG Utilization:        %4.2f%%\n", global_util);
		printf("AVG Mem Utilization:    %4.2f%%\n", global_util_mem);
		printf("AVG Temperature:        %4.2f C\n", global_temp);
		printf("AVG Frequency:          %4.0f MHz\n", global_clock);
#endif

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
				printf("%s: %lu - %.3f Sec - %.2f%%\n", 
					mpi_type_str[j]+2, 
					mpi_type_cnt[j], 
					mpi_type_time[j], 
					(mpi_type_time[j]/mpi_time)*100.0);
			}
		}

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
					printf("%s: %lu - %.3f Sec - %.2f%%\n",
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
			printf("MPIs: %lu - %.3f Sec - MPI: %.2f%% - TOT: %.2f%%\n",
				cntd_impact_cnt,
				cntd_impact,
				(cntd_impact/mpi_time)*100.0,
				(cntd_impact/(app_time+mpi_time))*100.0);
		}

		printf("######################################################\n");

		if(cntd->enable_rank_report)
		{
			uint64_t mpi_num;
			double mpi_time;
			char filename[STRING_SIZE];

			snprintf(filename, STRING_SIZE, "%s/"RANK_REPORT_FILE, cntd->log_dir);
			FILE *rank_report_fd = fopen(filename, "w");
			if(rank_report_fd == NULL)
			{
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the rank report: %s\n", 
					cntd->node.hostname, cntd->rank->world_rank, filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			fprintf(rank_report_fd, "Rank");
			for(j = 0; j < NUM_MPI_TYPE; j++)
				if(mpi_type_cnt[j] > 0)
					fprintf(rank_report_fd, ";%s-NUM;%s-TIME", mpi_type_str[j]+2, mpi_type_str[j]+2);
			fprintf(rank_report_fd, ";MPI-NUM;MPI-TIME\n");

			for(i = 0; i < world_size; i++)
			{
				mpi_num = 0;
				mpi_time = 0;
				fprintf(rank_report_fd, "%d", rankinfo[i].world_rank);
				for(j = 0; j < NUM_MPI_TYPE; j++)
					if(mpi_type_cnt[j] > 0)
					{
						mpi_num += rankinfo[i].mpi_type_cnt[j];
						mpi_time += rankinfo[i].mpi_type_time[j];
						fprintf(rank_report_fd, ";%lu;%.9f", rankinfo[i].mpi_type_cnt[j], rankinfo[i].mpi_type_time[j]);
					}
				fprintf(rank_report_fd, ";%lu;%.9f\n", mpi_num, mpi_time);
			}

			fclose(rank_report_fd);
		}
	}

	PMPI_Barrier(MPI_COMM_WORLD);
}

HIDDEN void init_timeseries_report()
{
	int i, j;
	char filename[STRING_SIZE];

	snprintf(filename, STRING_SIZE, "%s/"TIME_SERIES_FILE, TMP_DIR, cntd->node.hostname);
	timeseries_fd = fopen(filename, "w");
	if(timeseries_fd == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed create time-series file '%s'!\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
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
		fprintf(timeseries_fd, ";energy-gpus-pkg-%d", i);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->gpu.num_gpus; i++)
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
		fprintf(timeseries_fd, ";power-gpus-pkg-%d", i);
#endif
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";power-gpu-%d", i);
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";power-sys");
#endif

	// GPU info
#ifdef NVIDIA_GPU
	// GPU utilization
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";util-gpu-%d", i);
	// GPU memory utilization
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";util-gpu-%d", i);
	// GPU temperature
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";temp-gpu-%d", i);
	// Clock temperature
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";clock-gpu-%d", i);
#endif	

	// Memery usage
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";rank-%d-cpu-%d-mem_usage", 
			cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

	// Average Frequency
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";rank-%d-cpu-%d-freq", 
			cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

	// Average IPC
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";rank-%d-cpu-%d-ipc", 
			cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

	// Average cycles
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";rank-%d-cpu-%d-cycles", 
			cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

	// Average Instructions retired
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";rank-%d-cpu-%d-inst_ret", 
			cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

	// Linux perf
	for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
		for(i = 0; i < cntd->num_local_ranks; i++)
			if(cntd->perf_fd[j] > 0)
				fprintf(timeseries_fd, ";rank-%d-cpu-%d-perf-%d", 
					cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id, j);

	// End line
	fprintf(timeseries_fd, "\n");
}

HIDDEN void finalize_timeseries_report()
{
	char oldname[STRING_SIZE];
	char newname[STRING_SIZE];

	fclose(timeseries_fd);

	snprintf(oldname, STRING_SIZE, "%s/cntd_%s.csv", TMP_DIR, cntd->node.hostname);
	snprintf(newname, STRING_SIZE, "%s/cntd_%s.csv", cntd->log_dir, cntd->node.hostname);

	int rc = copyFile(oldname, newname);
	int rc2 = remove(oldname);
}

HIDDEN void print_timeseries_report(
	double time_curr, double time_prev, 
	double energy_sys, double *energy_pkg, double *energy_dram, 
	double *energy_gpu_sys, double *energy_gpu,
	double mem_usage,
	unsigned int *util_gpu, unsigned int *util_mem_gpu, 
	unsigned int *temp, unsigned int *clock)
{
	int i, j;
	double sample_duration = time_curr - time_prev;

	// Time sample
	fprintf(timeseries_fd, "%.3f", 
		time_curr - cntd->rank->exe_time[START]);

	// Energy
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
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
	}
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%.2f", energy_gpu[i]);
#endif	
#ifdef POWER9
	fprintf(timeseries_fd, ";%.2f", energy_sys);
#endif

	// Power
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
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
#ifdef NVIDIA_GPU
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%.2f",
			energy_gpu[i] / sample_duration);
#endif
#ifdef POWER9
	fprintf(timeseries_fd, ";%.2f", 
		energy_sys / sample_duration);
#endif

	// GPU info
#ifdef NVIDIA_GPU
	// GPU utilization
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%u", util_gpu[i]);
	// GPU memory utilization
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%u", util_mem_gpu[i]);
	// GPU temperature
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%u", temp[i]);
	// Clock temperature
	for(i = 0; i < cntd->gpu.num_gpus; i++)
		fprintf(timeseries_fd, ";%u", clock[i]);
#endif

	// Memery usage
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";%.2f", mem_usage);

	// Average Frequency
	for(i = 0; i < cntd->num_local_ranks; i++)
	{
#ifdef INTEL
		if(cntd->local_ranks[i]->perf_curr[PERF_CYCLES] > 0 && cntd->local_ranks[i]->perf_curr[PERF_CYCLES_REF] > 0)
			fprintf(timeseries_fd, ";%.0f", 
				((double) cntd->local_ranks[i]->perf_curr[PERF_CYCLES] / (double) cntd->local_ranks[i]->perf_curr[PERF_CYCLES_REF]) * cntd->nom_freq_mhz);
		else
			fprintf(timeseries_fd, ";0");
#else
		if(cntd->local_ranks[i]->perf_curr[PERF_CYCLES] > 0))
			fprintf(timeseries_fd, ";%.0f", 
				(double) cntd->local_ranks[i]->perf_curr[PERF_CYCLES] / ((double) sample_duration * 1.0E6));
		else
			fprintf(timeseries_fd, ";0");
#endif
	}

	// Average IPC
	for(i = 0; i < cntd->num_local_ranks; i++)
	{
		if(cntd->local_ranks[i]->perf_curr[PERF_CYCLES] > 0 && cntd->local_ranks[i]->perf_curr[PERF_INST_RET] > 0)
			fprintf(timeseries_fd, ";%.2f", 
				(double) cntd->local_ranks[i]->perf_curr[PERF_INST_RET] / (double) cntd->local_ranks[i]->perf_curr[PERF_CYCLES]);
		else
			fprintf(timeseries_fd, ";0");
	}

	// Average cycles
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf_curr[PERF_CYCLES]);

	// Average Instructions retired
	for(i = 0; i < cntd->num_local_ranks; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf_curr[PERF_INST_RET]);

	// Linux perf
	for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
	{
		for(i = 0; i < cntd->num_local_ranks; i++)
		{
			if(cntd->perf_fd[j] > 0)
				fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf_curr[j]);
		}
	}

	fprintf(timeseries_fd, "\n");
}
