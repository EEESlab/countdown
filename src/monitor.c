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

// FIXED PMC
#define RDPMC_INSTR		(1 << 30)
#define RDPMC_CLKCURR	((1 << 30) + 1)
#define RDPMC_CLKREF	((1 << 30) + 2)

__attribute__((unused)) static uint64_t RDTSC()
{
	unsigned a, d;
	__asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
	return ((unsigned long) a) | (((unsigned long) d) << 32);
}

__attribute__((unused)) static uint64_t RDTSCP()
{
	unsigned a, d;
	__asm__ volatile("rdtscp" : "=a"(a), "=d"(d));
	return ((unsigned long) a) | (((unsigned long) d) << 32);
}

static uint64_t RDPMC(unsigned pmc)
{
	unsigned a, d;
	__asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (pmc));
	return ((unsigned long)a) | (((unsigned long)d) << 32);
}

void add_profiling(CNTD_Call_t *call, int when)
{
	int i;
	struct timespec epoch;

	// Timing
	call->tsc[when] = READ_TSC();
	clock_gettime(CLOCK_TYPE, &epoch);

	// Fixed
	call->fix[0][when] = RDPMC(RDPMC_INSTR);
	call->fix[1][when] = RDPMC(RDPMC_CLKCURR);
	call->fix[2][when] = RDPMC(RDPMC_CLKREF);

	// PMC
	if(cntd->pmc)
	{
		for(i = 0; i < 8; i++)
			call->pmc[i][when] = RDPMC(i);
	}

	// Timing
	call->epoch[when] = timespec2double(epoch);

	// Stacktrace
	if(cntd->event_trace && when == END)
	{
		double time_duration = call->epoch[END] - call->epoch[START];
	 	if(time_duration > cntd->timeout)
			call->stacktrace = hash_backtrace(call->mpi_type);
	}
}

void add_network(CNTD_Call_t *call,
	const int *send_count, MPI_Datatype *send_type, int dest,
	const int *recv_count, MPI_Datatype *recv_type, int source)
{
	int i, payload, send_size, recv_size;
	CNTD_Group_t *group;

	// Send
	call->net_addr[DEST] = dest;
	if(dest == MPI_NONE)
	{
		call->net[SEND] = 0;
	}
	else if(dest == MPI_ALL)
	{
		PMPI_Type_size(*send_type, &send_size);

		payload = (*send_count) * send_size;
		group = call->cntd_comm->cntd_group;
		call->net[SEND] = payload * group->size;
	}
	else if(dest == MPI_ALLV)
	{
		PMPI_Type_size(*send_type, &send_size);
		group = call->cntd_comm->cntd_group;
		for(i = 0; i < group->size; i++)
		{
			payload = send_count[i] * send_size;
			call->net[SEND] += payload;
		}
	}
	else if(dest == MPI_ALLW)
	{
		group = call->cntd_comm->cntd_group;
		for(i = 0; i < group->size; i++)
		{
			PMPI_Type_size(send_type[i], &send_size);
			payload = send_count[i] * send_size;
			call->net[SEND] += payload;
		}
	}
	else
	{
		PMPI_Type_size(*send_type, &send_size);
		payload = (*send_count) * send_size;
		call->net[SEND] = payload;
		group = call->cntd_comm->cntd_group;
	}

	// Receive
	call->net_addr[SOURCE] = source;
	if(source == MPI_NONE)
	{
		call->net[RECV] = 0;
	}
	else if(source == MPI_ALL)
	{
		PMPI_Type_size(*recv_type, &recv_size);
		payload = (*recv_count) * recv_size;
		group = call->cntd_comm->cntd_group;
		call->net[RECV] = payload * group->size;
	}
	else if(source == MPI_ALLV)
	{
		group = call->cntd_comm->cntd_group;
		PMPI_Type_size(*recv_type, &recv_size);
		for(i = 0; i < group->size; i++)
		{
			payload = recv_count[i] * recv_size;
			call->net[RECV] = payload;
		}
	}
	else if(source == MPI_ALLW)
	{
		group = call->cntd_comm->cntd_group;

		for(i = 0; i < group->size; i++)
		{
			PMPI_Type_size(recv_type[i], &recv_size);
			payload = recv_count[i] * recv_size;
			call->net[RECV] = payload;
		}
	}
	else
	{
		PMPI_Type_size(*recv_type, &recv_size);
		payload = (*recv_count) * recv_size;
		call->net[RECV] = payload;
		group = call->cntd_comm->cntd_group;
	}
}

void add_file(CNTD_Call_t *call,
	int read_count, MPI_Datatype read_datatype,
	int write_count, MPI_Datatype write_datatype)
{
	int read_size, write_size;

	if(read_count > 0)
	{
		PMPI_Type_size(read_datatype, &read_size);
		call->file[READ] = read_count * read_size;
		cntd->rank->file[READ] += call->file[READ];
	}

	if(write_count > 0)
	{
		PMPI_Type_size(write_datatype, &write_size);
		call->file[WRITE] = write_count * write_size;
		cntd->rank->file[WRITE] += call->file[WRITE];
	}
}

void update_call()
{
	int i;

	CNTD_Call_t *curr_call = &cntd->call[cntd->curr_call];
	CNTD_Call_t *prev_call = &cntd->call[cntd->prev_call];

	// Time
	double mpi_time_duration = (curr_call->epoch[END] - curr_call->epoch[START]);
	cntd->rank->epoch[START] = curr_call->epoch[START] - cntd->epoch[START];
	cntd->rank->epoch[END] = curr_call->epoch[END] - cntd->epoch[START];

	// Network
	cntd->rank->net[SEND] += curr_call->net[SEND];
	cntd->rank->net[RECV] += curr_call->net[RECV];

	// File
	cntd->rank->file[READ] += curr_call->file[READ];
	cntd->rank->file[WRITE] += curr_call->file[WRITE];

	// Call profiling
	cntd->rank->mpi_count[curr_call->mpi_type][MPI_TYPE_TOT]++;
	cntd->rank->mpi_timing[curr_call->mpi_type][MPI_TYPE_TOT] += mpi_time_duration;
	cntd->rank->mpi_tsc[curr_call->mpi_type][MPI_TYPE_TOT] += diff_64(curr_call->tsc[END], curr_call->tsc[START]);
	cntd->rank->mpi_inst_ret[curr_call->mpi_type][MPI_TYPE_TOT] += diff_48(curr_call->fix[0][END], curr_call->fix[0][START]);
	cntd->rank->mpi_clk_curr[curr_call->mpi_type][MPI_TYPE_TOT] += diff_48(curr_call->fix[1][END], curr_call->fix[1][START]);
	cntd->rank->mpi_clk_ref[curr_call->mpi_type][MPI_TYPE_TOT] += diff_48(curr_call->fix[2][END], curr_call->fix[2][START]);
	cntd->rank->mpi_send_data[curr_call->mpi_type][MPI_TYPE_TOT] += (curr_call->net[SEND] + curr_call->file[WRITE]);
	cntd->rank->mpi_recv_data[curr_call->mpi_type][MPI_TYPE_TOT] += (curr_call->net[RECV] + curr_call->file[READ]);

	// Total: timing & HW perf counters
	cntd->rank->timing[TOT] += (curr_call->epoch[END] - prev_call->epoch[END]);

	cntd->rank->tsc[TOT] += diff_64(curr_call->tsc[END], prev_call->tsc[END]);
	cntd->rank->inst_ret[TOT] += diff_48(curr_call->fix[0][END], prev_call->fix[0][END]);
	cntd->rank->clk_curr[TOT] += diff_48(curr_call->fix[1][END], prev_call->fix[1][END]);
	cntd->rank->clk_ref[TOT] += diff_48(curr_call->fix[2][END], prev_call->fix[2][END]);

	if(cntd->pmc)
		for(i = 0; i < 8; i++)
			cntd->rank->pmc[TOT][i] += diff_48(curr_call->pmc[i][END], prev_call->pmc[i][END]);

	// Application: timing & HW perf counters
	cntd->rank->timing[APP] += (curr_call->epoch[START] - prev_call->epoch[END]);
	cntd->rank->tsc[APP] += diff_64(curr_call->tsc[START], prev_call->tsc[END]);
	cntd->rank->inst_ret[APP] += diff_48(curr_call->fix[0][START], prev_call->fix[0][END]);
	cntd->rank->clk_curr[APP] += diff_48(curr_call->fix[1][START], prev_call->fix[1][END]);
	cntd->rank->clk_ref[APP] += diff_48(curr_call->fix[2][START], prev_call->fix[2][END]);
	if(cntd->pmc)
		for(i = 0; i < 8; i++)
			cntd->rank->pmc[APP][i] += diff_48(curr_call->pmc[i][START], prev_call->pmc[i][END]);

	// MPI: timing & HW perf counters
	cntd->rank->timing[MPI] += mpi_time_duration;
	cntd->rank->tsc[MPI] += diff_64(curr_call->tsc[END], curr_call->tsc[START]);
	cntd->rank->inst_ret[MPI] += diff_48(curr_call->fix[0][END], curr_call->fix[0][START]);
	cntd->rank->clk_curr[MPI] += diff_48(curr_call->fix[1][END], curr_call->fix[1][START]);
	cntd->rank->clk_ref[MPI] += diff_48(curr_call->fix[2][END], curr_call->fix[2][START]);

	if(cntd->pmc)
		for(i = 0; i < 8; i++)
			cntd->rank->pmc[MPI][i] += diff_48(curr_call->pmc[i][END], curr_call->pmc[i][START]);

	// CNTD impact
	double timing_gt = mpi_time_duration - cntd->timeout;
	if(timing_gt > 0)
		cntd->rank->timing[GT_TIMEOUT] += timing_gt;

	// EAM profiling
	if(curr_call->flag_eam)
	{
		cntd->rank->mpi_count[curr_call->mpi_type][MPI_TYPE_EAM]++;
		cntd->rank->mpi_send_data[curr_call->mpi_type][MPI_TYPE_EAM] = curr_call->net[SEND] + curr_call->file[WRITE];
		cntd->rank->mpi_recv_data[curr_call->mpi_type][MPI_TYPE_EAM] = curr_call->net[RECV] + curr_call->file[READ];
		cntd->rank->mpi_timing[curr_call->mpi_type][MPI_TYPE_EAM] += timing_gt;
		cntd->rank->mpi_tsc[curr_call->mpi_type][MPI_TYPE_EAM] += diff_64(curr_call->tsc[END], curr_call->tsc[START]);
		cntd->rank->mpi_inst_ret[curr_call->mpi_type][MPI_TYPE_EAM] += diff_48(curr_call->fix[0][END], curr_call->fix[0][START]);
		cntd->rank->mpi_clk_curr[curr_call->mpi_type][MPI_TYPE_EAM] += diff_48(curr_call->fix[1][END], curr_call->fix[1][START]);
		cntd->rank->mpi_clk_ref[curr_call->mpi_type][MPI_TYPE_EAM] += diff_48(curr_call->fix[2][END], curr_call->fix[2][START]);
		if(cntd->pmc)
			for(i = 0; i < 8; i++)
				cntd->rank->mpi_pmc[curr_call->mpi_type][MPI_TYPE_EAM][i] += diff_48(curr_call->pmc[i][END], curr_call->pmc[i][START]);
	}
}

void switch_call_ptr()
{
	if(cntd->curr_call == 0)
	{
		cntd->curr_call = 1;
		cntd->prev_call = 0;
	}
	else
	{
		cntd->curr_call = 0;
		cntd->prev_call = 1;
	}
}
