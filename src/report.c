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

static void print_rank_mpi(CNTD_RankInfo_t *rankinfo, uint64_t *mpi_type_cnt)
{
	int i, j, world_size;
	char filename[STRING_SIZE];

	PMPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Create file
	snprintf(filename, STRING_SIZE, "%s/"RANK_MPI_REPORT_FILE, cntd->log_dir);
	FILE *fd = fopen(filename, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the rank mpi report: %s\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Labels
	fprintf(fd, "rank");
	for(j = 0; j < NUM_MPI_TYPE; j++)
		if(mpi_type_cnt[j] > 0)
			fprintf(fd, ";%s-NUM", mpi_type_str[j]+2);
	for(j = 0; j < NUM_MPI_TYPE; j++)
		if(mpi_type_cnt[j] > 0)
			fprintf(fd, ";%s-TIME", mpi_type_str[j]+2);
	for(j = 0; j < NUM_MPI_TYPE; j++)
		if(mpi_type_cnt[j] > 0)
			fprintf(fd, ";%s-DATA_SEND", mpi_type_str[j]+2);
	for(j = 0; j < NUM_MPI_TYPE; j++)
		if(mpi_type_cnt[j] > 0)
			fprintf(fd, ";%s-DATA_RECV", mpi_type_str[j]+2);
	fprintf(fd, "\n");

	// Data
	for(i = 0; i < world_size; i++)
	{
		fprintf(fd, "%d", rankinfo[i].world_rank);
		for(j = 0; j < NUM_MPI_TYPE; j++)
			if(mpi_type_cnt[j] > 0)
				fprintf(fd, ";%lu", rankinfo[i].mpi_type_cnt[j]);
		for(j = 0; j < NUM_MPI_TYPE; j++)
			if(mpi_type_cnt[j] > 0)
				fprintf(fd, ";%.9f", rankinfo[i].mpi_type_time[j]);
		for(j = 0; j < NUM_MPI_TYPE; j++)
			if(mpi_type_cnt[j] > 0)
				fprintf(fd, ";%lu", rankinfo[i].mpi_type_data[SEND][j]);
		for(j = 0; j < NUM_MPI_TYPE; j++)
			if(mpi_type_cnt[j] > 0)
				fprintf(fd, ";%lu", rankinfo[i].mpi_type_data[RECV][j]);
		fprintf(fd, "\n");
	}

	fclose(fd);
}

static void print_rank(CNTD_RankInfo_t *rankinfo, double exe_time)
{
	int i, j, world_size;
	char filename[STRING_SIZE];

	PMPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Create file
	snprintf(filename, STRING_SIZE, "%s/"RANK_REPORT_FILE, cntd->log_dir);
	FILE *fd = fopen(filename, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the rank report: %s\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Labels
	fprintf(fd, "rank;hostname;cpu_id;app_time;mpi_time;max_mem_usage;ipc;freq;load;cycles;inst_ret;dp_flops_tot;dp_flops_64;dp_flops_128;dp_flops_256;dp_flops_512;dp_uops_tot(time_en);dp_uops_tot(time_run);dp_uops_64(time_en);dp_uops_64(time_run);dp_uops_128(time_en);dp_uops_128(time_run);dp_uops_256(time_en);dp_uops_256(time_run);dp_uops_512(time_en);dp_uops_512(time_run);sp_flops_tot;sp_flops_32;sp_flops_128;sp_flops_256;sp_flops_512;sp_uops_tot(time_en);sp_uops_tot(time_run);sp_uops_32(time_en);sp_uops_32(time_run);sp_uops_128(time_en);sp_uops_128(time_run);sp_uops_256(time_en);sp_uops_256(time_run);sp_uops_512(time_en);sp_uops_512(time_run);mem_cas_count(time_en);mem_cas_count(time_run);mem_data_tot");
	for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
		if(cntd->perf_fd[0][j] > 0)
			fprintf(fd, ";perf_event_%d", j);
	fprintf(fd, "\n");

	// Data
	const char* format = "%d;%s;%d;%.9f;%.9f;%ld;%.3f;%0.f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu;%lu;%lu;%lu;%lu;%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu(%lu);%lu";

    uint64_t time_steps = ceil(exe_time/cntd->sampling_time);
	for(i = 0; i < world_size; i++)
	{
		uint64_t dp_uops_64 = rankinfo[i].perf[PERF_SCALAR_DOUBLE][TOT];
		uint64_t dp_uops_128 = rankinfo[i].perf[PERF_128_PACKED_DOUBLE][TOT];
		uint64_t dp_uops_256 = rankinfo[i].perf[PERF_256_PACKED_DOUBLE][TOT];
		uint64_t dp_uops_512 = rankinfo[i].perf[PERF_512_PACKED_DOUBLE][TOT];
		uint64_t dp_uops_tot = (dp_uops_64 + dp_uops_128 + dp_uops_256 + dp_uops_512);
		uint64_t dp_flops_64 = dp_uops_64;
		uint64_t dp_flops_128 = (dp_uops_128 * 2);
		uint64_t dp_flops_256 = (dp_uops_256 * 4);
		uint64_t dp_flops_512 = (dp_uops_512 * 8);
		uint64_t dp_flops_tot = (dp_flops_64 + dp_flops_128 + dp_flops_256 + dp_flops_512);
		uint64_t sp_uops_32 = rankinfo[i].perf[PERF_SCALAR_SINGLE][TOT];
		uint64_t sp_uops_128 = rankinfo[i].perf[PERF_128_PACKED_SINGLE][TOT];
		uint64_t sp_uops_256 = rankinfo[i].perf[PERF_256_PACKED_SINGLE][TOT];
		uint64_t sp_uops_512 = rankinfo[i].perf[PERF_512_PACKED_SINGLE][TOT];
		uint64_t sp_uops_tot = (sp_uops_32 + sp_uops_128 + sp_uops_256 + sp_uops_512);
		uint64_t sp_flops_32 = sp_uops_32;
		uint64_t sp_flops_128 = (sp_uops_128 * 4);
		uint64_t sp_flops_256 = (sp_uops_256 * 8);
		uint64_t sp_flops_512 = (sp_uops_512 * 16);
		uint64_t sp_flops_tot = (sp_flops_32 + sp_flops_128 + sp_flops_256 + sp_flops_512);
		uint64_t mem = 0;
		uint64_t time_en_mem = 0;
		uint64_t time_run_mem = 0;
        double load = rankinfo[i].load[TOT]/time_steps;

		if (i == 0) {
			int j;
			int k;
			int t_k; // temporal index.

			for (j = 0; j < cntd->node.num_sockets; j++) {
				for (k = 0; k < MAX_NUM_MEM_CHANNELS_PER_SOCKET; k++) {
					t_k = PERF_CAS_COUNT_ALL + k + (j * MAX_NUM_MEM_CHANNELS_PER_SOCKET);
					mem += rankinfo[i].perf[t_k][TOT];
					time_en_mem += rankinfo[i].perf_te[t_k][TOT];
					time_run_mem += rankinfo[i].perf_tr[t_k][TOT];
				}
			}
		}

		uint64_t mem_data = (mem * 64);

		uint64_t time_en_dp_uops_64 = rankinfo[i].perf_te[PERF_SCALAR_DOUBLE][TOT];
		uint64_t time_en_dp_uops_128 = rankinfo[i].perf_te[PERF_128_PACKED_DOUBLE][TOT];
		uint64_t time_en_dp_uops_256 = rankinfo[i].perf_te[PERF_256_PACKED_DOUBLE][TOT];
		uint64_t time_en_dp_uops_512 = rankinfo[i].perf_te[PERF_512_PACKED_DOUBLE][TOT];
		uint64_t time_en_dp_uops_tot = time_en_dp_uops_64  +
									   time_en_dp_uops_128 +
									   time_en_dp_uops_256 +
									   time_en_dp_uops_512;
		uint64_t time_en_sp_uops_32 = rankinfo[i].perf_te[PERF_SCALAR_SINGLE][TOT];
		uint64_t time_en_sp_uops_128 = rankinfo[i].perf_te[PERF_128_PACKED_SINGLE][TOT];
		uint64_t time_en_sp_uops_256 = rankinfo[i].perf_te[PERF_256_PACKED_SINGLE][TOT];
		uint64_t time_en_sp_uops_512 = rankinfo[i].perf_te[PERF_512_PACKED_SINGLE][TOT];
		uint64_t time_en_sp_uops_tot = time_en_sp_uops_32  +
									   time_en_sp_uops_128 +
									   time_en_sp_uops_256 +
									   time_en_sp_uops_512;
		uint64_t time_run_dp_uops_64 = rankinfo[i].perf_tr[PERF_SCALAR_DOUBLE][TOT];
		uint64_t time_run_dp_uops_128 = rankinfo[i].perf_tr[PERF_128_PACKED_DOUBLE][TOT];
		uint64_t time_run_dp_uops_256 = rankinfo[i].perf_tr[PERF_256_PACKED_DOUBLE][TOT];
		uint64_t time_run_dp_uops_512 = rankinfo[i].perf_tr[PERF_512_PACKED_DOUBLE][TOT];
		uint64_t time_run_dp_uops_tot = time_run_dp_uops_64  +
										time_run_dp_uops_128 +
										time_run_dp_uops_256 +
										time_run_dp_uops_512;
		uint64_t time_run_sp_uops_32 = rankinfo[i].perf_tr[PERF_SCALAR_SINGLE][TOT];
		uint64_t time_run_sp_uops_128 = rankinfo[i].perf_tr[PERF_128_PACKED_SINGLE][TOT];
		uint64_t time_run_sp_uops_256 = rankinfo[i].perf_tr[PERF_256_PACKED_SINGLE][TOT];
		uint64_t time_run_sp_uops_512 = rankinfo[i].perf_tr[PERF_512_PACKED_SINGLE][TOT];
		uint64_t time_run_sp_uops_tot = time_run_sp_uops_32  +
									    time_run_sp_uops_128 +
									    time_run_sp_uops_256 +
									    time_run_sp_uops_512;

		fprintf(fd, format,
			rankinfo[i].world_rank, 
			rankinfo[i].hostname, 
			rankinfo[i].cpu_id,
			rankinfo[i].app_time[TOT],
			rankinfo[i].mpi_time[TOT],
			rankinfo[i].max_mem_usage * 1024,
			rankinfo[i].perf[PERF_CYCLES][TOT] > 0 ? (double) rankinfo[i].perf[PERF_INST_RET][TOT] / (double) rankinfo[i].perf[PERF_CYCLES][TOT] : 0,
#ifdef INTEL
			rankinfo[i].perf[PERF_CYCLES_REF][TOT] > 0 ? ((double) rankinfo[i].perf[PERF_CYCLES][TOT] / (double) rankinfo[i].perf[PERF_CYCLES_REF][TOT]) * cntd->nom_freq_mhz : 0,
#else
			(double) rankinfo[i].perf[PERF_CYCLES][TOT] / (exe_time * 1.0E6),
#endif
            load                ,
			rankinfo[i].perf[PERF_CYCLES][TOT],
			rankinfo[i].perf[PERF_INST_RET][TOT],
			dp_flops_tot		,
			dp_flops_64 		,
			dp_flops_128		,
			dp_flops_256		,
			dp_flops_512		,
			dp_uops_tot			,
			time_en_dp_uops_tot ,
			time_run_dp_uops_tot,
			dp_uops_64  		,
			time_en_dp_uops_64  ,
			time_run_dp_uops_64 ,
			dp_uops_128 		,
			time_en_dp_uops_128 ,
			time_run_dp_uops_128,
			dp_uops_256 		,
			time_en_dp_uops_256 ,
			time_run_dp_uops_256,
			dp_uops_512 		,
			time_en_dp_uops_512 ,
			time_run_dp_uops_512,
			sp_flops_tot		,
			sp_flops_32 		,
			sp_flops_128		,
			sp_flops_256		,
			sp_flops_512		,
			sp_uops_tot			,
			time_en_sp_uops_tot ,
			time_run_sp_uops_tot,
			sp_uops_32  		,
			time_en_sp_uops_32 ,
			time_run_sp_uops_32,
			sp_uops_128 		,
			time_en_sp_uops_128 ,
			time_run_sp_uops_128,
			sp_uops_256 		,
			time_en_sp_uops_256 ,
			time_run_sp_uops_256,
			sp_uops_512 		,
			time_en_sp_uops_512 ,
			time_run_sp_uops_512,
			mem		    		,
			time_en_mem			,
			time_run_mem		,
			mem_data    );
		for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
			if(cntd->perf_fd[0][j] > 0)
				fprintf(fd, ";%lu", rankinfo[i].perf[j][TOT]);
		fprintf(fd, "\n");
	}

	fclose(fd);
}

static void print_mpi_report(uint64_t *mpi_type_cnt, double *mpi_type_time, uint64_t *mpi_data_send, uint64_t *mpi_data_recv)
{
	int i;
	char filename[STRING_SIZE];

	// Create file
	snprintf(filename, STRING_SIZE, "%s/"MPI_REPORT_FILE, cntd->log_dir);
	FILE *fd = fopen(filename, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the mpi report: %s\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Labels
	fprintf(fd, "type;number;time;data_send;data_recv\n");

	// Data
	for(i = 0; i < NUM_MPI_TYPE; i++)
		if(mpi_type_cnt[i] > 0)
			fprintf(fd, "%s;%lu;%.9f;%lu;%lu\n", 
				mpi_type_str[i]+2, 
				mpi_type_cnt[i], 
				mpi_type_time[i], 
				mpi_data_send[i],
				mpi_data_recv[i]);

	fclose(fd);
}

static void print_eam_report(uint64_t *cntd_mpi_type_cnt, double *cntd_mpi_type_time)
{
	int i;
	char filename[STRING_SIZE];

	// Create file
	if(cntd->enable_cntd)
		snprintf(filename, STRING_SIZE, "%s/"EAM_REPORT_FILE, cntd->log_dir);
	else if(cntd->enable_cntd_slack)
		snprintf(filename, STRING_SIZE, "%s/"EAM_SLACK_REPORT_FILE, cntd->log_dir);
	else
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Misconfiguration of eam report call: %s\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(filename, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the eam report: %s\n", 
			cntd->node.hostname, cntd->rank->world_rank, filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Labels
	fprintf(fd, "type;number;time\n");

	// Data
	for(i = 0; i < NUM_MPI_TYPE; i++)
		if(cntd_mpi_type_cnt[i] > 0)
			fprintf(fd, "%s;%lu;%.9f\n", 
				mpi_type_str[i]+2, 
				cntd_mpi_type_cnt[i], 
				cntd_mpi_type_time[i]);

	fclose(fd);
}

HIDDEN void print_final_report()
{
	int i, j;
	int world_size, local_master_size;
	char filename[STRING_SIZE];
	FILE *summary_report_fd;

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
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
		double global_energy_pkg = 0;
#endif
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

		if(cntd->enable_power_monitor)
		{
			// Energy reduction
			for(i = 0; i < local_master_size; i++)
			{
#ifdef POWER9
				global_energy_sys += nodeinfo[i].energy_sys;
#endif
            	for(j = 0; j < nodeinfo[i].num_sockets; j++)
				{
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
					global_energy_pkg += nodeinfo[i].energy_pkg[j];
#endif
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
		uint64_t max_mem_usage = 0;
		uint64_t mpi_net_data[2] =  {0};
		uint64_t mpi_file_data[2] =  {0};
		uint64_t mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double mpi_type_time[NUM_MPI_TYPE] = {0};
		uint64_t mpi_type_data[2][NUM_MPI_TYPE] = {0};
		uint64_t cntd_mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double cntd_mpi_type_time[NUM_MPI_TYPE] = {0};
		double avg_ipc = 0;
		double avg_freq = 0;
        double global_dp_flops_sec = 0.0;
        double global_sp_flops_sec = 0.0;
		uint64_t global_cycles = 0;
		uint64_t global_inst_ret = 0;

		uint64_t global_dp_flops = 0;
		uint64_t global_dp_flops_64 = 0;
		uint64_t global_dp_flops_128 = 0;
		uint64_t global_dp_flops_256 = 0;
		uint64_t global_dp_flops_512 = 0;
		uint64_t global_dp_uops = 0;
		uint64_t global_dp_uops_64 = 0;
		uint64_t global_dp_uops_128 = 0;
		uint64_t global_dp_uops_256 = 0;
		uint64_t global_dp_uops_512 = 0;
		uint64_t global_mem = 0;
        double global_mem_bandwidth = 0.0;
		uint64_t global_mem_data = 0;
		uint64_t global_sp_flops = 0;
		uint64_t global_sp_flops_32 = 0;
		uint64_t global_sp_flops_128 = 0;
		uint64_t global_sp_flops_256 = 0;
		uint64_t global_sp_flops_512 = 0;
		uint64_t global_sp_uops = 0;
		uint64_t global_sp_uops_32 = 0;
		uint64_t global_sp_uops_128 = 0;
		uint64_t global_sp_uops_256 = 0;
		uint64_t global_sp_uops_512 = 0;
		uint64_t global_time_en_dp_uops = 0;
		uint64_t global_time_en_dp_uops_64 = 0;
		uint64_t global_time_en_dp_uops_128 = 0;
		uint64_t global_time_en_dp_uops_256 = 0;
		uint64_t global_time_en_dp_uops_512 = 0;
		uint64_t global_time_en_mem = 0;
		uint64_t global_time_en_sp_uops = 0;
		uint64_t global_time_en_sp_uops_32 = 0;
		uint64_t global_time_en_sp_uops_128 = 0;
		uint64_t global_time_en_sp_uops_256 = 0;
		uint64_t global_time_en_sp_uops_512 = 0;
		uint64_t global_time_run_dp_uops = 0;
		uint64_t global_time_run_dp_uops_64 = 0;
		uint64_t global_time_run_dp_uops_128 = 0;
		uint64_t global_time_run_dp_uops_256 = 0;
		uint64_t global_time_run_dp_uops_512 = 0;
		uint64_t global_time_run_mem = 0;
		uint64_t global_time_run_sp_uops = 0;
		uint64_t global_time_run_sp_uops_32 = 0;
		uint64_t global_time_run_sp_uops_128 = 0;
		uint64_t global_time_run_sp_uops_256 = 0;
		uint64_t global_time_run_sp_uops_512 = 0;

		uint64_t global_perf[MAX_NUM_CUSTOM_PERF] = {0};
		int perf_flag = FALSE;

        double avg_load = 0.0;
        uint64_t time_steps = (uint64_t)(ceil(exe_time/cntd->sampling_time));
		for(i = 0; i < world_size; i++)
		{
			app_time += rankinfo[i].app_time[TOT];
			mpi_time += rankinfo[i].mpi_time[TOT];

			max_mem_usage += rankinfo[i].max_mem_usage;

			mpi_net_data[SEND] += rankinfo[i].mpi_net_data[SEND][TOT];
			mpi_net_data[RECV] += rankinfo[i].mpi_net_data[RECV][TOT];

			mpi_file_data[SEND] += rankinfo[i].mpi_file_data[SEND][TOT];
			mpi_file_data[RECV] += rankinfo[i].mpi_file_data[RECV][TOT];

			if(rankinfo[i].perf[PERF_CYCLES][TOT] == 0 || rankinfo[i].perf[PERF_INST_RET][TOT] == 0)
				perf_flag = TRUE;

			avg_ipc += rankinfo[i].perf[PERF_CYCLES][TOT] > 0 ? ((double) rankinfo[i].perf[PERF_INST_RET][TOT] / (double) rankinfo[i].perf[PERF_CYCLES][TOT]) : 0;
#ifdef INTEL
			if(rankinfo[i].perf[PERF_CYCLES_REF][TOT] == 0)
				perf_flag = TRUE;
			avg_freq += rankinfo[i].perf[PERF_CYCLES_REF][TOT] > 0 ? ((double) rankinfo[i].perf[PERF_CYCLES][TOT] / (double) rankinfo[i].perf[PERF_CYCLES_REF][TOT]) * cntd->nom_freq_mhz : 0;
#else
			avg_freq += ((double) rankinfo[i].perf[PERF_CYCLES][TOT] / (exe_time * 1.0E6));
#endif
			global_cycles += rankinfo[i].perf[PERF_CYCLES][TOT];
			global_inst_ret += rankinfo[i].perf[PERF_INST_RET][TOT];

			global_dp_uops_64 += rankinfo[i].perf[PERF_SCALAR_DOUBLE][TOT];
			global_time_en_dp_uops_64 += rankinfo[i].perf_te[PERF_SCALAR_DOUBLE][TOT];
			global_time_run_dp_uops_64 += rankinfo[i].perf_tr[PERF_SCALAR_DOUBLE][TOT];
			global_sp_uops_32 += rankinfo[i].perf[PERF_SCALAR_SINGLE][TOT];
			global_time_en_sp_uops_32 += rankinfo[i].perf_te[PERF_SCALAR_SINGLE][TOT];
			global_time_run_sp_uops_32 += rankinfo[i].perf_tr[PERF_SCALAR_SINGLE][TOT];
			global_dp_uops_128 += rankinfo[i].perf[PERF_128_PACKED_DOUBLE][TOT];
			global_time_en_dp_uops_128 += rankinfo[i].perf_te[PERF_128_PACKED_DOUBLE][TOT];
			global_time_run_dp_uops_128 += rankinfo[i].perf_tr[PERF_128_PACKED_DOUBLE][TOT];
			global_sp_uops_128 += rankinfo[i].perf[PERF_128_PACKED_SINGLE][TOT];
			global_time_en_sp_uops_128 += rankinfo[i].perf_te[PERF_128_PACKED_SINGLE][TOT];
			global_time_run_sp_uops_128 += rankinfo[i].perf_tr[PERF_128_PACKED_SINGLE][TOT];
			global_dp_uops_256 += rankinfo[i].perf[PERF_256_PACKED_DOUBLE][TOT];
			global_time_en_dp_uops_256 += rankinfo[i].perf_te[PERF_256_PACKED_DOUBLE][TOT];
			global_time_run_dp_uops_256 += rankinfo[i].perf_tr[PERF_256_PACKED_DOUBLE][TOT];
			global_sp_uops_256 += rankinfo[i].perf[PERF_256_PACKED_SINGLE][TOT];
			global_time_en_sp_uops_256 += rankinfo[i].perf_te[PERF_256_PACKED_SINGLE][TOT];
			global_time_run_sp_uops_256 += rankinfo[i].perf_tr[PERF_256_PACKED_SINGLE][TOT];
			global_dp_uops_512 += rankinfo[i].perf[PERF_512_PACKED_DOUBLE][TOT];
			global_time_en_dp_uops_512 += rankinfo[i].perf_te[PERF_512_PACKED_DOUBLE][TOT];
			global_time_run_dp_uops_512 += rankinfo[i].perf_tr[PERF_512_PACKED_DOUBLE][TOT];
			global_sp_uops_512 += rankinfo[i].perf[PERF_512_PACKED_SINGLE][TOT];
			global_time_en_sp_uops_512 += rankinfo[i].perf_te[PERF_512_PACKED_SINGLE][TOT];
			global_time_run_sp_uops_512 += rankinfo[i].perf_tr[PERF_512_PACKED_SINGLE][TOT];

#ifdef INTEL
			if (rankinfo[i].local_rank == 0) {
				int j;
				int k;
				int t_k; // temporal index.

				for (j = 0; j < cntd->node.num_sockets; j++) {
					for (k = 0; k < MAX_NUM_MEM_CHANNELS_PER_SOCKET; k++) {
						t_k = PERF_CAS_COUNT_ALL + k + (j * MAX_NUM_MEM_CHANNELS_PER_SOCKET);
						global_mem += rankinfo[i].perf[t_k][TOT];
						global_time_en_mem += rankinfo[i].perf_te[t_k][TOT];
						global_time_run_mem += rankinfo[i].perf_tr[t_k][TOT];
					}
				}
			}
#endif

			for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
				global_perf[j] += rankinfo[i].perf[j][TOT];

			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				mpi_type_cnt[j] += rankinfo[i].mpi_type_cnt[j];
				mpi_type_time[j] += rankinfo[i].mpi_type_time[j];
				mpi_type_data[SEND][j] += rankinfo[i].mpi_type_data[SEND][j];
				mpi_type_data[RECV][j] += rankinfo[i].mpi_type_data[RECV][j];

				cntd_mpi_type_cnt[j] += rankinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_type_time[j] += rankinfo[i].cntd_mpi_type_time[j];

				cntd_mpi_cnt += rankinfo[i].cntd_mpi_type_cnt[j];
				cntd_mpi_time += rankinfo[i].cntd_mpi_type_time[j];
			}

            avg_load += rankinfo[i].load[TOT];
		}

		global_dp_flops_64 = global_dp_uops_64;
		global_dp_flops_128 = (global_dp_uops_128 * 2);
		global_dp_flops_256 = (global_dp_uops_256 * 4);
		global_dp_flops_512 = (global_dp_uops_512 * 8);
		global_dp_flops = (global_dp_flops_64  +
						   global_dp_flops_128 +
						   global_dp_flops_256 +
						   global_dp_flops_512);

		global_dp_uops = (global_dp_uops_64  +
						  global_dp_uops_128 +
						  global_dp_uops_256 +
						  global_dp_uops_512);

		global_time_en_dp_uops = (global_time_en_dp_uops_64  +
						          global_time_en_dp_uops_128 +
								  global_time_en_dp_uops_256 +
								  global_time_en_dp_uops_512);

		global_time_run_dp_uops = (global_time_run_dp_uops_64  +
								   global_time_run_dp_uops_128 +
								   global_time_run_dp_uops_256 +
								   global_time_run_dp_uops_512);

		global_mem_data = (global_mem * 64);

		global_sp_flops_32 = global_sp_uops_32;
		global_sp_flops_128 = (global_sp_uops_128 * 4);
		global_sp_flops_256 = (global_sp_uops_256 * 8);
		global_sp_flops_512 = (global_sp_uops_512 * 16);
		global_sp_flops = (global_sp_flops_32  +
						   global_sp_flops_128 +
						   global_sp_flops_256 +
						   global_sp_flops_512);

		global_sp_uops = (global_sp_uops_32  +
						  global_sp_uops_128 +
						  global_sp_uops_256 +
						  global_sp_uops_512);

		global_time_en_sp_uops = (global_time_en_sp_uops_32  +
								  global_time_en_sp_uops_128 +
								  global_time_en_sp_uops_256 +
								  global_time_en_sp_uops_512);

		global_time_run_sp_uops = (global_time_run_sp_uops_32  +
								   global_time_run_sp_uops_128 +
								   global_time_run_sp_uops_256 +
								   global_time_run_sp_uops_512);

        avg_load = ((double)avg_load/(double)time_steps);
        avg_load = ((double)avg_load/(double)world_size);
		if(perf_flag)
		{
			avg_ipc = 0;
			avg_freq = 0;
			global_cycles = 0;
			global_inst_ret = 0;
		}
		else
		{
			avg_ipc /= world_size;
			avg_freq /= world_size;
		}

		unsigned int num_cpus = 0;
		unsigned int num_sockets = 0;
		unsigned int num_gpus = 0;
		for(i = 0; i < local_master_size; i++)
		{
			num_cpus += nodeinfo[i].num_cpus;
			num_sockets += nodeinfo[i].num_sockets;
			num_gpus += nodeinfo[i].num_gpus;
		}

		// Create summary report file with labels
		if(cntd->enable_report)
		{
			snprintf(filename, STRING_SIZE, "%s/"SUMMARY_REPORT_FILE, cntd->log_dir);
			summary_report_fd = fopen(filename, "w");
			if(summary_report_fd == NULL)
			{
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to create the summary report: %s\n", 
					cntd->node.hostname, cntd->rank->world_rank, filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			fprintf(summary_report_fd, "exe_time;num_mpi_ranks;num_nodes;num_sockets;num_cpus");
#ifdef NVIDIA_GPU
			fprintf(summary_report_fd, ";num_gpus");
#endif
			if(cntd->enable_power_monitor)
			{
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
				fprintf(summary_report_fd, ";energy_pkg");
#endif
#if defined(INTEL) || defined(POWER9)
				fprintf(summary_report_fd, ";energy_dram");
#endif
#if defined(NVIDIA_GPU) || defined(POWER9)
				fprintf(summary_report_fd, ";energy_gpu");
#endif
#ifdef POWER9
				fprintf(summary_report_fd, ";energy_sys");
#endif
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
				fprintf(summary_report_fd, ";power_pkg");
#endif
#if defined(INTEL) || defined(POWER9)
				fprintf(summary_report_fd, ";power_dram");
#endif
#if defined(NVIDIA_GPU) || defined(POWER9)
				fprintf(summary_report_fd, ";power_gpu");
#endif
#ifdef POWER9
				fprintf(summary_report_fd, ";power_sys");
#endif
			}
			fprintf(summary_report_fd, ";mpi_net_send;mpi_net_recv;mpi_file_write;mpi_file_read;max_mem_usage;ipc;freq;load;cycles;inst_ret;dp_flops_tot;dp_flops_64;dp_flops_128;dp_flops_256;dp_flops_512;dp_uops_tot(time_en/time_run);dp_uops_64(time_en/time_run);dp_uops_128(time_en/time_run);dp_uops_256(time_en/time_run);dp_uops_512(time_en/time_run);sp_flops_tot;sp_flops_32;sp_flops_128;sp_flops_256;sp_flops_512;sp_uops_tot(time_en/time_run);sp_uops_32(time_en/time_run);sp_uops_128(time_en/time_run);sp_uops_256(time_en/time_run);sp_uops_512(time_en/time_run);mem_uops(time_en/time_run);mem_data_tot");
			for(i = 0; i < MAX_NUM_CUSTOM_PERF; i++)
				if(cntd->perf_fd[0][i] > 0)
					fprintf(summary_report_fd, ";perf_even_%d", i);
#ifdef NVIDIA_GPU
			fprintf(summary_report_fd, ";gpu_util;gpu_mem_util;gpu_temp;gpu_freq");
#endif
			fprintf(summary_report_fd, ";app_time;mpi_time;tot_time");

			if(cntd->enable_cntd || cntd->enable_cntd_slack)
			{
				if(cntd->enable_cntd)
					fprintf(summary_report_fd, ";cntd_impact_cnt;cntd_impact_time");
				else
					fprintf(summary_report_fd, ";cntd_slack_impact_cnt;cntd_slack_impact_time");
			}
			fprintf(summary_report_fd, "\n");
		}

		printf("######################################################\n");
		printf("##################### COUNTDOWN ######################\n");
		printf("######################################################\n");
		ft_table_t *table = ft_create_table();
		ft_printf_ln(table,"%s|%.3f sec", "EXE time", exe_time);
		printf("%s\n", ft_to_string(table));
		ft_destroy_table(table);
		if(cntd->enable_report)
			fprintf(summary_report_fd, "%.3f", exe_time);
		printf("#################### GENERAL INFO ####################\n");
		table = ft_create_table();
		ft_printf_ln(table,"%s|%d","Number of MPI Ranks", world_size);
		ft_printf_ln(table,"%s|%d","Number of Nodes", local_master_size);
		ft_printf_ln(table,"%s|%u","Number of Sockets", num_sockets);
		ft_printf_ln(table,"%s|%u","Number of CPUs", num_cpus);
		if(cntd->enable_report)
			fprintf(summary_report_fd, ";%d;%d;%u;%u", 
				world_size, local_master_size, num_sockets, num_cpus);
#ifdef NVIDIA_GPU
		ft_printf_ln(table,"%s|%d","Number of GPUs", num_gpus);
		if(cntd->enable_report)
			fprintf(summary_report_fd, ";%d", num_gpus);
#endif
		printf("%s\n", ft_to_string(table));
    		ft_destroy_table(table);
		if(cntd->enable_power_monitor)
		{
			printf("##################### ENERGY #########################\n");
			table = ft_create_table();
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
			ft_printf_ln(table,"%s|%.0f J","PKG", global_energy_pkg);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.0f", global_energy_pkg);
#endif
#if defined(INTEL) || defined(POWER9)
			if (global_energy_dram != 0) {
				ft_printf_ln(table,"%s|%.0f J","DRAM",global_energy_dram);
				if(cntd->enable_report)
					fprintf(summary_report_fd, ";%.0f", global_energy_dram);
			}
#endif
#ifdef NVIDIA_GPU
			ft_printf_ln(table,"%s|%.0f J","GPU";global_energy_gpu);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.0f", global_energy_gpu);
#elif POWER9
			ft_printf_ln(table,"%s|%.0f J","GPU", global_energy_gpu_sys);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.0f", global_energy_gpu_sys);
#endif
#ifdef POWER9
			ft_printf_ln(table,"%s|%.0f J","SYS",global_energy_sys);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.0f", global_energy_sys);
#endif
			printf("%s\n", ft_to_string(table));
                        ft_destroy_table(table);
			printf("##################### AVG POWER ######################\n");
			table = ft_create_table();
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
			ft_printf_ln(table,"%s|%.2f W", "PKG",global_energy_pkg / exe_time);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.2f", global_energy_pkg / exe_time);
#endif
#if defined(INTEL) || defined(POWER9)
			if (global_energy_dram != 0) {
				ft_printf_ln(table,"%s|%.2f W", "DRAM",global_energy_dram / exe_time);
				if(cntd->enable_report)
					fprintf(summary_report_fd, ";%.2f", global_energy_dram / exe_time);
			}
#endif
#ifdef NVIDIA_GPU
			ft_printf_ln(table,"%s| %.2f W", "GPU",global_energy_gpu / exe_time);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.2f", global_energy_gpu / exe_time);
#elif POWER9
			ft_printf_ln(table,"%s|%.0f J", "GPU",global_energy_gpu_sys / exe_time);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.2f", global_energy_gpu_sys / exe_time);
#endif
#ifdef POWER9
			ft_printf_ln(table,"%s|%.2f W", "SYS",global_energy_sys / exe_time);
			if(cntd->enable_report) 
				fprintf(summary_report_fd, ";%.2f", global_energy_sys / exe_time);
#endif
			printf("%s\n", ft_to_string(table));
                        ft_destroy_table(table);
		}

		printf("################## PERFORMANCE INFO ##################\n");
		table = ft_create_table();
		if(mpi_net_data[SEND] < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI network - SENT",(double) mpi_net_data[SEND]);
		else if(mpi_net_data[SEND] < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_10);
		else if(mpi_net_data[SEND] < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_20);
		else if(mpi_net_data[SEND] < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_30);
		else if(mpi_net_data[SEND] < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_40);
		else if(mpi_net_data[SEND] < POW_2_60)
			ft_printf_ln(table,"%s|%.2f PByte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f PEyte", "MPI network - SENT",(double) mpi_net_data[SEND] / POW_2_60);

		if(mpi_net_data[RECV] < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI network - RECV",(double) mpi_net_data[RECV]);
		else if(mpi_net_data[RECV] < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_10);
		else if(mpi_net_data[RECV] < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_20);
		else if(mpi_net_data[RECV] < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_30);
		else if(mpi_net_data[RECV] < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_40);
		else if(mpi_net_data[RECV] < POW_2_60)
			ft_printf_ln(table,"%s| %.2f PByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f EByte", "MPI network - RECV",(double) mpi_net_data[RECV] / POW_2_60);

		if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]));
		else if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_10);
		else if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_20);
		else if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_30);
		else if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_40);
		else if((mpi_net_data[SEND] + mpi_net_data[RECV]) < POW_2_60)
			ft_printf_ln(table,"%s|%.2f PByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f EByte", "MPI network - TOT",(double) (mpi_net_data[SEND] + mpi_net_data[RECV]) / POW_2_60);

		if(cntd->enable_report) 
			fprintf(summary_report_fd, ";%lu;%lu", mpi_net_data[SEND], mpi_net_data[RECV]);

		if(mpi_file_data[WRITE] < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI file - WRITE",(double) mpi_file_data[WRITE]);
		else if(mpi_file_data[WRITE] < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_10);
		else if(mpi_file_data[WRITE] < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_20);
		else if(mpi_file_data[WRITE] < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_30);
		else if(mpi_file_data[WRITE] < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_40);
		else if(mpi_file_data[WRITE] < POW_2_60)
			ft_printf_ln(table,"%s|%.2f PByte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f PEyte", "MPI file - WRITE",(double) mpi_file_data[WRITE] / POW_2_60);

		if(mpi_file_data[READ] < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI file - READ",(double) mpi_file_data[READ]);
		else if(mpi_file_data[READ] < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_10);
		else if(mpi_file_data[READ] < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_20);
		else if(mpi_file_data[READ] < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_30);
		else if(mpi_file_data[READ] < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_40);
		else if(mpi_file_data[READ] < POW_2_60)
			ft_printf_ln(table,"%s|%.2f PByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f EByte", "MPI file - READ",(double) mpi_file_data[READ] / POW_2_60);

		if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_10)
			ft_printf_ln(table,"%s|%.0f Byte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]));
		else if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_20)
			ft_printf_ln(table,"%s|%.2f KByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_10);
		else if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_30)
			ft_printf_ln(table,"%s|%.2f MByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_20);
		else if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_40)
			ft_printf_ln(table,"%s|%.2f GByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_30);
		else if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_50)
			ft_printf_ln(table,"%s|%.2f TByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_40);
		else if((mpi_file_data[WRITE] + mpi_file_data[READ]) < POW_2_60)
			ft_printf_ln(table,"%s|%.2f PByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_50);
		else
			ft_printf_ln(table,"%s|%.2f EByte", "MPI file - TOT",(double) (mpi_file_data[WRITE] + mpi_file_data[READ]) / POW_2_60);

		if(cntd->enable_report) 
			fprintf(summary_report_fd, ";%lu;%lu", mpi_file_data[WRITE], mpi_file_data[READ]);

		if(max_mem_usage < POW_2_10)
			ft_printf_ln(table,"%s|%.0f KByte", "MAX Memory usage",(double) max_mem_usage);
		else if(max_mem_usage < POW_2_20)
			ft_printf_ln(table,"%s|%.2f MByte", "MAX Memory usage",(double) max_mem_usage / POW_2_10);
		else if(max_mem_usage < POW_2_30)
			ft_printf_ln(table,"%s|%.2f GByte", "MAX Memory usage",(double) max_mem_usage / POW_2_20);
		else if(max_mem_usage < POW_2_40)
			ft_printf_ln(table,"%s|%.2f TByte", "MAX Memory usage",(double) max_mem_usage / POW_2_30);
		else if(max_mem_usage < POW_2_50)
			ft_printf_ln(table,"%s|%.2f PByte", "MAX Memory usage",(double) max_mem_usage / POW_2_40);
		else if(max_mem_usage < POW_2_60)
			ft_printf_ln(table,"%s|%.2f EByte", "MAX Memory usage",(double) max_mem_usage / POW_2_50);
		if(cntd->enable_report)
			fprintf(summary_report_fd, ";%.0f", (double) max_mem_usage * 1024.0);

		ft_printf_ln(table,"%s|%.2f", "AVG IPC",avg_ipc);
		ft_printf_ln(table,"%s|%.0f MHz", "AVG CPU frequency",avg_freq);
		ft_printf_ln(table,"%s|%.2f", "AVG CPU load",avg_load);
		ft_printf_ln(table,"%s|%lu", "Cycles",global_cycles);
		ft_printf_ln(table,"%s|%lu", "Instructions retired",global_inst_ret);

        global_dp_flops_sec = (double)global_dp_flops/exe_time;
        global_sp_flops_sec = (double)global_sp_flops/exe_time;
        if (global_dp_flops_sec < POW_2_10) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP FLOPS/sec (64/128/256/512/TOT)",
                   (double)global_dp_flops_64/exe_time                            ,
                   (double)global_dp_flops_128/exe_time                           ,
                   (double)global_dp_flops_256/exe_time                           ,
                   (double)global_dp_flops_512/exe_time                           ,
                   (double)global_dp_flops_sec);
        } else if (global_dp_flops_sec < POW_2_20) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP KFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_10)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_10)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_10)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_10)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_10));
       } else if (global_dp_flops_sec < POW_2_30) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP MFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_20)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_20)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_20)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_20)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_20));
        } else if (global_dp_flops_sec < POW_2_40) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP GFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_30)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_30)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_30)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_30)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_30));
        } else if (global_dp_flops_sec < POW_2_50) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP TFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_40)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_40)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_40)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_40)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_40));
        } else if (global_dp_flops_sec < POW_2_60) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP PFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_50)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_50)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_50)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_50)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_50));
        } else {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","DP EFLOPS/sec (64/128/256/512/TOT)",
                   ((double)global_dp_flops_64/POW_2_60)/exe_time                  ,
                   ((double)global_dp_flops_128/POW_2_60)/exe_time                 ,
                   ((double)global_dp_flops_256/POW_2_60)/exe_time                 ,
                   ((double)global_dp_flops_512/POW_2_60)/exe_time                 ,
                   ((double)global_dp_flops_sec/POW_2_60));
        }
        if (global_sp_flops_sec < POW_2_10) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP FLOPS/sec (32/128/256/512/TOT)",
                   (double)global_sp_flops_32/exe_time                            ,
                   (double)global_sp_flops_128/exe_time                           ,
                   (double)global_sp_flops_256/exe_time                           ,
                   (double)global_sp_flops_512/exe_time                           ,
                   (double)global_sp_flops_sec);
        } else if (global_sp_flops_sec < POW_2_20) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP KFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_10)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_10)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_10)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_10)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_10));
       } else if (global_sp_flops_sec < POW_2_30) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP MFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_20)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_20)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_20)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_20)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_20));
        } else if (global_sp_flops_sec < POW_2_40) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP GFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_30)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_30)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_30)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_30)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_30));
        } else if (global_sp_flops_sec < POW_2_50) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP TFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_40)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_40)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_40)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_40)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_40));
        } else if (global_sp_flops_sec < POW_2_60) {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP PFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_50)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_50)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_50)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_50)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_50));
        } else {
		    ft_printf_ln(table,"%s|%.2f/%.2f/%.2f/%.2f/%.2f","SP EFLOPS/sec (32/128/256/512/TOT)",
                   ((double)global_sp_flops_32/POW_2_60)/exe_time                  ,
                   ((double)global_sp_flops_128/POW_2_60)/exe_time                 ,
                   ((double)global_sp_flops_256/POW_2_60)/exe_time                 ,
                   ((double)global_sp_flops_512/POW_2_60)/exe_time                 ,
                   ((double)global_sp_flops_sec/POW_2_60));
        }
        if (global_mem_data < POW_2_10) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in Bytes", (double)global_mem_data);
        } else if (global_mem_data < POW_2_20) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in KBytes",(double)global_mem_data/POW_2_10);
        } else if (global_mem_data < POW_2_30) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in MBytes",(double)global_mem_data/POW_2_20);
        } else if (global_mem_data < POW_2_40) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in GBytes",(double)global_mem_data/POW_2_30);
        } else if (global_mem_data < POW_2_50) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in TBytes",(double)global_mem_data/POW_2_40);
        } else if (global_mem_data < POW_2_60) {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in PBytes",(double)global_mem_data/POW_2_50);
        } else {
		    ft_printf_ln(table,"%s|%.3f","MEM Data Volume in EBytes",(double)global_mem_data/POW_2_60);
        }
        global_mem_bandwidth = ((double)global_mem_data)/exe_time;
        if (global_mem_bandwidth < POW_2_10) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in Bytes/s",global_mem_bandwidth);
        } else if (global_mem_bandwidth < POW_2_20) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in KByte/s",global_mem_bandwidth/POW_2_10);
        } else if (global_mem_bandwidth < POW_2_30) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in MBytes/s",global_mem_bandwidth/POW_2_20);
        } else if (global_mem_bandwidth < POW_2_40) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in GBytes/s",global_mem_bandwidth/POW_2_30);
        } else if (global_mem_bandwidth < POW_2_50) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in TBytes/s",global_mem_bandwidth/POW_2_40);
        } else if (global_mem_bandwidth < POW_2_60) {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in PBytes/s",global_mem_bandwidth/POW_2_50);
        } else {
		    ft_printf_ln(table,"%s|%.3f","MEM Bandwidth in EBytes/s",global_mem_bandwidth/POW_2_60);
        }
        ft_printf_ln(table,"%s|%.3f","DP Computational intensity in FLOPS/bytes",(double)global_dp_flops/(double)global_mem_data);
        ft_printf_ln(table,"%s|%.3f","SP Computational intensity in FLOPS/bytes",(double)global_sp_flops/(double)global_mem_data);
        ft_printf_ln(table,"%s|%.3f","DP Vector ratio"                                                                        ,
              (global_dp_flops > 0)                                                                             ?
              (double)(global_dp_flops_128 + global_dp_flops_256 + global_dp_flops_512)/(double)global_dp_flops :
              0.0);
        ft_printf_ln(table,"%s|%.3f","SP Vector ratio"                                                                        ,
              (global_sp_flops > 0)                                                                             ?
              (double)(global_sp_flops_128 + global_sp_flops_256 + global_sp_flops_512)/(double)global_sp_flops :
              0.0);
		if(cntd->enable_report) {

			const char* format = ";%.3f;%.0f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu;%lu;%lu;%lu;%lu;%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu(%lu/%lu);%lu";
			fprintf(summary_report_fd  		   ,
					format			   		   ,
					avg_ipc			   		   ,
					avg_freq		   		   ,
                    avg_load                   ,
					global_cycles	   		   ,
					global_inst_ret    		   ,
					global_dp_flops	   		   ,
					global_dp_flops_64 		   ,
					global_dp_flops_128		   ,
					global_dp_flops_256		   ,
					global_dp_flops_512		   ,
					global_dp_uops	   		   ,
				    global_time_en_dp_uops	   ,
				    global_time_run_dp_uops	   ,
					global_dp_uops_64  		   ,
				    global_time_en_dp_uops_64  ,
				    global_time_run_dp_uops_64 ,
					global_dp_uops_128 		   ,
				    global_time_en_dp_uops_128 ,
				    global_time_run_dp_uops_128,
					global_dp_uops_256 		   ,
				    global_time_en_dp_uops_256 ,
				    global_time_run_dp_uops_256,
					global_dp_uops_512 		   ,
				    global_time_en_dp_uops_512 ,
				    global_time_run_dp_uops_512,
					global_sp_flops	   		   ,
					global_sp_flops_32 		   ,
					global_sp_flops_128		   ,
					global_sp_flops_256		   ,
					global_sp_flops_512		   ,
					global_sp_uops	   		   ,
				    global_time_en_sp_uops	   ,
				    global_time_run_sp_uops	   ,
					global_sp_uops_32  		   ,
				    global_time_en_sp_uops_32  ,
				    global_time_run_sp_uops_32 ,
					global_sp_uops_128 		   ,
				    global_time_en_sp_uops_128 ,
				    global_time_run_sp_uops_128,
					global_sp_uops_256 		   ,
				    global_time_en_sp_uops_256 ,
				    global_time_run_sp_uops_256,
					global_sp_uops_512 		   ,
				    global_time_en_sp_uops_512 ,
				    global_time_run_sp_uops_512,
					global_mem		   		   ,
				    global_time_en_mem		   ,
				    global_time_run_mem		   ,
					global_mem_data);
		}
		for(i = 0; i < MAX_NUM_CUSTOM_PERF; i++)
		{
			if(cntd->perf_fd[0][i] > 0)
			{
				ft_printf_ln(table,"%s %d|%lu","Perf event", i, global_perf[i]);
				if(cntd->enable_report) 
					fprintf(summary_report_fd, ";%lu", global_perf[i]);
			}
		}
                printf("%s\n", ft_to_string(table));
                ft_destroy_table(table);

#ifdef NVIDIA_GPU
		double global_util = 0;
		double global_util_mem = 0;
		double global_temp = 0;
		double global_clock = 0;

		for(i = 0; i < local_master_size; i++)
		{
			for(j = 0; j < nodeinfo[i].num_gpus; j++)
			{
				global_util += ((double) gpuinfo[i].util[j]) / (double) rankinfo[i].num_sampling;
				global_util_mem += ((double) gpuinfo[i].util_mem[j]) / (double) rankinfo[i].num_sampling;
				global_temp += ((double) gpuinfo[i].temp[j]) / (double) rankinfo[i].num_sampling;
				global_clock += ((double) gpuinfo[i].clock[j]) / (double) rankinfo[i].num_sampling;
			}
		}
		global_util /= (double) num_gpus;
		global_util_mem /= (double) num_gpus;
		global_temp /= (double) num_gpus;
		global_clock /= (double) num_gpus;


		printf("##################### GPU REPORTING ##################\n");
		table = ft_create_table();
		ft_printf_ln(table,"%s|%.2f%%","AVG Utilization",global_util);
		ft_printf_ln(table,"%s|%.2f%%","AVG Mem Utilization",global_util_mem);
		ft_printf_ln(table,"%s|%.2f C","AVG Temperature",global_temp);
		ft_printf_ln(table,"%s|%.0f MHz","AVG Frequency",global_clock);
                printf("%s\n", ft_to_string(table));
                ft_destroy_table(table);

		if(cntd->enable_report)
			fprintf(summary_report_fd, ";%.2f;%.2f;%.2f;%.0f",
				global_util, global_util_mem, global_temp, global_clock);
#endif
		printf("##################### MPI TIMING #####################\n");
		table = ft_create_table();
		ft_printf_ln(table,"%s|%.3f sec (%.2f%%)","APP time",app_time,(app_time/(app_time+mpi_time))*100.0);
		ft_printf_ln(table,"%s|%.3f sec (%.2f%%)","MPI time",mpi_time,(mpi_time/(app_time+mpi_time))*100.0);
		ft_printf_ln(table,"%s|%.3f sec (100.00%%)","TOT time",app_time+mpi_time);
                printf("%s\n", ft_to_string(table));
                ft_destroy_table(table);

		if(cntd->enable_report)
			fprintf(summary_report_fd, ";%.9f;%.9f;%.9f",
				app_time, mpi_time, app_time+mpi_time);

		printf("##################### MPI REPORTING ##################\n");
		table = ft_create_table();
		for(j = 0; j < NUM_MPI_TYPE; j++)
		{
			if(mpi_type_cnt[j] > 0)
			{
				printf("%s: %lu - %.3f Sec (%.2f%%)", 
					mpi_type_str[j]+2, 
					mpi_type_cnt[j], 
					mpi_type_time[j], 
					(mpi_type_time[j]/mpi_time)*100.0);
				if(mpi_type_data[SEND][j])
				{
					if(mpi_type_data[SEND][j] < POW_2_10)
						printf(" - SEND %.0f Byte", (double) mpi_type_data[SEND][j]);
					else if(mpi_type_data[SEND][j] < POW_2_20)
						printf(" - SEND %.2f KByte", (double) mpi_type_data[SEND][j] / POW_2_10);
					else if(mpi_type_data[SEND][j] < POW_2_30)
						printf(" - SEND %.2f MByte", (double) mpi_type_data[SEND][j] / POW_2_20);
					else if(mpi_type_data[SEND][j] < POW_2_40)
						printf(" - SEND %.2f GByte", (double) mpi_type_data[SEND][j] / POW_2_30);
					else if(mpi_type_data[SEND][j] < POW_2_50)
						printf(" - SEND %.2f TByte", (double) mpi_type_data[SEND][j] / POW_2_40);
					else if(mpi_type_data[SEND][j] < POW_2_60)
						printf(" - SEND %.2f PByte", (double) mpi_type_data[SEND][j] / POW_2_50);
					else
						printf(" - SEND %.2f EByte", (double) mpi_type_data[SEND][j] / POW_2_60);
				}
				if(mpi_type_data[RECV][j])
				{
					if(mpi_type_data[RECV][j] < POW_2_10)
						printf(" - RECV %.0f Byte", (double) mpi_type_data[RECV][j]);
					else if(mpi_type_data[RECV][j] < POW_2_20)
						printf(" - RECV %.2f KByte", (double) mpi_type_data[RECV][j] / POW_2_10);
					else if(mpi_type_data[RECV][j] < POW_2_30)
						printf(" - RECV %.2f MByte", (double) mpi_type_data[RECV][j] / POW_2_20);
					else if(mpi_type_data[RECV][j] < POW_2_40)
						printf(" - RECV %.2f GByte", (double) mpi_type_data[RECV][j] / POW_2_30);
					else if(mpi_type_data[RECV][j] < POW_2_50)
						printf(" - RECV %.2f TByte", (double) mpi_type_data[RECV][j] / POW_2_40);
					else if(mpi_type_data[RECV][j] < POW_2_60)
						printf(" - RECV %.2f PByte", (double) mpi_type_data[RECV][j] / POW_2_50);
					else
						printf(" - RECV %.2f EByte", (double) mpi_type_data[RECV][j] / POW_2_60);
				}
				printf("\n");
			}
		}
                printf("%s\n", ft_to_string(table));
                ft_destroy_table(table);

		uint64_t cntd_impact_cnt = 0;
		double cntd_impact_time = 0;
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
					cntd_impact_time += cntd_mpi_type_time[j];
					printf("%s: %lu - %.3f Sec (%.2f%%)\n",
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
				cntd_impact_time,
				(cntd_impact_time/mpi_time)*100.0,
				(cntd_impact_time/(app_time+mpi_time))*100.0);
		}

		if(cntd->enable_report)
		{
			if(cntd->enable_cntd || cntd->enable_cntd_slack)
				fprintf(summary_report_fd, ";%lu;%.9f",
					cntd_impact_cnt, cntd_impact_time);

			fprintf(summary_report_fd, "\n");
			fclose(summary_report_fd);

			// print mpi report
			print_mpi_report(mpi_type_cnt, mpi_type_time, mpi_type_data[SEND], mpi_type_data[RECV]);

			// print eam report
			if(cntd->enable_cntd || cntd->enable_cntd_slack)
				print_eam_report(cntd_mpi_type_cnt, cntd_mpi_type_time);
		}

		printf("######################################################\n");

		// Print rank report
		if(cntd->enable_report)
		{
			print_rank(rankinfo, exe_time);
			print_rank_mpi(rankinfo, mpi_type_cnt);
		}
	}

	PMPI_Barrier(MPI_COMM_WORLD);
}

HIDDEN void init_timeseries_report()
{
	if(cntd->rank->local_rank == 0)
	{
		int i, j;
		char postfix[STRING_SIZE], filename[STRING_SIZE];

		get_rand_postfix(postfix, STRING_SIZE);
		snprintf(filename, STRING_SIZE, TMP_TIME_SERIES_FILE, cntd->tmp_dir, cntd->node.hostname, postfix);
		timeseries_fd = fopen(filename, "w");
		if(timeseries_fd == NULL)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed create time-series file '%s'!\n", 
				cntd->node.hostname, cntd->rank->world_rank, filename);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Time sample
		fprintf(timeseries_fd, "time-sample");

		if(cntd->enable_power_monitor)
		{
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
		}

		// GPU info
#ifdef NVIDIA_GPU
		// GPU utilization
		for(i = 0; i < cntd->gpu.num_gpus; i++)
			fprintf(timeseries_fd, ";util-gpu-%d", i);
		// GPU memory utilization
		for(i = 0; i < cntd->gpu.num_gpus; i++)
			fprintf(timeseries_fd, ";util-mem-gpu-%d", i);
		// GPU temperature
		for(i = 0; i < cntd->gpu.num_gpus; i++)
			fprintf(timeseries_fd, ";temp-gpu-%d", i);
		// GPU temperature
		for(i = 0; i < cntd->gpu.num_gpus; i++)
			fprintf(timeseries_fd, ";clock-gpu-%d", i);
#endif

		// MPI file write and read
		fprintf(timeseries_fd, ";mpi_file_write;mpi_file_read");

		// Application time
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-app_time", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// MPI time
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-mpi_time", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// MPI network send
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-mpi_net_send", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// MPI network recv
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-mpi_net_recv", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// Average Frequency
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-freq", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

        // Average Load
        for(i = 0; i < cntd->local_rank_size; i++)
            fprintf(timeseries_fd, ";rank-%d-cpu-%d-load",
                    cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// Average IPC
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-ipc", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// Average cycles
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-cycles", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		// Average Instructions retired
		for(i = 0; i < cntd->local_rank_size; i++)
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-inst_ret", 
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

		for(i = 0; i < cntd->local_rank_size; i++) {
			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_flops_tot",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_flops_64",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_flops_128",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_flops_256",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_flops_512",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_uops_tot_(te/tr)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_uops_64_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_uops_128_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_uops_256_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-dp_uops_512_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_flops_tot",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_flops_32",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_flops_128",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_flops_256",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_flops_512",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_uops_tot_(te/tr)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_uops_32_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_uops_128_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_uops_256_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-sp_uops_512_(te/tr/tm)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-mem_uops(te/tr)",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);

			fprintf(timeseries_fd, ";rank-%d-cpu-%d-mem_data",
				cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id);
		}

		// Linux perf
		for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
			for(i = 0; i < cntd->local_rank_size; i++)
				if(cntd->perf_fd[i][j] > 0)
					fprintf(timeseries_fd, ";rank-%d-cpu-%d-perf-event-%d", 
						cntd->local_ranks[i]->world_rank, cntd->local_ranks[i]->cpu_id, j);

		// End line
		fprintf(timeseries_fd, "\n");
	}
}

HIDDEN void finalize_timeseries_report()
{
	if(cntd->rank->local_rank == 0)
	{
		char oldname[STRING_SIZE], newname[STRING_SIZE], postfix[STRING_SIZE];

		fclose(timeseries_fd);

		get_rand_postfix(postfix, STRING_SIZE);
		snprintf(oldname, STRING_SIZE, TMP_TIME_SERIES_FILE, cntd->tmp_dir, cntd->node.hostname, postfix);
		snprintf(newname, STRING_SIZE, TIME_SERIES_FILE, cntd->log_dir, cntd->node.hostname);

		int rc = copyFile(oldname, newname);
		int rc2 = remove(oldname);
	}
}

#ifdef MOSQUITTO_ENABLED
HIDDEN void send_mosquitto_report(char* topic_ending,
								  int local_rank	,
								  double payload_value) {
	char payload[STRING_SIZE];
	int p_length; // \"payload\" length.
	char postfix[STRING_SIZE];
	int rc = 0;
	char topic[STRING_SIZE];
	time_t utc_secs;

	time(&utc_secs);

	get_rand_postfix(postfix,
					 STRING_SIZE);

    snprintf(topic				   ,
             STRING_SIZE		   ,
             MQTT_TOPIC 		   ,
			 postfix			   ,
			 cntd->rank->hostname  ,
			 cntd->local_ranks[local_rank]->cpu_id	  ,
			 cntd->local_ranks[local_rank]->world_rank,
			 cntd->local_ranks[local_rank]->local_rank,
			 topic_ending);
    snprintf(payload	  ,
             STRING_SIZE  ,
             MQTT_PAYLOAD ,
			 payload_value,
			 utc_secs);
	p_length = strlen(payload);

	rc = mosquitto_connect(mosq		,
						   MQTT_HOST,
						   MQTT_PORT,
						   MQTT_KEEPALIVE);

	mosquitto_publish(mosq	  ,
					  NULL	  ,
					  topic	  ,
					  p_length,
					  payload ,
					  MQTT_QOS,
					  MQTT_RETAIN);
}
#endif

HIDDEN void print_timeseries_report(
	double time_curr, double time_prev, 
	double energy_sys, double *energy_pkg, double *energy_dram, 
	double *energy_gpu_sys, double *energy_gpu,
	unsigned int *util_gpu, unsigned int *util_mem_gpu, 
	unsigned int *temp, unsigned int *clock)
{
	int i, j;
	double sample_duration = time_curr - time_prev;

	// Time sample
	fprintf(timeseries_fd, "%.3f", 
		time_curr - cntd->rank->exe_time[START]);

	if(cntd->enable_power_monitor)
	{
		// Energy
		for(i = 0; i < cntd->node.num_sockets; i++)
		{
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
			fprintf(timeseries_fd, ";%.2f", 
				energy_pkg[i]);
#endif
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
#if defined(INTEL) || defined(POWER9) || defined(THUNDERX2)
			fprintf(timeseries_fd, ";%.2f", 
				energy_pkg[i] / sample_duration);
#endif
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
	}

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

	// MPI file write
	uint64_t mpi_file[2] = {0};
	for(i = 0; i < cntd->local_rank_size; i++)
	{
		mpi_file[READ] += cntd->local_ranks[i]->mpi_file_data[READ][CURR];
		mpi_file[WRITE] += cntd->local_ranks[i]->mpi_file_data[WRITE][CURR];
	}
	fprintf(timeseries_fd, ";%lu;%lu", mpi_file[READ], mpi_file[WRITE]);

	// Application time
	for(i = 0; i < cntd->local_rank_size; i++) {
		fprintf(timeseries_fd, ";%.9f", cntd->local_ranks[i]->app_time[CURR]);
#ifdef MOSQUITTO_ENABLED
		send_mosquitto_report("app_time",
							  i			,
							  cntd->local_ranks[i]->app_time[CURR]);
#endif
	}

	// MPI time
	for(i = 0; i < cntd->local_rank_size; i++) {
		fprintf(timeseries_fd, ";%.9f", cntd->local_ranks[i]->mpi_time[CURR]);
#ifdef MOSQUITTO_ENABLED
		send_mosquitto_report("mpi_time",
							  i			,
							  cntd->local_ranks[i]->mpi_time[CURR]);
#endif
	}

	// MPI network send
	for(i = 0; i < cntd->local_rank_size; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->mpi_net_data[SEND][CURR]);

	// MPI network recv
	for(i = 0; i < cntd->local_rank_size; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->mpi_net_data[RECV][CURR]);

	// Average Frequency
	double curr_freq;
	for(i = 0; i < cntd->local_rank_size; i++)
	{
#ifdef INTEL
		fprintf(timeseries_fd, ";%.0f", 
			cntd->local_ranks[i]->perf[PERF_CYCLES_REF][CURR] > 0 ? ((double) cntd->local_ranks[i]->perf[PERF_CYCLES][CURR] / (double) cntd->local_ranks[i]->perf[PERF_CYCLES_REF][CURR]) * cntd->nom_freq_mhz : 0);
#ifdef MOSQUITTO_ENABLED
		curr_freq = (cntd->local_ranks[i]->perf[PERF_CYCLES_REF][CURR] > 0 ? ((double) cntd->local_ranks[i]->perf[PERF_CYCLES][CURR] / (double) cntd->local_ranks[i]->perf[PERF_CYCLES_REF][CURR]) * cntd->nom_freq_mhz : 0);
		send_mosquitto_report("avg_freq",
							  i			,
							  curr_freq);
#endif
#else
		fprintf(timeseries_fd, ";%.0f", 
			(double) cntd->local_ranks[i]->perf[PERF_CYCLES][CURR] / ((double) sample_duration * 1.0E6));
#endif
	}

    // Average Load
	for(i = 0; i < cntd->local_rank_size; i++)
        fprintf(timeseries_fd, ";%.2f",
                (double)cntd->local_ranks[i]->load[CURR]);

	// Average IPC
	for(i = 0; i < cntd->local_rank_size; i++)
	{
		fprintf(timeseries_fd, ";%.3f", 
			cntd->local_ranks[i]->perf[PERF_CYCLES][CURR] > 0 ? (double) cntd->local_ranks[i]->perf[PERF_INST_RET][CURR] / (double) cntd->local_ranks[i]->perf[PERF_CYCLES][CURR] : 0);
	}

	// Average cycles
	for(i = 0; i < cntd->local_rank_size; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf[PERF_CYCLES][CURR]);

	// Average Instructions retired
	for(i = 0; i < cntd->local_rank_size; i++)
		fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf[PERF_INST_RET][CURR]);

	for(i = 0; i < cntd->local_rank_size; i++) {
		uint64_t dp_uops_64 = cntd->local_ranks[i]->perf[PERF_SCALAR_DOUBLE][CURR];
		uint64_t dp_uops_128 = cntd->local_ranks[i]->perf[PERF_128_PACKED_DOUBLE][CURR];
		uint64_t dp_uops_256 = cntd->local_ranks[i]->perf[PERF_256_PACKED_DOUBLE][CURR];
		uint64_t dp_uops_512 = cntd->local_ranks[i]->perf[PERF_512_PACKED_DOUBLE][CURR];
		uint64_t dp_uops_tot = (dp_uops_64 + dp_uops_128 + dp_uops_256 + dp_uops_512);
		uint64_t dp_flops_64 = dp_uops_64;
		uint64_t dp_flops_128 = (dp_uops_128 * 2);
		uint64_t dp_flops_256 = (dp_uops_256 * 4);
		uint64_t dp_flops_512 = (dp_uops_512 * 8);
		uint64_t dp_flops_tot = (dp_flops_64 + dp_flops_128 + dp_flops_256 + dp_flops_512);
		uint64_t sp_uops_32 = cntd->local_ranks[i]->perf[PERF_SCALAR_SINGLE][CURR];
		uint64_t sp_uops_128 = cntd->local_ranks[i]->perf[PERF_128_PACKED_SINGLE][CURR];
		uint64_t sp_uops_256 = cntd->local_ranks[i]->perf[PERF_256_PACKED_SINGLE][CURR];
		uint64_t sp_uops_512 = cntd->local_ranks[i]->perf[PERF_512_PACKED_SINGLE][CURR];
		uint64_t sp_uops_tot = (sp_uops_32 + sp_uops_128 + sp_uops_256 + sp_uops_512);
		uint64_t sp_flops_32 = sp_uops_32;
		uint64_t sp_flops_128 = (sp_uops_128 * 4);
		uint64_t sp_flops_256 = (sp_uops_256 * 8);
		uint64_t sp_flops_512 = (sp_uops_512 * 16);
		uint64_t sp_flops_tot = (sp_flops_32 + sp_flops_128 + sp_flops_256 + sp_flops_512);
		uint64_t mem = 0;
		uint64_t time_en_mem = 0;
		uint64_t time_run_mem = 0;

#ifdef INTEL
		if (i == 0) {
			int j;
			int k;
			int t_k; // temporal index.
			for (j = 0; j < cntd->node.num_sockets; j++) {
				for (k = 0; k < MAX_NUM_MEM_CHANNELS_PER_SOCKET; k++) {
					t_k = PERF_CAS_COUNT_ALL + k + (j * MAX_NUM_MEM_CHANNELS_PER_SOCKET);
					mem += cntd->local_ranks[i]->perf[t_k][CURR];
					time_en_mem += cntd->local_ranks[i]->perf_te[t_k][CURR];
					time_run_mem += cntd->local_ranks[i]->perf_tr[t_k][CURR];
				}
			}
		}
#endif
		uint64_t mem_data = (mem * 64);
		uint64_t time_en_dp_uops_64 = cntd->local_ranks[i]->perf_te[PERF_SCALAR_DOUBLE][CURR];
		uint64_t time_en_dp_uops_128 = cntd->local_ranks[i]->perf_te[PERF_128_PACKED_DOUBLE][CURR];
		uint64_t time_en_dp_uops_256 = cntd->local_ranks[i]->perf_te[PERF_256_PACKED_DOUBLE][CURR];
		uint64_t time_en_dp_uops_512 = cntd->local_ranks[i]->perf_te[PERF_512_PACKED_DOUBLE][CURR];
		uint64_t time_en_dp_uops_tot = time_en_dp_uops_64  +
									   time_en_dp_uops_128 +
									   time_en_dp_uops_256 +
									   time_en_dp_uops_512;
		uint64_t time_en_sp_uops_32 = cntd->local_ranks[i]->perf_te[PERF_SCALAR_SINGLE][CURR];
		uint64_t time_en_sp_uops_128 = cntd->local_ranks[i]->perf_te[PERF_128_PACKED_SINGLE][CURR];
		uint64_t time_en_sp_uops_256 = cntd->local_ranks[i]->perf_te[PERF_256_PACKED_SINGLE][CURR];
		uint64_t time_en_sp_uops_512 = cntd->local_ranks[i]->perf_te[PERF_512_PACKED_SINGLE][CURR];
		uint64_t time_en_sp_uops_tot = time_en_sp_uops_32  +
									   time_en_sp_uops_128 +
									   time_en_sp_uops_256 +
									   time_en_sp_uops_512;
		double time_mul_dp_uops_64 = cntd->local_ranks[i]->perf_tm[PERF_SCALAR_DOUBLE][CURR];
		double time_mul_dp_uops_128 = cntd->local_ranks[i]->perf_tm[PERF_128_PACKED_DOUBLE][CURR];
		double time_mul_dp_uops_256 = cntd->local_ranks[i]->perf_tm[PERF_256_PACKED_DOUBLE][CURR];
		double time_mul_dp_uops_512 = cntd->local_ranks[i]->perf_tm[PERF_512_PACKED_DOUBLE][CURR];
		double time_mul_sp_uops_32 = cntd->local_ranks[i]->perf_tm[PERF_SCALAR_SINGLE][CURR];
		double time_mul_sp_uops_128 = cntd->local_ranks[i]->perf_tm[PERF_128_PACKED_SINGLE][CURR];
		double time_mul_sp_uops_256 = cntd->local_ranks[i]->perf_tm[PERF_256_PACKED_SINGLE][CURR];
		double time_mul_sp_uops_512 = cntd->local_ranks[i]->perf_tm[PERF_512_PACKED_SINGLE][CURR];
		uint64_t time_run_dp_uops_64 = cntd->local_ranks[i]->perf_tr[PERF_SCALAR_DOUBLE][CURR];
		uint64_t time_run_dp_uops_128 = cntd->local_ranks[i]->perf_tr[PERF_128_PACKED_DOUBLE][CURR];
		uint64_t time_run_dp_uops_256 = cntd->local_ranks[i]->perf_tr[PERF_256_PACKED_DOUBLE][CURR];
		uint64_t time_run_dp_uops_512 = cntd->local_ranks[i]->perf_tr[PERF_512_PACKED_DOUBLE][CURR];
		uint64_t time_run_dp_uops_tot = time_run_dp_uops_64  +
										time_run_dp_uops_128 +
										time_run_dp_uops_256 +
										time_run_dp_uops_512;
		uint64_t time_run_sp_uops_32 = cntd->local_ranks[i]->perf_tr[PERF_SCALAR_SINGLE][CURR];
		uint64_t time_run_sp_uops_128 = cntd->local_ranks[i]->perf_tr[PERF_128_PACKED_SINGLE][CURR];
		uint64_t time_run_sp_uops_256 = cntd->local_ranks[i]->perf_tr[PERF_256_PACKED_SINGLE][CURR];
		uint64_t time_run_sp_uops_512 = cntd->local_ranks[i]->perf_tr[PERF_512_PACKED_SINGLE][CURR];
		uint64_t time_run_sp_uops_tot = time_run_sp_uops_32  +
									   time_run_sp_uops_128 +
									   time_run_sp_uops_256 +
									   time_run_sp_uops_512;

		fprintf(timeseries_fd, ";%lu", dp_flops_tot);
		fprintf(timeseries_fd, ";%lu", dp_flops_64);
		fprintf(timeseries_fd, ";%lu", dp_flops_128);
		fprintf(timeseries_fd, ";%lu", dp_flops_256);
		fprintf(timeseries_fd, ";%lu", dp_flops_512);
		fprintf(timeseries_fd	   ,
				";%lu(%lu/%lu)"	   ,
				dp_uops_tot		   ,
				time_en_dp_uops_tot,
				time_run_dp_uops_tot);
		fprintf(timeseries_fd	   ,
				";%lu(%lu/%lu/%lf)",
				dp_uops_64		   ,
				time_en_dp_uops_64 ,
				time_run_dp_uops_64,
				time_mul_dp_uops_64);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				dp_uops_128		    ,
				time_en_dp_uops_128 ,
				time_run_dp_uops_128,
				time_mul_dp_uops_128);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				dp_uops_256		    ,
				time_en_dp_uops_256 ,
				time_run_dp_uops_256,
				time_mul_dp_uops_256);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				dp_uops_512		    ,
				time_en_dp_uops_512 ,
				time_run_dp_uops_512,
				time_mul_dp_uops_512);
		fprintf(timeseries_fd, ";%lu", sp_flops_tot);
		fprintf(timeseries_fd, ";%lu", sp_flops_32);
		fprintf(timeseries_fd, ";%lu", sp_flops_128);
		fprintf(timeseries_fd, ";%lu", sp_flops_256);
		fprintf(timeseries_fd, ";%lu", sp_flops_512);
		fprintf(timeseries_fd	   ,
				";%lu(%lu/%lu)"	   ,
				sp_uops_tot		   ,
				time_en_sp_uops_tot,
				time_run_sp_uops_tot);
		fprintf(timeseries_fd	   ,
				";%lu(%lu/%lu/%lf)",
				sp_uops_32		   ,
				time_en_sp_uops_32 ,
				time_run_sp_uops_32,
				time_mul_sp_uops_32);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				sp_uops_128		    ,
				time_en_sp_uops_128 ,
				time_run_sp_uops_128,
				time_mul_sp_uops_128);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				sp_uops_256		    ,
				time_en_sp_uops_256 ,
				time_run_sp_uops_256,
				time_mul_sp_uops_256);
		fprintf(timeseries_fd	    ,
				";%lu(%lu/%lu/%lf)" ,
				sp_uops_512		    ,
				time_en_sp_uops_512 ,
				time_run_sp_uops_512,
				time_mul_sp_uops_512);
		fprintf(timeseries_fd	   ,
				";%lu(%lu/%lu)"	   ,
				mem		    	   ,
				time_en_mem 	   ,
				time_run_mem);
		fprintf(timeseries_fd	   , ";%lu", mem_data);
	}

	// Linux perf
	for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
	{
		for(i = 0; i < cntd->local_rank_size; i++)
		{
			if(cntd->perf_fd[i][j] > 0)
				fprintf(timeseries_fd, ";%lu", cntd->local_ranks[i]->perf[j][CURR]);
		}
	}

	fprintf(timeseries_fd, "\n");
}
