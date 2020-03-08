/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *		* Redistributions of source code must retain the above copyright notice, this
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
 * Date: 24.08.2018
*/

#include "cntd.h"

// Open event file
void open_event_trace_file()
{
	char log_dir[STRING_SIZE], log_file[STRING_SIZE];

	if(snprintf(log_dir, sizeof(log_dir), "%s/cntd_event_trace", cntd->log_dir) < 0)
	{
		
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log directory!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Root create directory
	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		create_dir(log_dir);
	PMPI_Barrier(MPI_COMM_WORLD);

	// Local root create directory if local scratch of the node is used
	if(cntd->my_local_rank == CNTD_MPI_ROOT)
		create_dir(log_dir);
	PMPI_Barrier(MPI_COMM_WORLD);

	// All MPI processes create their own trace file
	if(snprintf(log_file, sizeof(log_file), "%s/rank_%d.bin", log_dir, cntd->rank->my_rank) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->fd_event_trace = fopen(log_file, "wb");
	if(cntd->fd_event_trace == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create mpi call trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

void close_event_trace_file()
{
	fclose(cntd->fd_event_trace);
}

void print_event()
{
	int i, net_addr[2];
	double epoch[3];
	uint64_t data[2], tsc[2], fix[3][2];

	CNTD_Call_t *prev_call = &cntd->call[cntd->prev_call];
	CNTD_Call_t *curr_call = &cntd->call[cntd->curr_call];

	fwrite(&curr_call->idx, sizeof(uint64_t), 1, cntd->fd_event_trace);
	fwrite(&curr_call->mpi_type, sizeof(int), 1, cntd->fd_event_trace);
	fwrite(&curr_call->cntd_comm->idx, sizeof(int), 1, cntd->fd_event_trace);
	fwrite(&curr_call->eam_policy, sizeof(int), 1, cntd->fd_event_trace);
	fwrite(&curr_call->flag_eam, sizeof(int), 1, cntd->fd_event_trace);
	fwrite(&curr_call->stacktrace, sizeof(int), 1, cntd->fd_event_trace);

	data[IN] = curr_call->net[RECV] + curr_call->file[READ];
	data[OUT] = curr_call->net[SEND] + curr_call->file[WRITE];
	fwrite(data, sizeof(uint64_t), 2, cntd->fd_event_trace);

	net_addr[SOURCE] = curr_call->net_addr[SOURCE];
	net_addr[DEST] = curr_call->net_addr[DEST];
	fwrite(net_addr, sizeof(int), 2, cntd->fd_event_trace);

	epoch[0] = prev_call->epoch[END] - cntd->epoch[START];
	epoch[1] = curr_call->epoch[START] - cntd->epoch[START];
	epoch[2] = curr_call->epoch[END] - cntd->epoch[START];
	fwrite(epoch, sizeof(double), 3, cntd->fd_event_trace);

	tsc[APP] = diff_64(curr_call->tsc[START], prev_call->tsc[END]);
	tsc[MPI] = diff_64(curr_call->tsc[END], curr_call->tsc[START]);
	fwrite(tsc, sizeof(uint64_t), 2, cntd->fd_event_trace);

	for(i = 0; i < 3; i++)
	{
		fix[i][APP] = diff_48(curr_call->fix[i][START], prev_call->fix[i][END]);
		fix[i][MPI] = diff_48(curr_call->fix[i][END], curr_call->fix[i][START]);
	}
	fwrite(fix, sizeof(uint64_t), 3*2, cntd->fd_event_trace);

	// if pmc log is enable
	if(cntd->pmc)
	{
		uint64_t pmc[8][2];
		for(i = 0; i < 8; i++)
		{
			pmc[i][APP] = diff_48(curr_call->pmc[i][START], prev_call->pmc[i][END]);
			pmc[i][MPI] = diff_48(curr_call->pmc[i][END], curr_call->pmc[i][START]);
		}
		fwrite(pmc, sizeof(uint64_t), 8*2, cntd->fd_event_trace);
	}
}

static void print_mpi_type()
{
	int i;
	char log_file[STRING_SIZE];
	FILE *fd;

	// Print mpi_type
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_mpi_type.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create mpi type file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "%s", mpi_type_str[0]+2);
	for(i = 1; i < NUM_MPI_TYPE; i++)
		fprintf(fd, ";%s", mpi_type_str[i]+2);
	fprintf(fd, "\n");

	// Print data
	fprintf(fd, "0");
	for(i = 1; i < NUM_MPI_TYPE; i++)
		fprintf(fd, ";%d", i);
	fprintf(fd, "\n");

	// Close file
	fclose(fd);
}

static void print_event_metadata_file()
{
	char log_file[STRING_SIZE];
	FILE *fd;

	// Print mpi metadata
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_event_trace/metadata.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create event file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd,
		"idx;type;comm_idx;eam_policy;flag_eam;stacktrace"
		";data_in;data_out"
		";net_source;net_dest"
		";time_start_app;time_start_mpi;time_end_mpi"
		";tsc_app;tsc_mpi"
		";inst_ret_app;inst_ret_mpi;clk_curr_app;clk_curr_mpi;clk_ref_app;clk_ref_mpi");
	if(cntd->pmc)
	{
		fprintf(fd,
			";pmc_1_app;pmc_1_mpi;pmc_2_app;pmc_2_mpi"
			";pmc_3_app;pmc_3_mpi;pmc_4_app;pmc_4_mpi"
			";pmc_5_app;pmc_5_mpi;pmc_6_app;pmc_6_mpi"
			";pmc_7_app;pmc_7_mpi;pmc_8_app;pmc_8_mpi");
	}
	fprintf(fd, "\n");

	// Print data
	fprintf(fd,
		"uint64;int32;int32;int32;int32;int32"
		";uint64;uint64"
		";int32;int32"
		";float64;float64;float64"
		";uint64;uint64"
		";uint64;uint64;uint64;uint64;uint64;uint64");
	if(cntd->pmc)
	{
		fprintf(fd,
			"float64;float64;float64;float64"
			";float64;float64;float64;float64"
			";float64;float64;float64;float64"
			";float64;float64;float64;float64");
	}
	fprintf(fd, "\n");

	// Close file
	fclose(fd);
}

// Open andante file
void open_task_trace_file()
{
	char log_dir[STRING_SIZE], log_file[STRING_SIZE];

	if(snprintf(log_dir, sizeof(log_dir), "%s/cntd_task_trace", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log directory!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Root create directory
	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		create_dir(log_dir);
	PMPI_Barrier(MPI_COMM_WORLD);

	// Local root create directory if local scratch of the node is used
	if(cntd->my_local_rank == CNTD_MPI_ROOT)
		create_dir(log_dir);
	PMPI_Barrier(MPI_COMM_WORLD);

	// All MPI processes create their own trace file
	if(snprintf(log_file, sizeof(log_file), "%s/rank_%d.bin", log_dir, cntd->rank->my_rank) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->fd_task_trace = fopen(log_file, "wb");
	if(cntd->fd_task_trace == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create andante trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

void close_task_trace_file()
{
	fclose(cntd->fd_task_trace);
}

void print_task(CNTD_Andante_t *andante_task, CNTD_Call_t *mpi_call)
{
	int i, net_addr[2];
	double epoch[4];
	uint64_t data[2], tsc[3], fix[3][3];
	int zero = 0;

	CNTD_Call_t *slack_call = &cntd->call[cntd->prev_call];

	fwrite(&mpi_call->idx, sizeof(uint64_t), 1, cntd->fd_task_trace);
	fwrite(&mpi_call->mpi_type, sizeof(int), 1, cntd->fd_task_trace);
	fwrite(&mpi_call->cntd_comm->idx, sizeof(int), 1, cntd->fd_task_trace);
	fwrite(&mpi_call->eam_policy, sizeof(int), 1, cntd->fd_task_trace);

	if(mpi_call->mpi_type == __MPI_BARRIER)
	{
		fwrite(&mpi_call->flag_eam, sizeof(int), 1, cntd->fd_task_trace);
		fwrite(&zero, sizeof(int), 1, cntd->fd_task_trace);
	}
	else
	{
		fwrite(&slack_call->flag_eam, sizeof(int), 1, cntd->fd_task_trace);
		fwrite(&mpi_call->flag_eam, sizeof(int), 1, cntd->fd_task_trace);
	}


	data[IN] = mpi_call->net[RECV] + mpi_call->file[READ];
	data[OUT] = mpi_call->net[SEND] + mpi_call->file[WRITE];
	fwrite(data, sizeof(uint64_t), 2, cntd->fd_task_trace);

	net_addr[SOURCE] = mpi_call->net_addr[SOURCE];
	net_addr[DEST] = mpi_call->net_addr[DEST];
	fwrite(net_addr, sizeof(int), 2, cntd->fd_task_trace);

	fwrite(&andante_task->task_id, sizeof(int), 1, cntd->fd_task_trace);

	epoch[0] = andante_task->epoch[START] - cntd->epoch[START];
	epoch[1] = andante_task->epoch[END] - cntd->epoch[START];
	if(mpi_call->mpi_type == __MPI_BARRIER)
	{
		epoch[2] = mpi_call->epoch[START] - cntd->epoch[START];
		epoch[3] = epoch[2];
	}
	else
	{
		epoch[2] = mpi_call->epoch[START] - cntd->epoch[START];
		epoch[3] = mpi_call->epoch[END] - cntd->epoch[START];
	}
	fwrite(epoch, sizeof(double), 4, cntd->fd_task_trace);

	tsc[0] = diff_48(andante_task->tsc[END], andante_task->tsc[START]);
	if(mpi_call->mpi_type == __MPI_BARRIER)
	{
		tsc[1] = diff_48(mpi_call->tsc[END], mpi_call->tsc[START]);
		tsc[2] = 0;
	}
	else
	{
		tsc[1] = diff_48(mpi_call->tsc[START], andante_task->tsc[END]);
		tsc[2] = diff_48(mpi_call->tsc[END], mpi_call->tsc[START]);
	}

	fwrite(tsc, sizeof(uint64_t), 3, cntd->fd_task_trace);

	for(i = 0; i < 3; i++)
	{
		fix[i][0] = diff_48(andante_task->fix[i][END], andante_task->fix[i][START]);
		if(mpi_call->mpi_type == __MPI_BARRIER)
		{
			fix[i][1] = diff_48(mpi_call->fix[i][END], mpi_call->fix[i][START]);
			fix[i][2] = 0;
		}
		else
		{
			fix[i][1] = diff_48(mpi_call->fix[i][START], andante_task->fix[i][END]);
			fix[i][2] = diff_48(mpi_call->fix[i][END], mpi_call->fix[i][START]);
		}
	}
	fwrite(fix, sizeof(uint64_t), 3*3, cntd->fd_task_trace);

	// if pmc log is enable
	if(cntd->pmc)
	{
		uint64_t pmc[8][3];
		for(i = 0; i < 8; i++)
		{
			pmc[i][0] = diff_48(andante_task->pmc[i][END], andante_task->pmc[i][START]);
			if(mpi_call->mpi_type == __MPI_BARRIER)
			{
				pmc[i][1] = diff_48(mpi_call->pmc[i][END], mpi_call->pmc[i][START]);
				pmc[i][2] = 0;
			}
			else
			{
				pmc[i][1] = diff_48(mpi_call->pmc[i][START], andante_task->pmc[i][END]);
				pmc[i][2] = diff_48(mpi_call->pmc[i][END], mpi_call->pmc[i][START]);
			}
		}
		fwrite(pmc, sizeof(uint64_t), 8*3, cntd->fd_task_trace);
	}
}

static void print_task_metadata_file()
{
	char log_file[STRING_SIZE];
	FILE *fd;

	// Print mpi metadata
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_task_trace/metadata.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create andante metada file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd,
		"idx;type;comm_idx;eam_policy;flag_eam_slack;flag_eam_mpi"
		";data_in;data_out"
		";net_source;net_dest"
		";task_id"
		";time_start_app;time_start_slack;time_start_mpi;time_end_mpi"
		";tsc_app;tsc_slack;tsc_mpi"
		";inst_ret_app;inst_ret_slack;inst_ret_mpi"
		";clk_curr_app;clk_curr_slack;clk_curr_mpi"
		";clk_ref_app;clk_ref_slack;clk_ref_mpi");
	if(cntd->pmc)
		fprintf(fd,
			";pmc_1_app;pmc_1_slack;pmc_1_mpi"
			";pmc_2_app;pmc_2_slack;pmc_2_mpi"
			";pmc_3_app;pmc_2_slack;pmc_2_mpi"
			";pmc_4_app;pmc_2_slack;pmc_2_mpi"
			";pmc_5_app;pmc_2_slack;pmc_2_mpi"
			";pmc_6_app;pmc_2_slack;pmc_2_mpi"
			";pmc_7_app;pmc_2_slack;pmc_2_mpi"
			";pmc_8_app;pmc_2_slack;pmc_2_mpi");
	fprintf(fd, "\n");

	// Print data
	fprintf(fd,
		"uint64;int32;int32;int32;int32;int32"
		";uint64;uint64"
		";int32;int32"
		";int32"
		";float64;float64;float64;float64"
		";uint64;uint64;uint64"
		";uint64;uint64;uint64"
		";uint64;uint64;uint64"
		";uint64;uint64;uint64");
	if(cntd->pmc)
		fprintf(fd,
			"float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64"
			";float64;float64;float64");
	fprintf(fd, "\n");

	// Close file
	fclose(fd);
}

void open_time_trace_file(const char mode[])
{
	char log_file[STRING_SIZE];

	// Create file
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_time_trace/%s.csv", cntd->log_dir, cntd->arch.hostname) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->fd_time_trace = fopen(log_file, mode);
	if(cntd->fd_time_trace == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create time trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

void close_time_trace_file()
{
	fclose(cntd->fd_time_trace);
}

void print_label_time_trace_file()
{
	int i;

	fprintf(cntd->fd_time_trace, "epoch_start;epoch_end;epoch_duration");
	for(i = 0; i < cntd->arch.cpus; i++)
	{
		if(cntd->debug_metrics)
			fprintf(cntd->fd_time_trace, ";C%d_tsc;C%d_aperf;C%d_mperf;C%d_clk_curr;C%d_clk_ref", i, i, i, i, i);
		fprintf(cntd->fd_time_trace,
			";C%d_inst_ret;C%d_load;C%d_freq;C%d_cpi;C%d_temp;C%d_net_send;C%d_net_recv;C%d_file_read;C%d_file_write"
			";C%d_app_time;C%d_mpi_time"
			";C%d_cstate_0;C%d_cstate_1;C%d_cstate_3;C%d_cstate_6;C%d_cstate_7",
			i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
		if(cntd->pmc)
			fprintf(cntd->fd_time_trace,
				";C%d_pmc0;C%d_pmc1;C%d_pmc2;C%d_pmc3;C%d_pmc4;C%d_pmc5;C%d_pmc6;C%d_pmc7",
				i, i, i, i, i, i, i, i);
	}
	for(i = 0; i < cntd->arch.sockets; i++)
	{
		fprintf(cntd->fd_time_trace, ";S%d_energy_pkg;S%d_energy_dram;S%d_power_pkg;S%d_power_dram;S%d_temp_pkg", i, i, i, i, i);
		if(cntd->debug_metrics)
			fprintf(cntd->fd_time_trace, ";S%d_uclk", i);
		fprintf(cntd->fd_time_trace, ";S%d_uncore_freq;S%d_cstate_0;S%d_cstate_2;S%d_cstate_3;S%d_cstate_6;S%d_cstate_7", i, i, i, i, i, i);
		if(cntd->pcu)
			fprintf(cntd->fd_time_trace, ";S%d_pcu0;S%d_pcu1;S%d_pcu2;S%d_pcu3", i, i, i, i);
	}
	fprintf(cntd->fd_time_trace, "\n");
	fflush(cntd->fd_time_trace);
}

void print_time_trace(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket)
{
	int i, j, flag;
	CNTD_Rank_t *curr_rank, *prev_rank;
	CNTD_Rank_t empy_rank;
	double timing[2], time_duration = 0;
	uint64_t net[2], file[2];
	double energy_pkg, energy_dram;

	memset(&empy_rank, 0, sizeof(CNTD_Rank_t));

	for(i = 0; i < cntd->arch.cpus; i++)
	{
		flag = FALSE;
		memset(net, 0, sizeof(uint64_t)*2);
		memset(file, 0, sizeof(uint64_t)*2);
		memset(timing, 0, sizeof(double)*2);

		if(i == 0)
		{
			time_duration = cntd->rank->epoch_sample[CURR] - cntd->rank->epoch_sample[PREV];
			fprintf(cntd->fd_time_trace, "%.9f;%.9f;%.9f", cntd->rank->epoch_sample[PREV], cntd->rank->epoch_sample[CURR], time_duration);
		}

		flag = FALSE;
		for(j = 0; j < cntd->local_size; j++)
		{
			if(cntd->shmem_local_rank[j]->cpu_id == i)
			{
				curr_rank = cntd->shmem_local_rank[j];
				prev_rank = &cntd->last_batch_ranks[j];
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

			// Sampling time compensation
			double time_app_mpi = timing[APP] + timing[MPI];
			if(timing[APP] >= time_duration)
			{
				timing[APP] = time_duration;
				timing[MPI] = 0;
			}
			else if(timing[MPI] >= time_duration)
			{
				timing[APP] = 0;
				timing[MPI] = time_duration;
			}
			else if(time_app_mpi != time_duration)
			{
				double diff = time_app_mpi - time_duration;
				if(diff > 0)
				{
					if(timing[APP] >= timing[MPI])
						timing[APP] -= diff;
					else
						timing[MPI] -= diff;
				}
				else
				{
					if(timing[APP] >= timing[MPI])
						timing[MPI] += fabs(diff);
					else
						timing[APP] += fabs(diff);
				}
			}
		}

		if(cntd->debug_metrics)
			fprintf(cntd->fd_time_trace, ";%lu;%lu;%lu;%lu;%lu", cpu[i].tsc, cpu[i].aperf, cpu[i].mperf, cpu[i].clk_curr, cpu[i].clk_ref);
		fprintf(cntd->fd_time_trace, ";%lu;%.3f;%.0f;%.3f;%.2f;%lu;%lu;%lu;%lu;%.9f;%.9f;%.2f;%.2f;%.2f;%.2f;%.2f",
			cpu[i].inst_ret,
		get_load(cpu[i].clk_ref, cpu[i].tsc),
		get_core_freq(cpu[i].clk_curr, cpu[i].clk_ref, cntd->arch.nominal_freq),
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
		get_core_cstate(cpu[i].C7, cpu[i].tsc));
	if(cntd->pmc)
		fprintf(cntd->fd_time_trace, ";%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
		cpu[i].pmc0,
		cpu[i].pmc1,
		cpu[i].pmc2,
		cpu[i].pmc3,
		cpu[i].pmc4,
		cpu[i].pmc5,
		cpu[i].pmc6,
		cpu[i].pmc7);
	}

	for(i = 0; i < cntd->arch.sockets; i++)
	{
		energy_pkg = get_energy(socket[i].energy_pkg, socket[i].rapl_joules);
		energy_dram = get_energy(socket[i].energy_dram, socket[i].rapl_joules);
		fprintf(cntd->fd_time_trace,
			";%.0f;%.0f;%.3f;%.3f;%.2f",
			energy_pkg,
			energy_dram,
			energy_pkg / time_duration,
			energy_dram / time_duration,
			socket[i].temp);
		if(cntd->debug_metrics)
			fprintf(cntd->fd_time_trace, ";%lu", socket[i].uclk);
		fprintf(cntd->fd_time_trace, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f",
			get_uncore_freq(socket[i].uclk, time_duration, socket[i].C2, socket[i].C3, socket[i].C6, socket[i].C7, cpu[0].tsc),
			get_pkg_C0(socket[i].C2, socket[i].C3, socket[i].C6, socket[i].C7, cpu[0].tsc),
			get_pkg_cstate(socket[i].C2, cpu[0].tsc),
			get_pkg_cstate(socket[i].C3, cpu[0].tsc),
			get_pkg_cstate(socket[i].C6, cpu[0].tsc),
			get_pkg_cstate(socket[i].C7, cpu[0].tsc));
		if(cntd->pcu)
			fprintf(cntd->fd_time_trace, ";%lu;%lu;%lu;%lu",
				socket[i].pcu0,
				socket[i].pcu1,
				socket[i].pcu2,
				socket[i].pcu3);
	}
	fprintf(cntd->fd_time_trace, "\n");
	fflush(cntd->fd_time_trace);
}

static void print_comms_file()
{
	int i, j;
	int r, ranks[cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size];
	char log_file[STRING_SIZE];
	int send_buf, recv_buf;
	MPI_Status status;

	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
	{
		// Root create file
		if(snprintf(log_file, sizeof(log_file), "%s/cntd_mpi_comms.csv", cntd->log_dir) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of theloge file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		FILE *fd = fopen(log_file, "w");
		if(fd == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed to create communicator trace file: %s\n", log_file);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Root writes labels
		fprintf(fd, "rank_id;comm_idx;size");
		for(i = 0; i < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; i++)
			fprintf(fd, ";rank_%d", i);
		fprintf(fd, "\n");

		// Root writes his data
		for(i = 0; i < cntd->comm_count; i++)
		{
			memset(ranks, -1, cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size * sizeof(int));
			for(j = 0; j < cntd->comm[i].cntd_group->size; j++)
			{
				r = cntd->comm[i].cntd_group->world_ranks[j];
				ranks[r] = j;
			}

			fprintf(fd, "%d;%d;%d", cntd->rank->my_rank, cntd->comm[i].idx, cntd->comm[i].cntd_group->size);
			for(j = 0; j < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; j++)
				fprintf(fd, ";%d", ranks[j]);
			fprintf(fd, "\n");
		}
		fclose(fd);
	}

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->rank->size > 1)
	{
		// Root coordinates the writing procedures of the other MPI processes
		if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		{
			for(i = 1; i < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; i++)
			{
				PMPI_Send(&send_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
				PMPI_Recv(&recv_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			}
		}
		else
		{
			// Each MPI write his data coordinated with root
			PMPI_Recv(&recv_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			// Open file in append mode
			if(snprintf(log_file, sizeof(log_file), "%s/cntd_mpi_comms.csv", cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed to create the name of thlogce file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			FILE *fd = fopen(log_file, "a");
			if(fd == NULL)
			{
				fprintf(stderr, "Error: <countdown> Failed to create communicator trace file: %s\n", log_file);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			// Calculate and write data
			for(i = 0; i < cntd->comm_count; i++)
			{
				memset(ranks, -1, cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size * sizeof(int));
				for(j = 0; j < cntd->comm[i].cntd_group->size; j++)
				{
					r = cntd->comm[i].cntd_group->world_ranks[j];
					ranks[r] = j;
				}

				fprintf(fd, "%d;%d;%d", cntd->rank->my_rank, cntd->comm[i].idx, cntd->comm[i].cntd_group->size);
				for(j = 0; j < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; j++)
					fprintf(fd, ";%d", ranks[j]);
				fprintf(fd, "\n");
			}
			fclose(fd);

			// Green semaphore to root -> next MPI process
			PMPI_Send(&send_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
}

static void print_summary_fermata_file()
{
	int i;
	char log_file[STRING_SIZE];
	int send_buf, recv_buf;
	double load[2], freq[2], cpi[2];
	MPI_Status status;

	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
	{
		// Root create file
		if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_fermata.csv", cntd->log_dir) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of theloge file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		FILE *fd = fopen(log_file, "w");
		if(fd == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed to create fermata summary file: %s\n", log_file);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Root writes labels
		fprintf(fd, "rank_id;mpi;stacktrace;count;eam_count;time;eam_time"
			";send_data;recv_data;EAM-load;EAM-freq;EAM-cpi;EAM-inst_ret;load;freq;cpi;inst_ret");
		if(cntd->pmc)
		{
			for(i = 0; i < 8; i++)
				fprintf(fd, ";EAM-pmc_%d", i);
			for(i = 0; i < 8; i++)
				fprintf(fd, ";pmc_%d", i);
		}
		fprintf(fd, "\n");

		// Root writes his data
		for(i = 0; i < cntd->fermata_count; i++)
		{
			load[NO_EAM] = get_load(cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->fermata_data[i].tsc_p[NO_EAM]);
			freq[NO_EAM] = get_core_freq(cntd->fermata_data[i].clk_curr_p[NO_EAM], cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->arch.nominal_freq);
			cpi[NO_EAM] = get_cpi(cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->fermata_data[i].inst_ret_p[NO_EAM]);
			if(cntd->fermata_data[i].low_freq_count > 0)
			{
				load[EAM] = get_load(cntd->fermata_data[i].clk_ref_p[EAM], cntd->fermata_data[i].tsc_p[EAM]);
				freq[EAM] = get_core_freq(cntd->fermata_data[i].clk_curr_p[EAM], cntd->fermata_data[i].clk_ref_p[EAM], cntd->arch.nominal_freq);
				cpi[EAM] = get_cpi(cntd->fermata_data[i].clk_ref_p[EAM], cntd->fermata_data[i].inst_ret_p[EAM]);
			}
			else
				load[EAM] = freq[EAM] = cpi[EAM] = 0;

			fprintf(fd, "%d;%s;%d;%lu;%lu;%.9f;%.9f;%lu;%lu;%.2f;%.0f;%.3f;%lu;%.2f;%.0f;%.3f;%lu",
				cntd->rank->my_rank,
				mpi_type_str[cntd->fermata_data[i].mpi_type]+2,
				cntd->fermata_data[i].stacktrace,
				cntd->fermata_data[i].count,
				cntd->fermata_data[i].low_freq_count,
				cntd->fermata_data[i].timing_p[EAM] + cntd->fermata_data[i].timing_p[NO_EAM],
				cntd->fermata_data[i].timing_p[EAM],
				cntd->fermata_data[i].net[SEND] + cntd->fermata_data[i].file[WRITE],
				cntd->fermata_data[i].net[RECV] + cntd->fermata_data[i].file[READ],
				load[EAM],
				freq[EAM],
				cpi[EAM],
				cntd->fermata_data[i].inst_ret_p[EAM],
				load[NO_EAM],
				freq[NO_EAM],
				cpi[NO_EAM],
				cntd->fermata_data[i].inst_ret_p[NO_EAM]);
			if(cntd->pmc)
			{
				for(i = 0; i < 8; i++)
					fprintf(fd, ";%lu", cntd->andante_data[i].pmc_p[EAM][i]);
				for(i = 0; i < 8; i++)
					fprintf(fd, ";%lu", cntd->andante_data[i].pmc_p[NO_EAM][i]);
			}
			fprintf(fd, "\n");
		}
		fclose(fd);
	}

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->rank->size > 1)
	{
		// Root coordinates the writing procedures of the other MPI processes
		if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		{
			for(i = 1; i < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; i++)
			{
				PMPI_Send(&send_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
				PMPI_Recv(&recv_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			}
		}
		else
		{
			// Each MPI write his data coordinated with root
			PMPI_Recv(&recv_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			// Open file in append mode
			if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_fermata.csv", cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed to create the name of thlogce file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			FILE *fd = fopen(log_file, "a");
			if(fd == NULL)
			{
				fprintf(stderr, "Error: <countdown> Failed to create fermata file: %s\n", log_file);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			// Calculate and write data
			for(i = 0; i < cntd->fermata_count; i++)
			{
				load[NO_EAM] = get_load(cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->fermata_data[i].tsc_p[NO_EAM]);
				freq[NO_EAM] = get_core_freq(cntd->fermata_data[i].clk_curr_p[NO_EAM], cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->arch.nominal_freq);
				cpi[NO_EAM] = get_cpi(cntd->fermata_data[i].clk_ref_p[NO_EAM], cntd->fermata_data[i].inst_ret_p[NO_EAM]);
				if(cntd->fermata_data[i].low_freq_count > 0)
				{
					load[EAM] = get_load(cntd->fermata_data[i].clk_ref_p[EAM], cntd->fermata_data[i].tsc_p[EAM]);
					freq[EAM] = get_core_freq(cntd->fermata_data[i].clk_curr_p[EAM], cntd->fermata_data[i].clk_ref_p[EAM], cntd->arch.nominal_freq);
					cpi[EAM] = get_cpi(cntd->fermata_data[i].clk_ref_p[EAM], cntd->fermata_data[i].inst_ret_p[EAM]);
				}
				else
					load[EAM] = freq[EAM] = cpi[EAM] = 0;

				fprintf(fd, "%d;%s;%d;%lu;%lu;%.9f;%.9f;%lu;%lu;%.2f;%.0f;%.3f;%lu;%.2f;%.0f;%.3f;%lu",
					cntd->rank->my_rank,
					mpi_type_str[cntd->fermata_data[i].mpi_type]+2,
					cntd->fermata_data[i].stacktrace,
					cntd->fermata_data[i].count,
					cntd->fermata_data[i].low_freq_count,
					cntd->fermata_data[i].timing_p[EAM] + cntd->fermata_data[i].timing_p[NO_EAM],
					cntd->fermata_data[i].timing_p[EAM],
					cntd->fermata_data[i].net[SEND] + cntd->fermata_data[i].file[WRITE],
					cntd->fermata_data[i].net[RECV] + cntd->fermata_data[i].file[READ],
					load[EAM],
					freq[EAM],
					cpi[EAM],
					cntd->fermata_data[i].inst_ret_p[EAM],
					load[NO_EAM],
					freq[NO_EAM],
					cpi[NO_EAM],
					cntd->fermata_data[i].inst_ret_p[NO_EAM]);
					if(cntd->pmc)
					{
						for(i = 0; i < 8; i++)
							fprintf(fd, ";%lu", cntd->andante_data[i].pmc_p[EAM][i]);
						for(i = 0; i < 8; i++)
							fprintf(fd, ";%lu", cntd->andante_data[i].pmc_p[NO_EAM][i]);
					}
				fprintf(fd, "\n");
			}
			fclose(fd);

			// Green semaphore to root -> next MPI process
			PMPI_Send(&send_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
}

static void print_summary_andante_file()
{
	int i, p;
	char log_file[STRING_SIZE];
	int send_buf, recv_buf;
	double freq[4];
	MPI_Status status;

	int turbo_pstate = cntd->arch.turbo_ratio[1];
	int min_pstate = cntd->arch.pstate[MIN];

	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
	{
		// Root create file
		if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_andante.csv", cntd->log_dir) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of theloge file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		FILE *fd = fopen(log_file, "w");
		if(fd == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed to create andante summary file: %s\n", log_file);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}

		// Root writes labels
		fprintf(fd, "rank_id;task_id;count;miss_count;mpi_type_start;mpi_type_end"
			";app_time;slack_time;miss_app_time;miss_slack_time"
			";app_inst-ret;slack_inst-ret;miss_app_inst-ret;miss_slack_inst-ret"
			";app_freq;slack_freq;miss_app_freq;miss_slack_freq");
		if(cntd->pmc)
			for(i = 0; i < 8; i++)
				fprintf(fd, ";app_pmc_%d;slack_pmc_%d;miss_app_pmc_%d;miss_slack_pmc_%d", i, i, i, i);
		for(p = turbo_pstate; p >= min_pstate; p--)
			fprintf(fd, ";pstate_%d", p);
		fprintf(fd, "\n");

		// Root writes his data
		for(i = 0; i < cntd->andante_count; i++)
		{
			freq[APP] = get_core_freq(cntd->andante_data[i].clk_curr_p[APP], cntd->andante_data[i].clk_ref_p[APP], cntd->arch.nominal_freq);
			freq[SLACK] = get_core_freq(cntd->andante_data[i].clk_curr_p[SLACK], cntd->andante_data[i].clk_ref_p[SLACK], cntd->arch.nominal_freq);
			if(cntd->andante_data[i].clk_curr_p[MISS_APP] != 0)
				freq[MISS_APP] = get_core_freq(cntd->andante_data[i].clk_curr_p[MISS_APP], cntd->andante_data[i].clk_ref_p[MISS_APP], cntd->arch.nominal_freq);
			else
				freq[MISS_APP] = 0;
			if(cntd->andante_data[i].clk_curr_p[MISS_SLACK] != 0)
				freq[MISS_SLACK] = get_core_freq(cntd->andante_data[i].clk_curr_p[MISS_SLACK], cntd->andante_data[i].clk_ref_p[MISS_SLACK], cntd->arch.nominal_freq);
			else
				freq[MISS_APP] = freq[MISS_SLACK] = 0;

			fprintf(fd, "%d;%d;%lu;%lu;%s;%s;%.9f;%.9f;%.9f;%.9f;%lu;%lu;%lu;%lu;%.0f;%.0f;%.0f;%.0f",
				cntd->rank->my_rank,
				cntd->andante_data[i].task_id[START],
				cntd->andante_data[i].count,
				cntd->andante_data[i].miss_count,
				mpi_type_str[cntd->andante_data[i].mpi_type[START]]+2,
				mpi_type_str[cntd->andante_data[i].mpi_type[END]]+2,
				cntd->andante_data[i].timing_p[APP],
				cntd->andante_data[i].timing_p[SLACK],
				cntd->andante_data[i].timing_p[MISS_APP],
				cntd->andante_data[i].timing_p[MISS_SLACK],
				cntd->andante_data[i].inst_ret_p[APP],
				cntd->andante_data[i].inst_ret_p[SLACK],
				cntd->andante_data[i].inst_ret_p[MISS_APP],
				cntd->andante_data[i].inst_ret_p[MISS_SLACK],
				freq[APP],
				freq[SLACK],
				freq[MISS_APP],
				freq[MISS_SLACK]);
			if(cntd->pmc)
			{
				for(i = 0; i < 8; i++)
				{
					fprintf(fd, ";%lu;%lu;%lu;%lu",
						cntd->andante_data[i].pmc_p[APP][i],
						cntd->andante_data[i].pmc_p[SLACK][i],
						cntd->andante_data[i].pmc_p[MISS_APP][i],
						cntd->andante_data[i].pmc_p[MISS_SLACK][i]);
				}
			}
			for(p = turbo_pstate; p >= min_pstate; p--)
				fprintf(fd, ";%lu", cntd->andante_data[i].sched_p[p]);
			fprintf(fd, "\n");
		}
		fclose(fd);
	}

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->rank->size > 1)
	{
		// Root coordinates the writing procedures of the other MPI processes
		if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		{
			for(i = 1; i < cntd->comm[CNTD_COMM_WORLD_IDX].cntd_group->size; i++)
			{
				PMPI_Send(&send_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
				PMPI_Recv(&recv_buf, 0, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			}
		}
		else
		{
			// Each MPI write his data coordinated with root
			PMPI_Recv(&recv_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			// Open file in append mode
			if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_andante.csv", cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed to create the name of thlogce file!\n");
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			FILE *fd = fopen(log_file, "a");
			if(fd == NULL)
			{
				fprintf(stderr, "Error: <countdown> Failed to create andante trace file: %s\n", log_file);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			// Calculate and write data
			for(i = 0; i < cntd->andante_count; i++)
			{
				freq[APP] = get_core_freq(cntd->andante_data[i].clk_curr_p[APP], cntd->andante_data[i].clk_ref_p[APP], cntd->arch.nominal_freq);
				freq[SLACK] = get_core_freq(cntd->andante_data[i].clk_curr_p[SLACK], cntd->andante_data[i].clk_ref_p[SLACK], cntd->arch.nominal_freq);
				if(cntd->andante_data[i].clk_curr_p[MISS_APP] != 0)
					freq[MISS_APP] = get_core_freq(cntd->andante_data[i].clk_curr_p[MISS_APP], cntd->andante_data[i].clk_ref_p[MISS_APP], cntd->arch.nominal_freq);
				else
					freq[MISS_APP] = 0;
				if(cntd->andante_data[i].clk_curr_p[MISS_SLACK] != 0)
					freq[MISS_SLACK] = get_core_freq(cntd->andante_data[i].clk_curr_p[MISS_SLACK], cntd->andante_data[i].clk_ref_p[MISS_SLACK], cntd->arch.nominal_freq);
				else
					freq[MISS_APP] = freq[MISS_SLACK] = 0;

				fprintf(fd, "%d;%d;%lu;%lu;%s;%s;%.9f;%.9f;%.9f;%.9f;%lu;%lu;%lu;%lu;%.0f;%.0f;%.0f;%.0f",
					cntd->rank->my_rank,
					cntd->andante_data[i].task_id[START],
					cntd->andante_data[i].count,
					cntd->andante_data[i].miss_count,
					mpi_type_str[cntd->andante_data[i].mpi_type[START]]+2,
					mpi_type_str[cntd->andante_data[i].mpi_type[END]]+2,
					cntd->andante_data[i].timing_p[APP],
					cntd->andante_data[i].timing_p[SLACK],
					cntd->andante_data[i].timing_p[MISS_APP],
					cntd->andante_data[i].timing_p[MISS_SLACK],
					cntd->andante_data[i].inst_ret_p[APP],
					cntd->andante_data[i].inst_ret_p[SLACK],
					cntd->andante_data[i].inst_ret_p[MISS_APP],
					cntd->andante_data[i].inst_ret_p[MISS_SLACK],
					freq[APP],
					freq[SLACK],
					freq[MISS_APP],
					freq[MISS_SLACK]);
				if(cntd->pmc)
				{
					for(i = 0; i < 8; i++)
					{
						fprintf(fd, ";%lu;%lu;%lu;%lu",
							cntd->andante_data[i].pmc_p[APP][i],
							cntd->andante_data[i].pmc_p[SLACK][i],
							cntd->andante_data[i].pmc_p[MISS_APP][i],
							cntd->andante_data[i].pmc_p[MISS_SLACK][i]);
					}
				}
				for(p = turbo_pstate; p >= min_pstate; p--)
					fprintf(fd, ";%lu", cntd->andante_data[i].sched_p[p]);
				fprintf(fd, "\n");
			}

			fclose(fd);

			// Green semaphore to root -> next MPI process
			PMPI_Send(&send_buf, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
}

static void print_cpus_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Arch_t *archs)
{
	int i, j;
	double cntd_impact[cntd->rank->size];
	uint64_t num_mpi_calls[cntd->rank->size];
	char log_file[STRING_SIZE];

	if(snprintf(log_file, sizeof(log_file), "%s/cntd_cpus.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary cpu trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "rank_id;cpu_id;socket_id;hostname;pid;turbo_pstate"
		";num_mpi_calls;cntd_impact;net_send;net_recv;file_read;file_write;temp");
	if(cntd->debug_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;freq;cpi"
		";APP-time;APP-inst_ret;APP-load;APP-freq;APP-cpi"
		";MPI-time;MPI-inst_ret;MPI-load;MPI-freq;MPI-cpi"
		";cstate_0;cstate_1;cstate_3;cstate_6;cstate_7");
	if(cntd->pmc)
		fprintf(fd, ";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7");
	fprintf(fd, "\n");

	memset(num_mpi_calls, 0, sizeof(uint64_t)*cntd->rank->size);
	memset(cntd_impact, 0, sizeof(double)*cntd->rank->size);
	for(i = 0; i < cntd->rank->size; i++)
	{
		cntd_impact[i] += ranks[i].timing[GT_TIMEOUT];
		for(j = 0; j < NUM_MPI_TYPE; j++)
			num_mpi_calls[i] += ranks[i].mpi_count[j][MPI_TYPE_TOT];
	}

	// Print data
	for(i = 0; i < cntd->rank->size; i++)
	{
		fprintf(fd, "%d;%d;%d;%s;%d;%d;%lu;%.2f;%lu;%lu;%lu;%lu;%.2f",
					ranks[i].my_rank,
					ranks[i].cpu_id,
					ranks[i].socket_id,
					archs[i].hostname,
					ranks[i].process_id,
					archs[i].turbo_ratio[archs[i].num_procs_local_socket],
					num_mpi_calls[i],
					(cntd_impact[i] * 100.0) / ranks[i].epoch[END],
					ranks[i].net[SEND],
					ranks[i].net[RECV],
					ranks[i].file[READ],
					ranks[i].file[WRITE],
					cpus[i].temp / cpus[i].num_samples);
		if(cntd->debug_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
						";%lu;%lu;%lu"
						";%lu;%lu;%lu",
				cpus[i].tsc, cpus[i].aperf, cpus[i].mperf, cpus[i].clk_curr, cpus[i].clk_ref,
				ranks[i].tsc[APP], ranks[i].clk_curr[APP], ranks[i].clk_ref[APP],
				ranks[i].tsc[MPI], ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.2f;%.2f;%.2f;%.2f;%.2f",
			cpus[i].inst_ret,
			get_load(cpus[i].clk_ref, cpus[i].tsc),
			get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, archs[i].nominal_freq),
			get_cpi(cpus[i].clk_ref, cpus[i].inst_ret),
			ranks[i].timing[APP],
			ranks[i].inst_ret[APP],
			get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]),
			get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], archs[i].nominal_freq),
			get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]),
			ranks[i].timing[MPI],
			ranks[i].inst_ret[MPI],
			get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]),
			get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], archs[i].nominal_freq),
			get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]),
			get_core_C0(cpus[i].clk_ref, cpus[i].tsc),
			get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7),
			get_core_cstate(cpus[i].C3, cpus[i].tsc),
			get_core_cstate(cpus[i].C6, cpus[i].tsc),
			get_core_cstate(cpus[i].C7, cpus[i].tsc));

		if(cntd->pmc)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
				cpus[i].pmc0,
				cpus[i].pmc1,
				cpus[i].pmc2,
				cpus[i].pmc3,
				cpus[i].pmc4,
				cpus[i].pmc5,
				cpus[i].pmc6,
				cpus[i].pmc7);
		fprintf(fd, "\n");
	}
	fclose(fd);
}

static void print_sockets_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets, CNTD_Arch_t *archs)
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
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_sockets.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create socket trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "socket_id;hostname;num_mpi_procs;num_mpi_calls;cntd_impact"
				";net_send;net_recv;file_read;file_write;core_temp");
	if(cntd->debug_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APPT-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot"
				";pkg_temp");
	if(cntd->pmc)
		fprintf(fd, ";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7");
	if(cntd->debug_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7");
	if(cntd->pcu)
		fprintf(fd, ";pcu0;pcu1;pcu2;pcu3");
	fprintf(fd, "\n");

	// Find sockets
	memset(num_mpi_procs, 0, sizeof(int)*cntd->rank->size);
	for(i = 0, num_sockets = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;
		if(snprintf(str_tmp, sizeof(str_tmp), "%s-%d", archs[i].hostname, ranks[i].socket_id) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of the string tmp!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
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
	double cntd_impact[num_sockets];

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
	if(cntd->pmc)
	{
		memset(pmc0, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc1, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc2, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc3, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc4, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc5, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc6, 0, sizeof(uint64_t)*num_sockets);
		memset(pmc7, 0, sizeof(uint64_t)*num_sockets);
	}

	memset(num_mpi_calls, 0, sizeof(uint64_t)*num_sockets);
	memset(cntd_impact, 0, sizeof(double)*num_sockets);

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
		core_freq[j][TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, archs[i].nominal_freq);
		cpi[j][TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		timing[j][APP] += ranks[i].timing[APP];
		tsc[j][APP] += ranks[i].tsc[APP];
		clk_curr[j][APP] += ranks[i].clk_curr[APP];
		clk_ref[j][APP] += ranks[i].clk_ref[APP];
		inst_ret[j][APP] += ranks[i].inst_ret[APP];
		load[j][APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[j][APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], archs[i].nominal_freq);
		cpi[j][APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);

		timing[j][MPI] += ranks[i].timing[MPI];
		tsc[j][MPI] += ranks[i].tsc[MPI];
		clk_curr[j][MPI] += ranks[i].clk_curr[MPI];
		clk_ref[j][MPI] += ranks[i].clk_ref[MPI];
		inst_ret[j][MPI] += ranks[i].inst_ret[MPI];
		load[j][MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[j][MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], archs[i].nominal_freq);
		cpi[j][MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);

		core_cstate_0[j] += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1[j] += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3[j] += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6[j] += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7[j] += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		if(cntd->pmc)
		{
			pmc0[j] += cpus[i].pmc0;
			pmc1[j] += cpus[i].pmc1;
			pmc2[j] += cpus[i].pmc2;
			pmc3[j] += cpus[i].pmc3;
			pmc4[j] += cpus[i].pmc4;
			pmc5[j] += cpus[i].pmc5;
			pmc6[j] += cpus[i].pmc6;
			pmc7[j] += cpus[i].pmc7;
		}

		cntd_impact[j] += ranks[i].timing[GT_TIMEOUT];
		for(z = 0; z < NUM_MPI_TYPE; z++)
			num_mpi_calls[j] += ranks[i].mpi_count[z][MPI_TYPE_TOT];
	}
	for(j = 0; j < num_sockets; j++)
	{
		load[j][TOT] = load[j][TOT] / cpu_per_socket[j];
		core_freq[j][TOT]	= core_freq[j][TOT] / cpu_per_socket[j];
		cpi[j][TOT]	= cpi[j][TOT] / cpu_per_socket[j];

		load[j][APP] = load[j][APP] / cpu_per_socket[j];
		core_freq[j][APP]	= core_freq[j][APP] / cpu_per_socket[j];
		cpi[j][APP]	= cpi[j][APP] / cpu_per_socket[j];

		load[j][MPI] = load[j][MPI] / cpu_per_socket[j];
		core_freq[j][MPI]	= core_freq[j][MPI] / cpu_per_socket[j];
		cpi[j][MPI]	= cpi[j][MPI] / cpu_per_socket[j];

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
			(cntd_impact[j] * 100.0) / timing[j][TOT],
			net[SEND][j],
			net[RECV][j],
			file[READ][j],
			file[WRITE][j],
			core_temp[j]);
		if(cntd->debug_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;"
				";%lu;%lu;%lu;"
				";%lu;%lu;%lu;",
				tsc[j][TOT], aperf[j], mperf[j], clk_curr[j][TOT], clk_ref[j][TOT],
				tsc[j][APP], clk_curr[j][APP], clk_ref[j][APP],
				tsc[j][MPI], clk_curr[j][MPI], clk_ref[j][MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.9f;%lu;%.3f;%.0f;%.3f"
			";%.2f;%.2f;%.2f;%.2f;%.2f"
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
			energy_pkg,
			energy_dram,
			energy_tot,
			energy_pkg / exe_time,
			energy_dram / exe_time,
			energy_tot / exe_time,
			sockets[i].temp / sockets[i].num_samples);
		if(cntd->pmc)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
				pmc0[j],
				pmc1[j],
				pmc2[j],
				pmc3[j],
				pmc4[j],
				pmc5[j],
				pmc6[j],
				pmc7[j]);
		if(cntd->debug_metrics)
			fprintf(fd, ";%lu", sockets[i].uclk);
		fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f",
			get_uncore_freq(sockets[i].uclk, exe_time, sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc),
			get_pkg_C0(sockets[i].C2, sockets[i].C3, sockets[i].C6, sockets[i].C7, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C2, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C3, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C6, cpus[i].tsc),
			get_pkg_cstate(sockets[i].C7, cpus[i].tsc));
		if(cntd->pcu)
			fprintf(fd, ";%lu;%lu;%lu;%lu",
				sockets[i].pcu0,
				sockets[i].pcu1,
				sockets[i].pcu2,
				sockets[i].pcu3);
		fprintf(fd, "\n");
	}

	fclose(fd);
}

static void print_nodes_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets, CNTD_Arch_t *archs)
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
	if(snprintf(log_file, sizeof(log_file), "%s/cntd_nodes.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary node trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "hostname;cpu_model;sockets;cores;ht_enable;cpus;nominal_freq;num_mpi_procs"
				";num_mpi_calls;cntd_impact;net_send;net_recv;file_read;file_write;core_temp");
	if(cntd->debug_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
			";APP-tsc;APP-clk_curr;APP-clk_ref"
			";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APP-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot");
	if(cntd->pmc)
		fprintf(fd, ";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7");
	if(cntd->debug_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_temp;pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7");
	if(cntd->pcu)
		fprintf(fd, ";pcu0;pcu1;pcu2;pcu3");
	fprintf(fd, "\n");

	double exe_time = cntd->epoch[END] - cntd->epoch[START];

	// Find cpu indexs
	memset(num_mpi_procs, 0, sizeof(int)*cntd->rank->size);
	for(i = 0, num_nodes = 0; i < cntd->rank->size; i++)
	{
		flag = FALSE;
		for(j = 0; j < num_nodes; j++)
		{
			if(strcmp(keys[j], archs[i].hostname) == 0)
			{
				flag = TRUE;
				break;
			}
		}

		if(!flag)
		{
			strcpy(keys[num_nodes], archs[i].hostname);
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
	double cntd_impact[num_nodes];

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
	if(cntd->pmc)
	{
		memset(pmc0, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc1, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc2, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc3, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc4, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc5, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc6, 0, sizeof(uint64_t)*num_nodes);
		memset(pmc7, 0, sizeof(uint64_t)*num_nodes);
	}

	memset(num_mpi_calls, 0, sizeof(uint64_t)*num_nodes);
	memset(cntd_impact, 0, sizeof(double)*num_nodes);

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
		core_freq[j][TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, archs[i].nominal_freq);
		cpi[j][TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		timing[j][APP] += ranks[i].timing[APP];
		tsc[j][APP] += ranks[i].tsc[APP];
		clk_curr[j][APP] += ranks[i].clk_curr[APP];
		clk_ref[j][APP] += ranks[i].clk_ref[APP];
		inst_ret[j][APP] += ranks[i].inst_ret[APP];
		load[j][APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[j][APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], archs[i].nominal_freq);
		cpi[j][APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);

		timing[j][MPI] += ranks[i].timing[MPI];
		tsc[j][MPI] += ranks[i].tsc[MPI];
		clk_curr[j][MPI] += ranks[i].clk_curr[MPI];
		clk_ref[j][MPI] += ranks[i].clk_ref[MPI];
		inst_ret[j][MPI] += ranks[i].inst_ret[MPI];
		load[j][MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[j][MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], archs[i].nominal_freq);
		cpi[j][MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);

		core_cstate_0[j] += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1[j] += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3[j] += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6[j] += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7[j] += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		if(cntd->pmc)
		{
			pmc0[j] += cpus[i].pmc0;
			pmc1[j] += cpus[i].pmc1;
			pmc2[j] += cpus[i].pmc2;
			pmc3[j] += cpus[i].pmc3;
			pmc4[j] += cpus[i].pmc4;
			pmc5[j] += cpus[i].pmc5;
			pmc6[j] += cpus[i].pmc6;
			pmc7[j] += cpus[i].pmc7;
		}

		cntd_impact[j] += ranks[i].timing[GT_TIMEOUT];
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

		if(snprintf(str_tmp, sizeof(str_tmp), "%s-%d", archs[i].hostname, ranks[i].socket_id) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of the string tmp!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
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
			if(strcmp(keys[z], archs[i].hostname) == 0)
			{
				flag = TRUE;
				break;
			}
		}

		if(!flag)
		{
			strcpy(keys[num_nodes], archs[i].hostname);
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
	if(cntd->pcu)
	{
		memset(pcu0, 0, sizeof(uint64_t)*num_nodes);
		memset(pcu1, 0, sizeof(uint64_t)*num_nodes);
		memset(pcu2, 0, sizeof(uint64_t)*num_nodes);
		memset(pcu3, 0, sizeof(uint64_t)*num_nodes);
	}

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
		if(cntd->pcu)
		{
			pcu0[z] = sockets[i].pcu0;
			pcu1[z] = sockets[i].pcu1;
			pcu2[z] = sockets[i].pcu2;
			pcu3[z] = sockets[i].pcu3;
		}
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

		fprintf(fd, "%s;%s;%d;%d;%d;%d;%d;%d;%lu;%.2f"
					";%lu;%lu;%lu;%lu;%.2f",
					archs[i].hostname,
					archs[i].cpu_model_name,
					archs[i].sockets,
					archs[i].cores,
					archs[i].ht_enable,
					archs[i].cpus,
					archs[i].nominal_freq,
					num_mpi_procs[z],
					num_mpi_calls[z],
					(cntd_impact[z] * 100.0) / timing[z][TOT],
					net[SEND][z],
					net[RECV][z],
					file[READ][z],
					file[WRITE][z],
					core_temp[z]);
		if(cntd->debug_metrics)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
				";%lu;%lu;%lu"
				";%lu;%lu;%lu",
				tsc[z][TOT], aperf[z], mperf[z], clk_curr[z][TOT], clk_ref[z][TOT],
				tsc[z][APP], clk_curr[z][APP], clk_ref[z][APP],
				tsc[z][MPI], clk_curr[z][MPI], clk_ref[z][MPI]);
		fprintf(fd, ";%lu;%.3f;%.0f;%.3f"
					";%.9f;%lu;%.3f;%.0f;%.3f"
					";%.9f;%lu;%.3f;%.0f;%.3f"
					";%.2f;%.2f;%.2f;%.2f;%.2f"
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
					energy_pkg[z],
					energy_dram[z],
					energy_pkg[z] + energy_dram[z],
					energy_pkg[z] / exe_time,
					energy_dram[z] / exe_time,
					(energy_pkg[z] + energy_dram[z]) / exe_time);
		if(cntd->pmc)
			fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
				pmc0[z],
				pmc1[z],
				pmc2[z],
				pmc3[z],
				pmc4[z],
				pmc5[z],
				pmc6[z],
				pmc7[z]);
		if(cntd->debug_metrics)
			fprintf(fd, ";%lu", uclk[z]);
		fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f",
					uncore_freq[z],
					pkg_temp[z],
					pkg_cstate_0[z],
					pkg_cstate_2[z],
					pkg_cstate_3[z],
					pkg_cstate_6[z],
					pkg_cstate_7[z]);
		if(cntd->pcu)
			fprintf(fd, ";%lu;%lu;%lu;%lu",
					pcu0[z],
					pcu1[z],
					pcu2[z],
					pcu3[z]);
		fprintf(fd, "\n");
	}

	fclose(fd);
}

static void print_summary_file(CNTD_Rank_t *ranks, CNTD_Cpu_t *cpus, CNTD_Socket_t *sockets, CNTD_Arch_t *archs)
{
	int i, j;
	char log_file[STRING_SIZE];
	char keys[cntd->rank->size][STRING_SIZE];
	char str_tmp[STRING_SIZE], start_time[STRING_SIZE], end_time[STRING_SIZE];
	int socket_to_cpu[cntd->rank->size];
	int flag, num_sockets;
	double exe_time, timing[3], cntd_impact;
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

	cntd_impact = 0;
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

	if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary trace file: %s\n", log_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Print labels
	fprintf(fd, "start_time;end_time;exe_time;num_mpi_procs;num_mpi_calls"
	";barrier;fermata;andante;eam_slack;eam_call;pmc;pcu;debug_metrics"
	";max_pstate;min_pstate;timeout;eam_impact;net_send;net_recv;file_read;file_write;core_temp");
	if(cntd->debug_metrics)
		fprintf(fd, ";tsc;aperf;mperf;clk_curr;clk_ref"
					";APP-tsc;APP-clk_curr;APP-clk_ref"
					";MPI-tsc;MPI-clk_curr;MPI-clk_ref");
	fprintf(fd, ";inst_ret;load;core_freq;cpi"
				";APP-time;APP-inst_ret;APP-load;APP-core_freq;APP-cpi"
				";MPI-time;MPI-inst_ret;MPI-load;MPI-core_freq;MPI-cpi"
				";core_cstate_0;core_cstate_1;core_cstate_3;core_cstate_6;core_cstate_7"
				";energy_pkg;energy_dram;energy_tot;power_pkg;power_dram;power_tot");
	if(cntd->pmc)
		fprintf(fd, ";pmc0;pmc1;pmc2;pmc3;pmc4;pmc5;pmc6;pmc7");
	if(cntd->debug_metrics)
		fprintf(fd, ";uclk");
	fprintf(fd, ";uncore_freq;pkg_temp;pkg_cstate_0;pkg_cstate_2;pkg_cstate_3;pkg_cstate_6;pkg_cstate_7");
	if(cntd->pcu)
		fprintf(fd, ";pcu0;pcu1;pcu2;pcu3");
	fprintf(fd, "\n");

	// Time conversion
	timespec2str(start_time, STRING_SIZE, double2timespec(cntd->epoch[START]));
	timespec2str(end_time, STRING_SIZE, double2timespec(cntd->epoch[END]));

	// Calculate core data
	exe_time = cntd->epoch[END] - cntd->epoch[START];
	for(i = 0; i < cntd->rank->size; i++)
	{
		timing[TOT] += ranks[i].timing[TOT];
		timing[APP] += ranks[i].timing[APP];
		timing[MPI] += ranks[i].timing[MPI];

		cntd_impact += ranks[i].timing[GT_TIMEOUT];
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
		core_freq[TOT] += get_core_freq(cpus[i].clk_curr, cpus[i].clk_ref, archs[i].nominal_freq);
		cpi[TOT] += get_cpi(cpus[i].clk_ref, cpus[i].inst_ret);

		tsc[APP] += ranks[i].tsc[APP];
		clk_curr[APP] += ranks[i].clk_curr[APP];
		clk_ref[APP] += ranks[i].clk_ref[APP];
		inst_ret[APP] += ranks[i].inst_ret[APP];
		load[APP] += get_load(ranks[i].clk_ref[APP], ranks[i].tsc[APP]);
		core_freq[APP] += get_core_freq(ranks[i].clk_curr[APP], ranks[i].clk_ref[APP], archs[i].nominal_freq);
		cpi[APP] += get_cpi(ranks[i].clk_ref[APP], ranks[i].inst_ret[APP]);

		tsc[MPI] += ranks[i].tsc[MPI];
		clk_curr[MPI] += ranks[i].clk_curr[MPI];
		clk_ref[MPI] += ranks[i].clk_ref[MPI];
		inst_ret[MPI] += ranks[i].inst_ret[MPI];
		load[MPI] += get_load(ranks[i].clk_ref[MPI], ranks[i].tsc[MPI]);
		core_freq[MPI] += get_core_freq(ranks[i].clk_curr[MPI], ranks[i].clk_ref[MPI], archs[i].nominal_freq);
		cpi[MPI] += get_cpi(ranks[i].clk_ref[MPI], ranks[i].inst_ret[MPI]);

		core_cstate_0 += get_core_C0(cpus[i].clk_ref, cpus[i].tsc);
		core_cstate_1 += get_core_C1(cpus[i].clk_ref, cpus[i].tsc, cpus[i].C3, cpus[i].C6, cpus[i].C7);
		core_cstate_3 += get_core_cstate(cpus[i].C3, cpus[i].tsc);
		core_cstate_6 += get_core_cstate(cpus[i].C6, cpus[i].tsc);
		core_cstate_7 += get_core_cstate(cpus[i].C7, cpus[i].tsc);

		if(cntd->pmc)
		{
			pmc0 += cpus[i].pmc0;
			pmc1 += cpus[i].pmc1;
			pmc2 += cpus[i].pmc2;
			pmc3 += cpus[i].pmc3;
			pmc4 += cpus[i].pmc4;
			pmc5 += cpus[i].pmc5;
			pmc6 += cpus[i].pmc6;
			pmc7 += cpus[i].pmc7;
		}
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
		if(snprintf(str_tmp, sizeof(str_tmp), "%s-%d", archs[i].hostname, ranks[i].socket_id) < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to create the name of the string tmp!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
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

		if(cntd->pcu)
		{
			pcu0 += sockets[j].pcu0;
			pcu1 += sockets[j].pcu1;
			pcu2 += sockets[j].pcu2;
			pcu3 += sockets[j].pcu3;
		}
	}
	uncore_freq = uncore_freq / num_sockets;
	pkg_temp = pkg_temp / (double) num_sockets;

	pkg_cstate_0 = pkg_cstate_0 / (double) num_sockets;
	pkg_cstate_2 = pkg_cstate_2 / (double) num_sockets;
	pkg_cstate_3 = pkg_cstate_3 / (double) num_sockets;
	pkg_cstate_6 = pkg_cstate_6 / (double) num_sockets;
	pkg_cstate_7 = pkg_cstate_7 / (double) num_sockets;

	// print data
	fprintf(fd, "%s;%s;%.9f;%d;%lu;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%.6f;%.2f;%lu;%lu;%lu;%lu;%.2f",
		start_time,
		end_time,
		exe_time,
		cntd->rank->size,
		num_mpi_calls,
		cntd->barrier,
		cntd->fermata,
		cntd->andante,
		cntd->eam_slack,
		cntd->eam_call,
		cntd->pmc,
		cntd->pcu,
		cntd->debug_metrics,
		cntd->arch.pstate[MAX],
		cntd->arch.pstate[MIN],
		cntd->timeout,
		(cntd_impact * 100.0) / timing[TOT],
		net[SEND],
		net[RECV],
		file[READ],
		file[WRITE],
		core_temp);
	if(cntd->debug_metrics)
		fprintf(fd, ";%lu;%lu;%lu;%lu;%lu"
			";%lu;%lu;%lu"
			";%lu;%lu;%lu",
			tsc[TOT], aperf, mperf, clk_curr[TOT], clk_ref[TOT],
			tsc[APP], clk_curr[APP], clk_ref[APP],
			tsc[MPI], clk_curr[MPI], clk_ref[MPI]);
	fprintf(fd, ";%lu;%.3f;%.0f;%.3f;%.9f;%lu;%.3f;%.0f;%.3f;%.9f;%lu;%.3f;%.0f;%.3f;%.2f;%.2f;%.2f;%.2f;%.2f;%.0f;%.0f;%.0f;%.2f;%.2f;%.2f",
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
		energy_pkg,
		energy_dram,
		energy_pkg + energy_dram,
		energy_pkg / exe_time,
		energy_dram / exe_time,
		(energy_pkg + energy_dram) / exe_time);
	if(cntd->pmc)
		fprintf(fd, ";%lu;%lu;%lu;%lu;%lu;%lu;%lu;%lu",
			pmc0,
			pmc1,
			pmc2,
			pmc3,
			pmc4,
			pmc5,
			pmc6,
			pmc7);
	if(cntd->debug_metrics)
		fprintf(fd, ";%lu",	uclk);
	fprintf(fd, ";%.0f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f",
		uncore_freq,
		pkg_temp,
		pkg_cstate_0,
		pkg_cstate_2,
		pkg_cstate_3,
		pkg_cstate_6,
		pkg_cstate_7);
	if(cntd->pcu)
		fprintf(fd, ";%lu;%lu;%lu;%lu",
			pcu0,
			pcu1,
			pcu2,
			pcu3);
	fprintf(fd, "\n");

	fclose(fd);
}

static void print_summary_mpi_file(CNTD_Rank_t *ranks, CNTD_Arch_t *archs)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];
	uint64_t count_proc, count, inst_ret, send_data, recv_data;
	double timing, load, freq, cpi;

	if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_mpi.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create summary mpi trace file: %s\n", log_file);
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
	fprintf(fd, "mpi;time;count;send_data;recv_data;load;freq;cpi;inst_ret\n");

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
				freq += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_TOT], ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], archs[j].nominal_freq) : 0;
				cpi += ranks[j].mpi_count[i][MPI_TYPE_TOT] != 0 ? get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_TOT], ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT]) : 0;
				inst_ret += ranks[j].mpi_inst_ret[i][MPI_TYPE_TOT];
			}

			if(count_proc > 1)
			{
				load /= count_proc;
				freq /= count_proc;
				cpi /= count_proc;
			}

			fprintf(fd, "%s;%.9f;%lu;%lu;%lu;%.2f;%.0f;%.3f;%lu\n", mpi_type_str[i]+2, timing, count, send_data, recv_data, load, freq, cpi, inst_ret);
		}
	}

	fclose(fd);
}

static void print_summary_eam_file(CNTD_Rank_t *ranks, CNTD_Arch_t *archs)
{
	int i, j;
	char log_file[STRING_SIZE];
	int flag[NUM_MPI_TYPE];
	uint64_t count_proc, count, inst_ret, send_data, recv_data;
	double timing, load, freq, cpi;

	if(snprintf(log_file, sizeof(log_file), "%s/cntd_summary_eam.csv", cntd->log_dir) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to create the name of the log file!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	FILE *fd = fopen(log_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed to create eam summary mpi trace file: %s\n", log_file);
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
	fprintf(fd, "mpi;time;count;send_data;recv_data;load;freq;cpi;inst_ret\n");

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
				load += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ?
					get_load(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM],
						ranks[j].mpi_tsc[i][MPI_TYPE_EAM]) : 0;
				freq += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ?
					get_core_freq(ranks[j].mpi_clk_curr[i][MPI_TYPE_EAM],
						ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM], archs[j].nominal_freq) : 0;
				cpi += ranks[j].mpi_count[i][MPI_TYPE_EAM] != 0 ?
					get_cpi(ranks[j].mpi_clk_ref[i][MPI_TYPE_EAM],
						ranks[j].mpi_inst_ret[i][MPI_TYPE_EAM]) : 0;
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
				fprintf(fd, "%s;%.9f;%lu;%lu;%lu;%.2f;%.0f;%.3f;%lu\n",
					mpi_type_str[i]+2, timing, count, send_data, recv_data,
					load, freq, cpi, inst_ret);
			}
		}
	}

	fclose(fd);
}

void print_logs()
{
	CNTD_Rank_t ranks[cntd->rank->size];
	CNTD_Cpu_t cpus[cntd->rank->size];
	CNTD_Socket_t sockets[cntd->rank->size];
	CNTD_Arch_t archs[cntd->rank->size];

	MPI_Datatype rank_type = get_mpi_datatype_rank();
	MPI_Datatype cpu_type = get_mpi_datatype_cpu();
	MPI_Datatype socket_type = get_mpi_datatype_socket();
	MPI_Datatype arch_type = get_mpi_datatype_arch();

	PMPI_Gather(cntd->rank, 1, rank_type, ranks, 1, rank_type, CNTD_MPI_ROOT, MPI_COMM_WORLD);
	PMPI_Gather(&(cntd->cpu[cntd->rank->cpu_id]), 1, cpu_type, cpus, 1, cpu_type, CNTD_MPI_ROOT, MPI_COMM_WORLD);
	PMPI_Gather(&(cntd->socket[cntd->rank->socket_id]), 1, socket_type, sockets, 1, socket_type, CNTD_MPI_ROOT, MPI_COMM_WORLD);
	PMPI_Gather(&(cntd->arch), 1, arch_type, archs, 1, arch_type, CNTD_MPI_ROOT, MPI_COMM_WORLD);

	// Bug if it used local folders
	if(cntd->event_trace || cntd->task_trace)
		print_comms_file();
	if(cntd->andante || cntd->andante_analysis)
		print_summary_andante_file();
	if(cntd->fermata || cntd->fermata_analysis)
		print_summary_fermata_file();
	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
	{
		print_cpus_file(ranks, cpus, archs);
		print_sockets_file(ranks, cpus, sockets, archs);
		print_nodes_file(ranks, cpus, sockets, archs);
		print_summary_file(ranks, cpus, sockets, archs);
		print_summary_mpi_file(ranks, archs);
		if(cntd->eam_slack || cntd->eam_call ||
			cntd->eam_slack_analysis || cntd->eam_call_analysis ||
			cntd->fermata || cntd->fermata_analysis)
			print_summary_eam_file(ranks, archs);
		if(cntd->event_trace || cntd->task_trace)
			print_mpi_type();
		if(cntd->event_trace)
			print_event_metadata_file();
		if(cntd->task_trace)
			print_task_metadata_file();
	}
	PMPI_Barrier(MPI_COMM_WORLD);
}
