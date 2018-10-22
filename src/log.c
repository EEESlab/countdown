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

typedef struct
{
	uint64_t idx;
	uint64_t mpi_type;
	uint64_t comm_idx;

	// Timing
	double epoch[2];

	// Network & File
	uint64_t net[2];
	uint64_t file[2];

	// HW perf counters
	uint64_t tsc[2];
	uint64_t fix[3][2];
	uint64_t pmc[8][2];
} CNTD_Print_Call_t;


// Open call file
void open_mpicall_file(int log_call)
{
	char log_file[STRING_SIZE];

	if(log_call)
	{
		sprintf(log_file, "%s/cntd_rank_%d.bin", cntd->log_dir, cntd->rank->my_rank);
		cntd->fd_mpicall = fopen(log_file, "wb");
		if(cntd->fd_mpicall == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed to create mpi call report file: %s\n", log_file);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
}

void close_mpicall_file()
{
	fclose(cntd->fd_mpicall);
}

void print_mpicall()
{
	int i;
	CNTD_Print_Call_t call;

	CNTD_Call_t *curr_call = &cntd->call[cntd->curr_call];
	CNTD_Call_t *prev_call = &cntd->call[cntd->prev_call];

	call.idx = curr_call->idx;
	call.mpi_type = curr_call->mpi_type;
	call.comm_idx = curr_call->cntd_comm->idx;

	call.epoch[0] = prev_call->epoch[START];
	call.epoch[1] = curr_call->epoch[START];
	call.epoch[2] = curr_call->epoch[END];

	call.net[SEND] = curr_call->net[SEND];
	call.net[RECV] = curr_call->net[RECV];

	call.file[READ] = curr_call->file[READ];
	call.file[WRITE] = curr_call->file[WRITE];

	call.tsc[APP] = diff_64(curr_call->tsc[START], prev_call->tsc[END]);
	call.tsc[MPI] = diff_64(curr_call->tsc[END], curr_call->tsc[START]);

	for(i = 0; i < 3; i++)
	{
		call.fix[i][APP] = diff_48(curr_call->fix[i][START], prev_call->fix[i][END]);
		call.fix[i][MPI] = diff_48(curr_call->fix[i][END], curr_call->fix[i][START]);
	}
	for(i = 0; i < 8; i++)
	{
		call.pmc[i][APP] = diff_48(curr_call->pmc[i][START], prev_call->pmc[i][END]);
		call.pmc[i][MPI] = diff_48(curr_call->pmc[i][END], curr_call->pmc[i][START]);
	}

	// Write
	fwrite(&call, sizeof(CNTD_Print_Call_t), 1, cntd->fd_mpicall);
}

void open_sampling_file(const char mode[])
{
	char log_file[STRING_SIZE];

	sprintf(log_file, "%s/cntd_sampling_%s.csv", cntd->log_dir, cntd->rank->hostname);
	cntd->fd_sampling = fopen(log_file, mode);
	if(cntd->fd_sampling == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create sampling report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

void close_sampling_file()
{
	fclose(cntd->fd_sampling);
}

void print_label_sampling_file()
{
	int i;

	fprintf(cntd->fd_sampling, "epoch_start;epoch_end;epoch_duration");
	for(i = 0; i < cntd->rank->cpus; i++)
	{
		if(cntd->adv_metrics)
			fprintf(cntd->fd_sampling, ";C%d_tsc;C%d_aperf;C%d_mperf;C%d_clk_curr;C%d_clk_ref", i, i, i, i, i);
		fprintf(cntd->fd_sampling,
			";C%d_inst_ret;C%d_load;C%d_freq;C%d_cpi;C%d_temp;C%d_net_send;C%d_net_recv;C%d_file_read;C%d_file_write"
			";C%d_app_time;C%d_mpi_time"
			";C%d_cstate_0;C%d_cstate_1;C%d_cstate_3;C%d_cstate_6;C%d_cstate_7"
			";C%d_pmc0;C%d_pmc1;C%d_pmc2;C%d_pmc3;C%d_pmc4;C%d_pmc5;C%d_pmc6;C%d_pmc7",
			i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
	}
	for(i = 0; i < cntd->rank->sockets; i++)
	{
		fprintf(cntd->fd_sampling, ";S%d_energy_pkg;S%d_energy_dram;S%d_power_pkg;S%d_power_dram;S%d_temp_pkg", i, i, i, i, i);
		if(cntd->adv_metrics)
			fprintf(cntd->fd_sampling, ";S%d_uclk", i);
		fprintf(cntd->fd_sampling, ";S%d_freq;S%d_cstate_0;S%d_cstate_2;S%d_cstate_3;S%d_cstate_6;S%d_cstate_7"
			";S%d_pcu0;S%d_pcu1;S%d_pcu2;S%d_pcu3", i, i, i, i, i, i, i, i, i, i);
	}
	fprintf(cntd->fd_sampling, "\n");
	fflush(cntd->fd_sampling);
}

void delete_sampling_file()
{
	char log_file[STRING_SIZE];

	sprintf(log_file, "%s/cntd_sampling_%s.csv", cntd->log_dir, cntd->rank->hostname);
	remove(log_file);
}

void print_batch(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket)
{
	int i, j, flag;
	CNTD_Rank_t *curr_rank, *prev_rank;
	CNTD_Rank_t empy_rank;
	double timing[2], time_duration = 0;
	uint64_t net[2], file[2];
	double energy_pkg, energy_dram;

	memset(&empy_rank, 0, sizeof(CNTD_Rank_t));

	for(i = 0; i < cntd->rank->cpus; i++)
    {
    	flag = FALSE;
    	memset(net, 0, sizeof(uint64_t)*2);
    	memset(file, 0, sizeof(uint64_t)*2);
    	memset(timing, 0, sizeof(double)*2);

    	if(i == 0)
    	{
    		time_duration = cntd->rank->epoch_sample[CURR] - cntd->rank->epoch_sample[PREV];
    		fprintf(cntd->fd_sampling, "%.9f;%.9f;%.9f", cntd->rank->epoch_sample[PREV], cntd->rank->epoch_sample[CURR], time_duration);
    	}

    	flag = FALSE;
    	for(j = 0; j < cntd->local_size; j++)
    	{
    		if(cntd->shmem_local_rank[j]->cpu_id == i)
    		{
    			curr_rank = cntd->shmem_local_rank[j];
    			prev_rank = &cntd->last_batch_rank[j];
    			flag = TRUE;
    		}
    	}

    	if(flag)
    	{
	    	net[SEND] = curr_rank->net[SEND] - prev_rank->net[SEND];
	    	net[RECV] = curr_rank->net[RECV] - prev_rank->net[RECV];

	    	file[READ] = curr_rank->file[READ] - prev_rank->file[READ];
	    	file[WRITE] = curr_rank->file[WRITE] - prev_rank->file[WRITE];

	    	timing[APP] = curr_rank->timing[APP] - prev_rank->timing[APP];
	    	timing[MPI] = curr_rank->timing[MPI] - prev_rank->timing[MPI];
	    	if(prev_rank->phase == APP)
	    		timing[APP] -= cntd->rank->epoch_sample[PREV] - prev_rank->epoch[END];
	    	else
	    		timing[MPI] -= cntd->rank->epoch_sample[PREV] - prev_rank->epoch[START];
	    	if(curr_rank->phase == APP)
	    		timing[APP] += epoch - curr_rank->epoch[END];
	    	else
	    		timing[MPI] += epoch - curr_rank->epoch[START];

	    	if(timing[APP] > DEFAULT_SAMPLING_TIME)
	    		timing[APP] = 1;
	    	else if(timing[APP] < 0)
	    		timing[APP] = 0;
	    	if(timing[MPI] > DEFAULT_SAMPLING_TIME)
	    		timing[MPI] = 1;
	    	else if(timing[MPI] < 0)
	    		timing[MPI] = 0;
	    }

    	if(cntd->adv_metrics)
    		fprintf(cntd->fd_sampling, ";%lu;%lu;%lu;%lu;%lu", cpu[i].tsc, cpu[i].aperf, cpu[i].mperf, cpu[i].clk_curr, cpu[i].clk_ref);
    	fprintf(cntd->fd_sampling, ";%lu;%.3f;%.0f;%.3f;%.2f;%lu;%lu;%lu;%lu;%.9f;%.9f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
    		cpu[i].inst_ret,
			get_load(cpu[i].clk_ref, cpu[i].tsc),
			get_core_freq(cpu[i].clk_curr, cpu[i].clk_ref, cntd->rank->nominal_freq),
			get_cpi(cpu[i].clk_ref, cpu[i].inst_ret),
			cpu[i].temp,
			net[SEND],
			net[RECV],
			file[READ],
			file[WRITE],
			timing[APP],
			timing[MPI],
			get_core_C0(cpu[i].clk_ref, cpu[i].tsc),
			get_core_C1(cpu[i].clk_ref, cpu[i].tsc, cpu[i].C3, cpu[i].C6, cpu[i].C7),
			get_core_cstate(cpu[i].C3, cpu[i].tsc),
			get_core_cstate(cpu[i].C6, cpu[i].tsc),
			get_core_cstate(cpu[i].C7, cpu[i].tsc),
			cpu[i].pmc0,
			cpu[i].pmc1,
			cpu[i].pmc2,
			cpu[i].pmc3,
			cpu[i].pmc4,
			cpu[i].pmc5,
			cpu[i].pmc6,
			cpu[i].pmc7);
    }

    for(i = 0; i < cntd->rank->sockets; i++)
    {
    	energy_pkg = get_energy(socket[i].energy_pkg, socket[i].rapl_joules);
    	energy_dram = get_energy(socket[i].energy_dram, socket[i].rapl_joules);
    	fprintf(cntd->fd_sampling,
    		";%.0f;%.0f;%.3f;%.3f;%.2f",
    		energy_pkg,
    		energy_dram,
    		energy_pkg / time_duration,
    		energy_dram / time_duration,
    		socket[i].temp);
    	if(cntd->adv_metrics)
    		fprintf(cntd->fd_sampling, ";%lu", socket[i].uclk);
    	fprintf(cntd->fd_sampling, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu",
    		get_uncore_freq(socket[i].uclk, time_duration, socket[i].C2, socket[i].C3, socket[i].C6, socket[i].C7, cpu[0].tsc),
    		get_pkg_C0(socket[i].C2, socket[i].C3, socket[i].C6, socket[i].C7, cpu[0].tsc),
    		get_pkg_cstate(socket[i].C2, cpu[0].tsc),
    		get_pkg_cstate(socket[i].C3, cpu[0].tsc),
    		get_pkg_cstate(socket[i].C6, cpu[0].tsc),
    		get_pkg_cstate(socket[i].C7, cpu[0].tsc),
    		socket[i].pcu0,
    		socket[i].pcu1,
    		socket[i].pcu2,
    		socket[i].pcu3);
    }
    fprintf(cntd->fd_sampling, "\n");
    fflush(cntd->fd_sampling);
}

static void print_cpus_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus)
{
	int i, j;
	double mpi_time_gt[cntd->rank->size];
	uint64_t num_mpi_calls[cntd->rank->size];
	char log_file[STRING_SIZE];

	sprintf(log_file, "%s/cntd_cpus.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary cpu report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "mpi_rank_id;cpu_id;socket_id;hostname;pid;num_mpi_calls;mpi_time_gt_%dus"
		";net_send;net_recv;file_read;file_write;temp", cntd->eam_timeout);
	if(cntd->adv_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;freq;cpi"
		";APP-time;APP-inst_ret;APP-load;APP-freq;APP-cpi"
		";MPI-time;MPI-inst_ret;MPI-load;MPI-freq;MPI-cpi"
		";cstate_0;cstate_1;cstate_3;cstate_6;cstate_7"
		";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7\n");

	memset(num_mpi_calls, 0, sizeof(uint64_t)*cntd->rank->size);
	memset(mpi_time_gt, 0, sizeof(double)*cntd->rank->size);
	for(i = 0; i < cntd->rank->size; i++)
	{
		mpi_time_gt[i] += ranks[i].timing[GT_TIMEOUT];
		for(j = 0; j < NUM_MPI_TYPE; j++)
			num_mpi_calls[i] += ranks[i].mpi_count[j][MPI_TYPE_TOT];
	}

	// Print data
	for(i = 0; i < cntd->rank->size; i++)
	{
		fprintf(fd, "%d;%d;%d;%s;%d;%lu;%.2f;%lu;%lu;%lu;%lu;%.2f",
					ranks[i].my_rank, 
					ranks[i].cpu_id,
					ranks[i].socket_id,
					ranks[i].hostname,
					ranks[i].process_id,
					num_mpi_calls[i],
					(mpi_time_gt[i] * 100.0) / ranks[i].epoch[END],
					ranks[i].net[SEND],
					ranks[i].net[RECV],
					ranks[i].file[READ],
					ranks[i].file[WRITE],
					cpus[i].temp / cpus[i].num_samples);
		if(cntd->adv_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
						";%lu;%lu;%lu"
						";%lu;%lu;%lu",
				cpus[i].tsc, cpus[i].aperf, cpus[i].mperf, cpus[i].clk_curr, cpus[i].clk_ref,
				ranks[i].tsc[APP], ranks[i].clk_curr[APP], ranks[i].clk_ref[APP],
				ranks[i].tsc[MPI], ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu\n",
			cpus[i].inst_ret,
			get_load(cpus[i].clk_ref, cpus[i].tsc),
			get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, ranks[i].nominal_freq),
			get_cpi(cpus[i].clk_ref, cpus[i].inst_ret),
			ranks[i].timing[APP],
			ranks[i].inst_ret[APP],
			get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]),
			get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], ranks[i].nominal_freq),
			get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]),
			ranks[i].timing[MPI],
			ranks[i].inst_ret[MPI],
			get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]),
			get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], ranks[i].nominal_freq),
			get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]),
			get_core_C0(cpus[i].clk_ref, cpus[i].tsc),
			get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7),
			get_core_cstate(cpus[i].C3, cpus[i].tsc),
			get_core_cstate(cpus[i].C6, cpus[i].tsc),
			get_core_cstate(cpus[i].C7, cpus[i].tsc),
			cpus[i].pmc0,
			cpus[i].pmc1,
			cpus[i].pmc2,
			cpus[i].pmc3,
			cpus[i].pmc4,
			cpus[i].pmc5,
			cpus[i].pmc6,
			cpus[i].pmc7);
	}
	fclose(fd);
}

static void print_sockets_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets)
{
	int i, j, z;
	char log_file[STRING_SIZE];
	char keys[cntd->rank->size][STRING_SIZE];
	char str_tmp[STRING_SIZE];
	int cpu_to_socket[cntd->rank->size];
	int socket_to_cpu[cntd->rank->size];
	int num_mpi_procs[cntd->rank->size];
	int flag, num_sockets;
	double exe_time;
	double energy_pkg;
	double energy_dram;
	double energy_tot;

	// Create file
	sprintf(log_file, "%s/cntd_sockets.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create socket report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "socket_id;hostname;num_mpi_procs;num_mpi_calls;mpi_time_gt_%dus"
				";net_send;net_recv;file_read;file_write;core_temp", cntd->eam_timeout);
	if(cntd->adv_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APPT-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot"
				";pkg_temp");
	if(cntd->adv_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7"
				";pcu0;pcu1;pcu2;pcu3\n");

	// Find sockets
	memset(num_mpi_procs, 0, sizeof(int)*cntd->rank->size);
	for(i = 0, num_sockets = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;
		sprintf(str_tmp, "%s-%d", ranks[i].hostname, ranks[i].socket_id);
		for(j = 0; j < num_sockets; j++)
		{
			if(strcmp(keys[j], str_tmp) == 0)
			{
				flag = TRUE;
				break;
			}
		}
		if(!flag)
		{
			strcpy(keys[num_sockets], str_tmp);
			cpu_to_socket[i] = num_sockets;
			socket_to_cpu[num_sockets] = i;
			num_mpi_procs[num_sockets]++;
			num_sockets++;
		}
		else
		{
			cpu_to_socket[i] = j;
			num_mpi_procs[j]++;
		}
	}
	
	// Inizialization
	int cpu_per_socket[num_sockets];
	uint64_t net[2][num_sockets];
	uint64_t file[2][num_sockets];
	double core_temp[num_sockets];
	uint64_t tsc[num_sockets][3];
	uint64_t aperf[num_sockets];
	uint64_t mperf[num_sockets];
	uint64_t clk_curr[num_sockets][3];
	uint64_t clk_ref[num_sockets][3];
	double timing[num_sockets][3];
	uint64_t inst_ret[num_sockets][3];
	double load[num_sockets][3];
	double core_freq[num_sockets][3];
	double cpi[num_sockets][3];
	double core_cstate_0[num_sockets];
	double core_cstate_1[num_sockets];
	double core_cstate_3[num_sockets];
	double core_cstate_6[num_sockets];
	double core_cstate_7[num_sockets];
	uint64_t pmc0[num_sockets];
	uint64_t pmc1[num_sockets];
	uint64_t pmc2[num_sockets];
	uint64_t pmc3[num_sockets];
	uint64_t pmc4[num_sockets];
	uint64_t pmc5[num_sockets];
	uint64_t pmc6[num_sockets];
	uint64_t pmc7[num_sockets];

	uint64_t num_mpi_calls[num_sockets];
	double mpi_time_gt[num_sockets];

	memset(cpu_per_socket, 0, sizeof(int)*num_sockets);
	memset(net, 0, sizeof(uint64_t)*num_sockets*2);
	memset(file, 0, sizeof(uint64_t)*num_sockets*2);
	memset(core_temp, 0, sizeof(double)*num_sockets);
	memset(timing, 0, sizeof(double)*num_sockets*3);
	memset(tsc, 0, sizeof(uint64_t)*num_sockets*3);
	memset(aperf, 0, sizeof(uint64_t)*num_sockets);
	memset(mperf, 0, sizeof(uint64_t)*num_sockets);
	memset(clk_curr, 0, sizeof(uint64_t)*num_sockets*3);
	memset(clk_ref, 0, sizeof(uint64_t)*num_sockets*3);
	memset(inst_ret, 0, sizeof(uint64_t)*num_sockets*3);
	memset(load, 0, sizeof(double)*num_sockets*3);
	memset(core_freq, 0, sizeof(double)*num_sockets*3);
	memset(cpi, 0, sizeof(double)*num_sockets*3);
	memset(core_cstate_0, 0, sizeof(double)*num_sockets);
	memset(core_cstate_1, 0, sizeof(double)*num_sockets);
	memset(core_cstate_3, 0, sizeof(double)*num_sockets);
	memset(core_cstate_6, 0, sizeof(double)*num_sockets);
	memset(core_cstate_7, 0, sizeof(double)*num_sockets);
	memset(pmc0, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc1, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc2, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc3, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc4, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc5, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc6, 0, sizeof(uint64_t)*num_sockets);
	memset(pmc7, 0, sizeof(uint64_t)*num_sockets);

	memset(num_mpi_calls, 0, sizeof(uint64_t)*num_sockets);
	memset(mpi_time_gt, 0, sizeof(double)*num_sockets);
	
	// Calculate socket data
	exe_time = cntd->epoch[END] - cntd->epoch[START];
	for(i = 0; i < cntd->rank->size; i++)
	{
		j = cpu_to_socket[i];

		cpu_per_socket[j]++;
		net[SEND][j] += ranks[i].net[SEND];
		net[RECV][j] += ranks[i].net[RECV];
		file[READ][j] += ranks[i].file[READ];
		file[WRITE][j] += ranks[i].file[WRITE];
		core_temp[j] += (cpus[i].temp / cpus[i].num_samples);

		aperf[j] += cpus[i].aperf;
		mperf[j] += cpus[i].mperf;

		timing[j][TOT] += ranks[i].timing[TOT];
		tsc[j][TOT] += cpus[i].tsc;
		clk_curr[j][TOT] += cpus[i].clk_curr;
		clk_ref[j][TOT] += cpus[i].clk_ref;
		inst_ret[j][TOT] += cpus[i].inst_ret;
		load[j][TOT] += get_load(cpus[i].clk_ref, cpus[i].tsc);
		core_freq[j][TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, ranks[i].nominal_freq);
		cpi[j][TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		timing[j][APP] += ranks[i].timing[APP];
		tsc[j][APP] += ranks[i].tsc[APP];
		clk_curr[j][APP] += ranks[i].clk_curr[APP];
		clk_ref[j][APP] += ranks[i].clk_ref[APP];
		inst_ret[j][APP] += ranks[i].inst_ret[APP];
		load[j][APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[j][APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], ranks[i].nominal_freq);
		cpi[j][APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);

		timing[j][MPI] += ranks[i].timing[MPI];
		tsc[j][MPI] += ranks[i].tsc[MPI];
		clk_curr[j][MPI] += ranks[i].clk_curr[MPI];
		clk_ref[j][MPI] += ranks[i].clk_ref[MPI];
		inst_ret[j][MPI] += ranks[i].inst_ret[MPI];
		load[j][MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[j][MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], ranks[i].nominal_freq);
		cpi[j][MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);

		core_cstate_0[j] += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1[j] += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3[j] += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6[j] += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7[j] += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		pmc0[j] += cpus[i].pmc0;
		pmc1[j] += cpus[i].pmc1;
		pmc2[j] += cpus[i].pmc2;
		pmc3[j] += cpus[i].pmc3;
		pmc4[j] += cpus[i].pmc4;
		pmc5[j] += cpus[i].pmc5;
		pmc6[j] += cpus[i].pmc6;
		pmc7[j] += cpus[i].pmc7;

		mpi_time_gt[j] += ranks[i].timing[GT_TIMEOUT];
		for(z = 0; z < NUM_MPI_TYPE; z++)
			num_mpi_calls[j] += ranks[i].mpi_count[z][MPI_TYPE_TOT];
	}
	for(j = 0; j < num_sockets; j++)
	{
		load[j][TOT] = load[j][TOT] / cpu_per_socket[j];
		core_freq[j][TOT]  = core_freq[j][TOT] / cpu_per_socket[j];
		cpi[j][TOT]  = cpi[j][TOT] / cpu_per_socket[j];

		load[j][APP] = load[j][APP] / cpu_per_socket[j];
		core_freq[j][APP]  = core_freq[j][APP] / cpu_per_socket[j];
		cpi[j][APP]  = cpi[j][APP] / cpu_per_socket[j];

		load[j][MPI] = load[j][MPI] / cpu_per_socket[j];
		core_freq[j][MPI]  = core_freq[j][MPI] / cpu_per_socket[j];
		cpi[j][MPI]  = cpi[j][MPI] / cpu_per_socket[j];

		core_temp[j] = core_temp[j] / cpu_per_socket[j];
		core_cstate_0[j] = core_cstate_0[j] / cpu_per_socket[j];
		core_cstate_1[j] = core_cstate_1[j] / cpu_per_socket[j];
		core_cstate_3[j] = core_cstate_3[j] / cpu_per_socket[j];
		core_cstate_6[j] = core_cstate_6[j] / cpu_per_socket[j];
		core_cstate_7[j] = core_cstate_7[j] / cpu_per_socket[j];
	}

	// Print data
	for(j = 0; j < num_sockets; j++)
	{
		i = socket_to_cpu[j];

		energy_pkg = get_energy(sockets[i].energy_pkg, sockets[i].rapl_joules);
		energy_dram = get_energy(sockets[i].energy_dram, sockets[i].rapl_joules);
		energy_tot = energy_pkg + energy_dram;

		fprintf(fd, "%d;%s;%d;%lu;%.2f;%lu;%lu;%lu;%lu;%.2f",
			sockets[i].socket_id,
			sockets[i].hostname,
			num_mpi_procs[j],
			num_mpi_calls[j],
			(mpi_time_gt[j] * 100.0) / timing[j][TOT],
			net[SEND][j],
			net[RECV][j],
			file[READ][j],
			file[WRITE][j],
			core_temp[j]);
		if(cntd->adv_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;"
				";%lu;%lu;%lu;"
				";%lu;%lu;%lu;",
				tsc[j][TOT], aperf[j], mperf[j], clk_curr[j][TOT], clk_ref[j][TOT],
				tsc[j][APP], clk_curr[j][APP], clk_ref[j][APP],
				tsc[j][MPI], clk_curr[j][MPI], clk_ref[j][MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu"
			";%.0f;%.0f;%.0f;%.2f;%.2f;%.2f;%.2f",
			inst_ret[j][TOT],
			load[j][TOT],
			core_freq[j][TOT],
			cpi[j][TOT],
			timing[j][APP],
			inst_ret[j][APP],
			load[j][APP],
			core_freq[j][APP],
			cpi[j][APP],
			timing[j][MPI],
			inst_ret[j][MPI],
			load[j][MPI],
			core_freq[j][MPI],
			cpi[j][MPI],
			core_cstate_0[j],
			core_cstate_1[j],
			core_cstate_3[j],
			core_cstate_6[j],
			core_cstate_7[j],
			pmc0[j],
			pmc1[j],
			pmc2[j],
			pmc3[j],
			pmc4[j],
			pmc5[j],
			pmc6[j],
			pmc7[j],
			energy_pkg,
			energy_dram,
			energy_tot,
			energy_pkg / exe_time,
			energy_dram / exe_time,
			energy_tot / exe_time,
			sockets[i].temp / sockets[i].num_samples);
		if(cntd->adv_metrics)
			fprintf(fd, ";%lu", sockets[i].uclk);
		fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu\n",
			get_uncore_freq(sockets[i].uclk, exe_time, sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc),
			get_pkg_C0(sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C2, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C3, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C6, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C7, cpus[i].tsc),
			sockets[i].pcu0,
			sockets[i].pcu1,
			sockets[i].pcu2,
			sockets[i].pcu3);
	}

	fclose(fd);
}

static void print_nodes_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets)
{
	int i, j, z;
	char log_file[STRING_SIZE];
	char str_tmp[STRING_SIZE];
	char keys[cntd->rank->size][STRING_SIZE];
	int cpu_to_node[cntd->rank->size];
	int node_to_cpu[cntd->rank->size];
	int socket_to_cpu[cntd->rank->size];
	int socket_to_node[cntd->rank->size];
	int num_mpi_procs[cntd->rank->size];
	int flag, num_sockets, num_nodes;

	// Create file
	sprintf(log_file, "%s/cntd_nodes.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary node report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "hostname;cpu_model;sockets;cores;ht_enable;cpus;nominal_freq;min_pstate;max_pstate;num_mpi_procs"
				";num_mpi_calls;mpi_time_gt_%dus;net_send;net_recv;file_read;file_write;core_temp", cntd->eam_timeout);
	if(cntd->adv_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
			";APP-tsc;APP-clk_curr;APP-clk_ref"
			";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APP-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot");
	if(cntd->adv_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_temp"
				";pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7"
				";pcu0;pcu1;pcu2;pcu3\n");

	double exe_time = cntd->epoch[END] - cntd->epoch[START];

	// Find cpu indexs
	memset(num_mpi_procs, 0, sizeof(int)*cntd->rank->size);
	for(i = 0, num_nodes = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;
		for(j = 0; j < num_nodes; j++)
		{
			if(strcmp(keys[j], ranks[i].hostname) == 0)
			{
				flag = TRUE;
				break;
			}
		}

		if(!flag)
		{
			strcpy(keys[num_nodes], ranks[i].hostname);
			cpu_to_node[i] = num_nodes;
			node_to_cpu[num_nodes] = i;
			num_mpi_procs[num_nodes]++;
			num_nodes++;
		}
		else
		{
			cpu_to_node[i] = j;
			num_mpi_procs[j]++;
		}
	}

	// Calculate cpu data
	uint64_t cpus_per_node[num_nodes];
	uint64_t net[2][num_nodes];
	uint64_t file[2][num_nodes];
	double core_temp[num_nodes];
	uint64_t tsc[num_nodes][3];
	uint64_t aperf[num_nodes];
	uint64_t mperf[num_nodes];
	double timing[num_nodes][3];
	uint64_t clk_curr[num_nodes][3];
	uint64_t clk_ref[num_nodes][3];
	uint64_t inst_ret[num_nodes][3];
	double load[num_nodes][3];
	double core_freq[num_nodes][3];
	double cpi[num_nodes][3];
	double core_cstate_0[num_nodes];
	double core_cstate_1[num_nodes];
	double core_cstate_3[num_nodes];
	double core_cstate_6[num_nodes];
	double core_cstate_7[num_nodes];
	uint64_t pmc0[num_nodes];
	uint64_t pmc1[num_nodes];
	uint64_t pmc2[num_nodes];
	uint64_t pmc3[num_nodes];
	uint64_t pmc4[num_nodes];
	uint64_t pmc5[num_nodes];
	uint64_t pmc6[num_nodes];
	uint64_t pmc7[num_nodes];

	uint64_t num_mpi_calls[num_nodes];
	double mpi_time_gt[num_nodes];

	memset(cpus_per_node, 0, sizeof(uint64_t)*num_nodes);
	memset(net, 0, sizeof(uint64_t)*num_nodes*2);
	memset(file, 0, sizeof(uint64_t)*num_nodes*2);
	memset(core_temp, 0, sizeof(double)*num_nodes);
	memset(tsc, 0, sizeof(uint64_t)*num_nodes*3);
	memset(aperf, 0, sizeof(uint64_t)*num_nodes);
	memset(mperf, 0, sizeof(uint64_t)*num_nodes);
	memset(clk_curr, 0, sizeof(uint64_t)*num_nodes*3);
	memset(clk_ref, 0, sizeof(uint64_t)*num_nodes*3);
	memset(timing, 0, sizeof(double)*num_nodes*3);
	memset(inst_ret, 0, sizeof(uint64_t)*num_nodes*3);
	memset(load, 0, sizeof(double)*num_nodes*3);
	memset(core_freq, 0, sizeof(double)*num_nodes*3);
	memset(cpi, 0, sizeof(double)*num_nodes*3);
	memset(core_cstate_0, 0, sizeof(double)*num_nodes);
	memset(core_cstate_1, 0, sizeof(double)*num_nodes);
	memset(core_cstate_3, 0, sizeof(double)*num_nodes);
	memset(core_cstate_6, 0, sizeof(double)*num_nodes);
	memset(core_cstate_7, 0, sizeof(double)*num_nodes);
	memset(pmc0, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc1, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc2, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc3, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc4, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc5, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc6, 0, sizeof(uint64_t)*num_nodes);
	memset(pmc7, 0, sizeof(uint64_t)*num_nodes);

	memset(num_mpi_calls, 0, sizeof(uint64_t)*num_nodes);
	memset(mpi_time_gt, 0, sizeof(double)*num_nodes);

	for(i = 0; i < cntd->rank->size; i++)
	{
		j = cpu_to_node[i];

		cpus_per_node[j]++;
		net[SEND][j] += ranks[i].net[SEND];
		net[RECV][j] += ranks[i].net[RECV];
		file[READ][j] += ranks[i].file[READ];
		file[WRITE][j] += ranks[i].file[WRITE];
		core_temp[j] += (cpus[i].temp / cpus[i].num_samples);

		aperf[j] += cpus[i].aperf;
		mperf[j] += cpus[i].mperf;

		timing[j][TOT] += ranks[i].timing[TOT];
		tsc[j][TOT] += cpus[i].tsc;
		clk_curr[j][TOT] += cpus[i].clk_curr;
		clk_ref[j][TOT] += cpus[i].clk_ref;
		inst_ret[j][TOT] += cpus[i].inst_ret;
		load[j][TOT] += get_load(cpus[i].clk_ref, cpus[i].tsc);
		core_freq[j][TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, ranks[i].nominal_freq);
		cpi[j][TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		timing[j][APP] += ranks[i].timing[APP];
		tsc[j][APP] += ranks[i].tsc[APP];
		clk_curr[j][APP] += ranks[i].clk_curr[APP];
		clk_ref[j][APP] += ranks[i].clk_ref[APP];
		inst_ret[j][APP] += ranks[i].inst_ret[APP];
		load[j][APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[j][APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], ranks[i].nominal_freq);
		cpi[j][APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);

		timing[j][MPI] += ranks[i].timing[MPI];
		tsc[j][MPI] += ranks[i].tsc[MPI];
		clk_curr[j][MPI] += ranks[i].clk_curr[MPI];
		clk_ref[j][MPI] += ranks[i].clk_ref[MPI];
		inst_ret[j][MPI] += ranks[i].inst_ret[MPI];
		load[j][MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[j][MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], ranks[i].nominal_freq);
		cpi[j][MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);
		
		core_cstate_0[j] += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1[j] += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3[j] += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6[j] += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7[j] += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		pmc0[j] += cpus[i].pmc0;
		pmc1[j] += cpus[i].pmc1;
		pmc2[j] += cpus[i].pmc2;
		pmc3[j] += cpus[i].pmc3;
		pmc4[j] += cpus[i].pmc4;
		pmc5[j] += cpus[i].pmc5;
		pmc6[j] += cpus[i].pmc6;
		pmc7[j] += cpus[i].pmc7;

		mpi_time_gt[j] += ranks[i].timing[GT_TIMEOUT];
		for(z = 0; z < NUM_MPI_TYPE; z++)
			num_mpi_calls[j] += ranks[i].mpi_count[z][MPI_TYPE_TOT];
	}
	for(j = 0; j < num_nodes; j++)
	{
		load[j][TOT] = load[j][TOT] / cpus_per_node[j];
		core_freq[j][TOT] = core_freq[j][TOT] / cpus_per_node[j];
		cpi[j][TOT] = cpi[j][TOT] / cpus_per_node[j];

		load[j][APP] = load[j][APP] / cpus_per_node[j];
		core_freq[j][APP] = core_freq[j][APP] / cpus_per_node[j];
		cpi[j][APP] = cpi[j][APP] / cpus_per_node[j];

		load[j][MPI] = load[j][MPI] / cpus_per_node[j];
		core_freq[j][MPI] = core_freq[j][MPI] / cpus_per_node[j];
		cpi[j][MPI] = cpi[j][MPI] / cpus_per_node[j];

		core_temp[j] = core_temp[j] / cpus_per_node[j];

		core_cstate_0[j] = core_cstate_0[j] / cpus_per_node[j];
		core_cstate_1[j] = core_cstate_1[j] / cpus_per_node[j];
		core_cstate_3[j] = core_cstate_3[j] / cpus_per_node[j];
		core_cstate_6[j] = core_cstate_6[j] / cpus_per_node[j];
		core_cstate_7[j] = core_cstate_7[j] / cpus_per_node[j];
	}

	// Find socket index
	for(i = 0, num_sockets = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;

		sprintf(str_tmp, "%s-%d", ranks[i].hostname, ranks[i].socket_id);
		for(j = 0; j < num_sockets; j++)
		{
			if(strcmp(keys[j], str_tmp) == 0)
			{
				flag = TRUE;
				break;
			}
		}

		if(!flag)
		{
			strcpy(keys[num_sockets], str_tmp);
			socket_to_cpu[num_sockets] = i;
			num_sockets++;
		}
		else
			socket_to_cpu[j] = i;
	}
	for(j = 0, num_nodes = 0; j < num_sockets; j++)
	{
		i = socket_to_cpu[j];
		flag = FALSE;

		for(z = 0; z < num_nodes; z++)
		{
			if(strcmp(keys[z], ranks[i].hostname) == 0)
			{
				flag = TRUE;
				break;
			}
		}

		if(!flag)
		{
			strcpy(keys[num_nodes], ranks[i].hostname);
			socket_to_node[j] = num_nodes;
			num_nodes++;
		}
		else
			socket_to_node[j] = z;
	}

	// Calculate socket data
	int sockets_per_node[num_nodes];
	double energy_pkg[num_nodes];
	double energy_dram[num_nodes];
	double pkg_temp[num_nodes];
	uint64_t uclk[num_nodes];
	double uncore_freq[num_nodes];
	double pkg_cstate_0[num_nodes];
	double pkg_cstate_2[num_nodes];
	double pkg_cstate_3[num_nodes];
	double pkg_cstate_6[num_nodes];
	double pkg_cstate_7[num_nodes];
	uint64_t pcu0[num_nodes];
	uint64_t pcu1[num_nodes];
	uint64_t pcu2[num_nodes];
	uint64_t pcu3[num_nodes];

	memset(sockets_per_node, 0, sizeof(int)*num_nodes);
	memset(energy_pkg, 0, sizeof(double)*num_nodes);
	memset(energy_dram, 0, sizeof(double)*num_nodes);
	memset(pkg_temp, 0, sizeof(double)*num_nodes);
	memset(uclk, 0, sizeof(uint64_t)*num_nodes);
	memset(uncore_freq, 0, sizeof(double)*num_nodes);
	memset(pkg_cstate_0, 0, sizeof(double)*num_nodes);
	memset(pkg_cstate_3, 0, sizeof(double)*num_nodes);
	memset(pkg_cstate_3, 0, sizeof(double)*num_nodes);
	memset(pkg_cstate_6, 0, sizeof(double)*num_nodes);
	memset(pkg_cstate_7, 0, sizeof(double)*num_nodes);
	memset(pcu0, 0, sizeof(uint64_t)*num_nodes);
	memset(pcu1, 0, sizeof(uint64_t)*num_nodes);
	memset(pcu2, 0, sizeof(uint64_t)*num_nodes);
	memset(pcu3, 0, sizeof(uint64_t)*num_nodes);

	for(j = 0; j < num_sockets; j++)
	{
		i = socket_to_cpu[j];
		z = socket_to_node[j];

		sockets_per_node[z]++;
		energy_pkg[z] += get_energy(sockets[i].energy_pkg, sockets[i].rapl_joules);
		energy_dram[z] += get_energy(sockets[i].energy_dram, sockets[i].rapl_joules);
		pkg_temp[z] += sockets[i].temp / sockets[i].num_samples;
		uclk[z] += sockets[i].uclk;
		uncore_freq[z] += get_uncore_freq(sockets[i].uclk, exe_time, sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc);
		pkg_cstate_0[z] += get_pkg_C0(sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc);
		pkg_cstate_3[z] += get_pkg_cstate(sockets[i].C2, cpus[i].tsc);
		pkg_cstate_3[z] += get_pkg_cstate(sockets[i].C3, cpus[i].tsc);
		pkg_cstate_6[z] += get_pkg_cstate(sockets[i].C6, cpus[i].tsc);
		pkg_cstate_7[z] += get_pkg_cstate(sockets[i].C7, cpus[i].tsc);
		pcu0[z] = sockets[i].pcu0;
		pcu1[z] = sockets[i].pcu1;
		pcu2[z] = sockets[i].pcu2;
		pcu3[z] = sockets[i].pcu3;
	}
	for(z = 0; z < num_nodes; z++)
	{
		pkg_temp[z] = pkg_temp[z] / sockets_per_node[z];
		uncore_freq[z] = uncore_freq[z] / sockets_per_node[z];
		pkg_cstate_0[z] = pkg_cstate_0[z] / sockets_per_node[z];
		pkg_cstate_3[z] = pkg_cstate_3[z] / sockets_per_node[z];
		pkg_cstate_3[z] = pkg_cstate_3[z] / sockets_per_node[z];
		pkg_cstate_6[z] = pkg_cstate_6[z] / sockets_per_node[z];
		pkg_cstate_7[z] = pkg_cstate_7[z] / sockets_per_node[z];
	}

	// Print data
	for(z = 0; z < num_nodes; z++)
	{
		i = node_to_cpu[z];

		fprintf(fd, "%s;%s;%d;%d;%d;%d;%d;%d;%d;%d;%lu;%.2f"
					";%lu;%lu;%lu;%lu;%.2f",
					ranks[i].hostname,
					ranks[i].cpu_model_name,
					ranks[i].sockets,
					ranks[i].cores,
					ranks[i].ht_enable,
					ranks[i].cpus,
					ranks[i].nominal_freq,
					ranks[i].min_pstate,
					ranks[i].max_pstate,
					num_mpi_procs[z],
					num_mpi_calls[z],
					(mpi_time_gt[z] * 100.0) / timing[z][TOT],
					net[SEND][z],
					net[RECV][z],
					file[READ][z],
					file[WRITE][z],
					core_temp[z]);
		if(cntd->adv_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
				";%lu;%lu;%lu"
				";%lu;%lu;%lu",
				tsc[z][TOT], aperf[z], mperf[z], clk_curr[z][TOT], clk_ref[z][TOT],
				tsc[z][APP], clk_curr[z][APP], clk_ref[z][APP],
				tsc[z][MPI], clk_curr[z][MPI], clk_ref[z][MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
					";%.9f;%lu;%.3f;%.0f;%.3f"
					";%.9f;%lu;%.3f;%.0f;%.3f"
					";%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu"
					";%.0f;%.0f;%.0f;%.2f;%.2f;%.2f",
					inst_ret[z][TOT],
					load[z][TOT],
					core_freq[z][TOT],
					cpi[z][TOT],
					timing[z][APP],
					inst_ret[z][APP],
					load[z][APP],
					core_freq[z][APP],
					cpi[z][APP],
					timing[z][MPI],
					inst_ret[z][MPI],
					load[z][MPI],
					core_freq[z][MPI],
					cpi[z][MPI],
					core_cstate_0[z],
					core_cstate_1[z],
					core_cstate_3[z],
					core_cstate_6[z],
					core_cstate_7[z],
					pmc0[z],
					pmc1[z],
					pmc2[z],
					pmc3[z],
					pmc4[z],
					pmc5[z],
					pmc6[z],
					pmc7[z],
					energy_pkg[z],
					energy_dram[z],
					energy_pkg[z] + energy_dram[z],
					energy_pkg[z] / exe_time,
					energy_dram[z] / exe_time,
					(energy_pkg[z] + energy_dram[z]) / exe_time);
		if(cntd->adv_metrics)
			fprintf(fd, ";%lu", uclk[z]);
		fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu\n",
					uncore_freq[z],
					pkg_temp[z],
					pkg_cstate_0[z],
					pkg_cstate_2[z],
					pkg_cstate_3[z],
					pkg_cstate_6[z],
					pkg_cstate_7[z],
					pcu0[z],
					pcu1[z],
					pcu2[z],
					pcu3[z]);
	}

	fclose(fd);
}

static void print_summary_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets)
{
	int i, j;
	char log_file[STRING_SIZE];
	char keys[cntd->rank->size][STRING_SIZE];
	char str_tmp[STRING_SIZE];
	int socket_to_cpu[cntd->rank->size];
	int flag, num_sockets;
	double exe_time, timing[3], mpi_time_gt;
	uint64_t num_mpi_calls, net[2], file[2];
	uint64_t tsc[3], aperf, mperf, clk_curr[3], clk_ref[3], inst_ret[3];
	double load[3], core_freq[3], cpi[3], core_temp;
	double core_cstate_0, core_cstate_1, core_cstate_3, core_cstate_6, core_cstate_7;
	uint64_t pmc0, pmc1, pmc2, pmc3, pmc4, pmc5, pmc6, pmc7;
	uint64_t uclk;
	double energy_pkg, energy_dram;
	double uncore_freq, pkg_temp;
	double pkg_cstate_0, pkg_cstate_2, pkg_cstate_3, pkg_cstate_6, pkg_cstate_7;
	uint64_t pcu0, pcu1, pcu2, pcu3;

	mpi_time_gt = 0;
	num_sockets = 0;
	num_mpi_calls = net[SEND] = net[RECV] = file[READ] = file[WRITE] = 0;
	aperf = mperf = 0;
	core_temp = 0;
	core_cstate_0 = core_cstate_1 = core_cstate_3 = core_cstate_6 = core_cstate_7 = 0;
	pmc0 = pmc1 = pmc2 = pmc3 = pmc4 = pmc5 = pmc6 = pmc7 = 0;
	energy_pkg = energy_dram = 0;
	uclk = 0;
	uncore_freq = pkg_temp = 0;
	pkg_cstate_0 = pkg_cstate_2 = pkg_cstate_3 = pkg_cstate_6 = pkg_cstate_7 = 0;
	pcu0 = pcu1 = pcu2 = pcu3 = 0;

	memset(tsc, 0, sizeof(uint64_t)*3);
	memset(clk_curr, 0, sizeof(uint64_t)*3);
	memset(clk_ref, 0, sizeof(uint64_t)*3);
	memset(timing, 0, sizeof(double)*3);
	memset(inst_ret, 0, sizeof(uint64_t)*3);
	memset(load, 0, sizeof(double)*3);
	memset(core_freq, 0, sizeof(double)*3);
	memset(cpi, 0, sizeof(double)*3);

	sprintf(log_file, "%s/cntd_summary.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "start_time;end_time;exe_time;num_mpi_procs;num_mpi_calls;mpi_time_gt_%dus"
				";net_send;net_recv;file_read;file_write;core_temp", cntd->eam_timeout);
	if(cntd->adv_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APP-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot");
	if(cntd->adv_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_temp"
				";pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7"
				";pcu0;pcu1;pcu2;pcu3\n");

	// Calculate core data
	exe_time = cntd->epoch[END] - cntd->epoch[START];
	for(i = 0; i < cntd->rank->size; i++)
	{
		timing[TOT] += ranks[i].timing[TOT];
		timing[APP] += ranks[i].timing[APP];
		timing[MPI] += ranks[i].timing[MPI];

		mpi_time_gt += ranks[i].timing[GT_TIMEOUT];
		for(j = 0; j < NUM_MPI_TYPE; j++)
			num_mpi_calls += ranks[i].mpi_count[j][MPI_TYPE_TOT];

		net[SEND] += ranks[i].net[SEND];
		net[RECV] += ranks[i].net[RECV];

		file[READ] += ranks[i].file[READ];
		file[WRITE] += ranks[i].file[WRITE];

		core_temp += (cpus[i].temp / cpus[i].num_samples);

		aperf += cpus[i].aperf;
		mperf += cpus[i].mperf;

		tsc[TOT] += cpus[i].tsc;
		clk_curr[TOT] += cpus[i].clk_curr;
		clk_ref[TOT] += cpus[i].clk_ref;
		inst_ret[TOT] += cpus[i].inst_ret;
		load[TOT] += get_load(cpus[i].clk_ref, cpus[i].tsc);
		core_freq[TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, ranks[i].nominal_freq);
		cpi[TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		tsc[APP] += ranks[i].tsc[APP];
		clk_curr[APP] += ranks[i].clk_curr[APP];
		clk_ref[APP] += ranks[i].clk_ref[APP];
		inst_ret[APP] += ranks[i].inst_ret[APP];
		load[APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], ranks[i].nominal_freq);
		cpi[APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);
		
		tsc[MPI] += ranks[i].tsc[MPI];
		clk_curr[MPI] += ranks[i].clk_curr[MPI];
		clk_ref[MPI] += ranks[i].clk_ref[MPI];
		inst_ret[MPI] += ranks[i].inst_ret[MPI];
		load[MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], ranks[i].nominal_freq);
		cpi[MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);

		core_cstate_0 += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1 += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3 += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6 += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7 += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		pmc0 += cpus[i].pmc0;
		pmc1 += cpus[i].pmc1;
		pmc2 += cpus[i].pmc2;
		pmc3 += cpus[i].pmc3;
		pmc4 += cpus[i].pmc4;
		pmc5 += cpus[i].pmc5;
		pmc6 += cpus[i].pmc6;
		pmc7 += cpus[i].pmc7;
	}

	core_temp = core_temp / (double) cntd->rank->size;

	load[TOT] = load[TOT] / (double) cntd->rank->size;
	core_freq[TOT] = core_freq[TOT] / (double) cntd->rank->size;
	cpi[TOT] = cpi[TOT] / (double) cntd->rank->size;

	load[APP] = load[APP] / (double) cntd->rank->size;
	core_freq[APP] = core_freq[APP] / (double) cntd->rank->size;
	cpi[APP] = cpi[APP] / (double) cntd->rank->size;

	load[MPI] = load[MPI] / (double) cntd->rank->size;
	core_freq[MPI] = core_freq[MPI] / (double) cntd->rank->size;
	cpi[MPI] = cpi[MPI] / (double) cntd->rank->size;

	core_cstate_0 = core_cstate_0 / (double) cntd->rank->size;
	core_cstate_1 = core_cstate_1 / (double) cntd->rank->size;
	core_cstate_3 = core_cstate_3 / (double) cntd->rank->size;
	core_cstate_6 = core_cstate_6 / (double) cntd->rank->size;
	core_cstate_7 = core_cstate_7 / (double) cntd->rank->size;

	// Find sockets
	for(i = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;
		sprintf(str_tmp, "%s-%d", ranks[i].hostname, ranks[i].socket_id);
		for(j = 0; j < num_sockets; j++)
		{
			if(strcmp(keys[j], str_tmp) == 0)
			{
				flag = TRUE;
				break;
			}
		}
		if(!flag)
		{
			strcpy(keys[num_sockets], str_tmp);
			socket_to_cpu[num_sockets] = i;
			num_sockets++;
		}
	}

	// Calculate socket data
	for(i = 0; i < num_sockets; i++)
	{
		j = socket_to_cpu[i];

		energy_pkg += get_energy(sockets[j].energy_pkg, sockets[j].rapl_joules);
		energy_dram += get_energy(sockets[j].energy_dram, sockets[j].rapl_joules);

		uclk += sockets[j].uclk;
		uncore_freq += get_uncore_freq(sockets[j].uclk, exe_time, sockets[j].C2, sockets[j].C3, sockets[j].C6, sockets[j].C7, cpus[j].tsc);
		pkg_temp += (sockets[j].temp / sockets[j].num_samples);

		pkg_cstate_0 += get_pkg_C0(sockets[j].C2, sockets[j].C3, sockets[j].C6, sockets[j].C7, cpus[j].tsc);
    	pkg_cstate_2 += get_pkg_cstate(sockets[j].C2, cpus[j].tsc);
    	pkg_cstate_3 += get_pkg_cstate(sockets[j].C3, cpus[j].tsc);
    	pkg_cstate_6 += get_pkg_cstate(sockets[j].C6, cpus[j].tsc);
    	pkg_cstate_7 += get_pkg_cstate(sockets[j].C7, cpus[j].tsc);

		pcu0 += sockets[j].pcu0;
		pcu1 += sockets[j].pcu1;
		pcu2 += sockets[j].pcu2;
		pcu3 += sockets[j].pcu3;
	}
	uncore_freq = uncore_freq / num_sockets;
	pkg_temp = pkg_temp / (double) num_sockets;

	pkg_cstate_0 = pkg_cstate_0 / (double) num_sockets;
	pkg_cstate_2 = pkg_cstate_2 / (double) num_sockets;
	pkg_cstate_3 = pkg_cstate_3 / (double) num_sockets;
	pkg_cstate_6 = pkg_cstate_6 / (double) num_sockets;
	pkg_cstate_7 = pkg_cstate_7 / (double) num_sockets;

	// print data
	fprintf(fd, "%.9f;%.9f;%.9f;%d;%lu;%.2f;%lu;%lu;%lu;%lu;%.2f",
		cntd->epoch[START], 
		cntd->epoch[END], 
		exe_time, 
		cntd->rank->size,
		num_mpi_calls,
		(mpi_time_gt * 100.0) / timing[TOT],
		net[SEND],
		net[RECV],
		file[READ],
		file[WRITE],
		core_temp);
	if(cntd->adv_metrics)
		fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
					";%lu;%lu;%lu"
					";%lu;%lu;%lu",
					tsc[TOT], aperf, mperf, clk_curr[TOT], clk_ref[TOT],
					tsc[APP], clk_curr[APP], clk_ref[APP],
					tsc[MPI], clk_curr[MPI], clk_ref[MPI]);
	fprintf(fd, ";%lu;%.3f;%.0f;%.3f;%.9f;%lu;%.3f;%.0f;%.3f;%.9f;%lu;%.3f;%.0f;%.3f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu"
		";%.0f;%.0f;%.0f;%.2f;%.2f;%.2f",
		inst_ret[TOT],
		load[TOT],
		core_freq[TOT],
		cpi[TOT],
		timing[APP],
		inst_ret[APP],
		load[APP],
		core_freq[APP],
		cpi[APP],
		timing[MPI],
		inst_ret[MPI],
		load[MPI],
		core_freq[MPI],
		cpi[MPI],
		core_cstate_0,
		core_cstate_1,
		core_cstate_3,
		core_cstate_6,
		core_cstate_7,
		pmc0,
		pmc1,
		pmc2,
		pmc3,
		pmc4,
		pmc5,
		pmc6,
		pmc7,
		energy_pkg,
		energy_dram,
		energy_pkg + energy_dram,
		energy_pkg / exe_time,
		energy_dram / exe_time,
		(energy_pkg + energy_dram) / exe_time);
	if(cntd->adv_metrics)
		fprintf(fd, ";%lu",	uclk);
	fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%lu;%lu;%lu;%lu\n",
		uncore_freq,
		pkg_temp,
		pkg_cstate_0,
		pkg_cstate_2,
		pkg_cstate_3,
		pkg_cstate_6,
		pkg_cstate_7,
		pcu0,
		pcu1,
		pcu2,
		pcu3);
	fclose(fd);
}

static void print_summary_mpinfo_file(CNTD_Rank_t *ranks)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];
	uint64_t count_proc, count, inst_ret, send_data, recv_data;
	double timing, load, freq, cpi;

	sprintf(log_file, "%s/cntd_summary_mpinfo.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary mpinfo report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Find called MPI functions
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		flag[i] = FALSE;
		for(j = 0; j < cntd->rank->size; j++)
		{
			if(ranks[j].mpi_count[i][MPI_TYPE_TOT] > 0)
			{
				flag[i] = TRUE;
				break;
			}
		}
	}

	// Print labels
	fprintf(fd, "mpi_func;count;send_data;recv_data;time;load;freq;cpi;inst_ret\n");

	// Print data
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		if(flag[i] == TRUE)
		{
			count_proc = count = inst_ret = send_data = recv_data = 0;
			timing = load = freq = cpi = 0;
			for(j = 0; j < cntd->rank->size; j++)
			{
				count_proc += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? 1 : 0;

				count += ranks[j].mpi_count[i][MPI_TYPE_TOT];
				send_data += ranks[j].mpi_send_data[i][MPI_TYPE_TOT];
				recv_data += ranks[j].mpi_recv_data[i][MPI_TYPE_TOT];
				timing += ranks[j].mpi_timing[i][MPI_TYPE_TOT];
				load += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_load(ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].mpi_tsc[i][MPI_TYPE_TOT]) : 0;
				freq += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_TOT], ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].nominal_freq) : 0;
				cpi += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT]) : 0;
				inst_ret += ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT];
			}

			if(count_proc > 1)
			{
				load /= count_proc;
				freq /= count_proc;
				cpi /= count_proc;
			}

			fprintf(fd, "%s;%lu;%lu;%lu;%.9f;%.2f;%.0f;%.3f;%lu\n", mpi_type_str[i]+5, count, send_data, recv_data, timing, load, freq, cpi, inst_ret);
		}
	}

	fclose(fd);
}

static void print_rank_mpinfo_file(CNTD_Rank_t *ranks)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];

	sprintf(log_file, "%s/cntd_rank_mpinfo.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create rank mpinfo report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Find called MPI functions
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		flag[i] = FALSE;
		for(j = 0; j < cntd->rank->size; j++)
		{
			if(ranks[j].mpi_count[i][MPI_TYPE_TOT] > 0)
			{
				flag[i] = TRUE;
				break;
			}
		}
	}

	// Print labels
	fprintf(fd, "mpi_rank_id");
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		if(flag[i] == TRUE)
		{
			fprintf(fd, ";%s(count);%s(send_data);%s(recv_data);%s(time);%s(load);%s(freq);%s(cpi);%s(inst_ret)",
				mpi_type_str[i]+5, 
				mpi_type_str[i]+5, 
				mpi_type_str[i]+5, 
				mpi_type_str[i]+5, 
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5);
		}
	}
	fprintf(fd, "\n");

	// Print data
	for(j = 0; j < cntd->rank->size; j++)
	{
		fprintf(fd, "%d", ranks[j].my_rank);
		for(i = 0; i < NUM_MPI_TYPE; i++)
		{
			if(flag[i] == TRUE)
			{
				fprintf(fd, ";%lu;%lu;%lu;%.9f;%.2f;%.0f;%.3f;%lu",
					ranks[j].mpi_count[i][MPI_TYPE_TOT],
					ranks[j].mpi_send_data[i][MPI_TYPE_TOT],
					ranks[j].mpi_recv_data[i][MPI_TYPE_TOT],
					ranks[j].mpi_timing[i][MPI_TYPE_TOT],
					ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_load(ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].mpi_tsc[i][MPI_TYPE_TOT]) : 0,
					ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_TOT], ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].nominal_freq) : 0,
					ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT]) : 0,
					ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT]);
			}
		}
		fprintf(fd, "\n");
	}

	fclose(fd);
}

static void print_eam_summary_mpinfo_file(CNTD_Rank_t *ranks)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];
	uint64_t count_proc, count, inst_ret, send_data, recv_data;
	double timing, load, freq, cpi;

	sprintf(log_file, "%s/cntd_summary_mpinfo_eam.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create eam summary mpinfo report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Find called MPI functions
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		flag[i] = FALSE;
		for(j = 0; j < cntd->rank->size; j++)
		{
			if(ranks[j].mpi_count[i][MPI_TYPE_TOT] > 0)
			{
				flag[i] = TRUE;
				break;
			}
		}
	}

	// Print labels
	fprintf(fd, "mpi_func;count;send_data;recv_data;time;load;freq;cpi;inst_ret\n");

	// Print data
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		if(flag[i] == TRUE)
		{
			count_proc = count = inst_ret = send_data = recv_data = 0;
			timing = load = freq = cpi = 0;
			for(j = 0; j < cntd->rank->size; j++)
			{
				count_proc += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? 1 : 0;

				count += ranks[j].mpi_count[i][MPI_TYPE_EAM];
				send_data += ranks[j].mpi_send_data[i][MPI_TYPE_EAM];
				recv_data += ranks[j].mpi_recv_data[i][MPI_TYPE_EAM];

				timing += ranks[j].mpi_timing[i][MPI_TYPE_EAM];
				load += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_load(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].mpi_tsc[i][MPI_TYPE_EAM]) : 0;
				freq += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_EAM], ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].nominal_freq) : 0;
				cpi += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].mpi_inst_ret[i][MPI_TYPE_EAM]) : 0;
				inst_ret += ranks[j].mpi_inst_ret[i][MPI_TYPE_EAM];
			}

			if(count_proc > 1)
			{
				load /= count_proc;
				freq /= count_proc;
				cpi /= count_proc;
			}
			
			if(count_proc > 0)
			{
				fprintf(fd, "%s;%lu;%lu;%lu;%.9f;%.2f;%.0f;%.3f;%lu\n",
					mpi_type_str[i]+5, count, send_data, recv_data, 
					timing, load, freq, cpi, inst_ret);
			}
		}
	}
	fprintf(fd, "\n");

	fclose(fd);
}

static void print_eam_rank_mpinfo_file(CNTD_Rank_t *ranks)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];

	sprintf(log_file, "%s/cntd_rank_mpinfo_eam.csv", cntd->log_dir);
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create eam rank mpinfo report file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Find called MPI functions
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		flag[i] = FALSE;
		for(j = 0; j < cntd->rank->size; j++)
		{
			if(ranks[j].mpi_count[i][MPI_TYPE_TOT] > 0)
			{
				flag[i] = TRUE;
				break;
			}
		}
	}

	// Print labels
	fprintf(fd, "mpi_rank_id");
	for(i = 0; i < NUM_MPI_TYPE; i++)
	{
		if(flag[i] == TRUE)
		{
			fprintf(fd, ";%s(count);%s(send_data);%s(recv_data)"
						";%s(time);%s(load);%s(freq);%s(cpi);%s(inst_ret)",
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5,
				mpi_type_str[i]+5);
		}
	}
	fprintf(fd, "\n");

	// Print data
	for(j = 0; j < cntd->rank->size; j++)
	{
		fprintf(fd, "%d", ranks[j].my_rank);
		for(i = 0; i < NUM_MPI_TYPE; i++)
		{
			if(flag[i] == TRUE)
			{
				fprintf(fd, ";%lu"
							";%lu;%lu"
							";%.9f;%.2f;%.0f;%.3f;%lu",
					ranks[j].mpi_count[i][MPI_TYPE_EAM],
					ranks[j].mpi_send_data[i][MPI_TYPE_EAM],
					ranks[j].mpi_recv_data[i][MPI_TYPE_EAM],
					ranks[j].mpi_timing[i][MPI_TYPE_EAM],
					ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_load(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].mpi_tsc[i][MPI_TYPE_EAM]) : 0,
					ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_EAM], ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].nominal_freq) : 0,
					ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ? get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], ranks[j].mpi_inst_ret[i][MPI_TYPE_EAM]) : 0,
					ranks[j].mpi_inst_ret[i][MPI_TYPE_EAM]);
			}
		}
		fprintf(fd, "\n");
	}

	fclose(fd);
}

void print_logs()
{
	CNTD_Rank_t ranks[cntd->rank->size];
	CNTD_Cpu_t cpus[cntd->rank->size];
	CNTD_Socket_t sockets[cntd->rank->size];

	MPI_Datatype rank_type = get_mpi_datatype_rank();
	MPI_Datatype cpu_type = get_mpi_datatype_cpu();
	MPI_Datatype socket_type = get_mpi_datatype_socket();

	PMPI_Gather(cntd->rank, 1, rank_type, ranks, 1, rank_type, ROOT_MPI, MPI_COMM_WORLD);
	PMPI_Gather(&(cntd->cpu[cntd->rank->cpu_id]), 1, cpu_type, cpus, 1, cpu_type, ROOT_MPI, MPI_COMM_WORLD);
	PMPI_Gather(&(cntd->socket[cntd->rank->socket_id]), 1, socket_type, sockets, 1, socket_type, ROOT_MPI, MPI_COMM_WORLD);

	if(cntd->rank->my_rank == ROOT_MPI)
	{
		print_cpus_file(ranks, cpus);
		print_sockets_file(ranks, cpus, sockets);
		print_nodes_file(ranks, cpus, sockets);
		print_summary_file(ranks, cpus, sockets);
		print_summary_mpinfo_file(ranks);
		print_rank_mpinfo_file(ranks);
		if(cntd->eamo == TRUE || cntd->eam == TRUE)
		{
			print_eam_summary_mpinfo_file(ranks);
			print_eam_rank_mpinfo_file(ranks);
		}
	}
	PMPI_Barrier(MPI_COMM_WORLD);
}
