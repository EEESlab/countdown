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

static MPI_Type_t get_collective_barrier_type(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_CART_CREATE:
			return __MPI_CART_CREATE__BARRIER;
			break;
		case __MPI_COMM_CREATE:
			return __MPI_COMM_CREATE__BARRIER;
			break;
		case __MPI_COMM_SPLIT:
			return __MPI_COMM_SPLIT__BARRIER;
			break;
		case __MPI_ALLGATHER:
			return __MPI_ALLGATHER__BARRIER;
			break;
		case __MPI_ALLGATHERV:
			return __MPI_ALLGATHERV__BARRIER;
			break;
		case __MPI_ALLREDUCE:
			return __MPI_ALLREDUCE__BARRIER;
			break;
		case __MPI_ALLTOALL:
			return __MPI_ALLTOALL__BARRIER;
			break;
		case __MPI_ALLTOALLV:
			return __MPI_ALLTOALLV__BARRIER;
			break;
		case __MPI_ALLTOALLW:
			return __MPI_ALLTOALLW__BARRIER;
			break;
		case __MPI_BCAST:
			return __MPI_BCAST__BARRIER;
			break;
		case __MPI_EXSCAN:
			return __MPI_EXSCAN__BARRIER;
			break;
		case __MPI_SCAN:
			return __MPI_SCAN__BARRIER;
			break;
		case __MPI_GATHER:
			return __MPI_GATHER__BARRIER;
			break;
		case __MPI_GATHERV:
			return __MPI_GATHERV__BARRIER;
			break;
		case __MPI_REDUCE:
			return __MPI_REDUCE__BARRIER;
			break;
		case __MPI_REDUCE_SCATTER:
			return __MPI_REDUCE_SCATTER__BARRIER;
			break;
		case __MPI_SCATTER:
			return __MPI_SCATTER__BARRIER;
			break;
		case __MPI_SCATTERV:
			return __MPI_SCATTERV__BARRIER;
			break;
		case __MPI_NEIGHBOR_ALLGATHER:
			return __MPI_NEIGHBOR_ALLGATHER__BARRIER;
			break;
		case __MPI_NEIGHBOR_ALLGATHERV:
			return __MPI_NEIGHBOR_ALLGATHERV__BARRIER;
			break;
		case __MPI_NEIGHBOR_ALLTOALL:
			return __MPI_NEIGHBOR_ALLTOALL__BARRIER;
			break;
		case __MPI_NEIGHBOR_ALLTOALLV:
			return __MPI_NEIGHBOR_ALLTOALLV__BARRIER;
			break;
		case __MPI_NEIGHBOR_ALLTOALLW:
			return __MPI_NEIGHBOR_ALLTOALLW__BARRIER;
			break;
		case __MPI_SENDRECV:
			return __MPI_SENDRECV__BARRIER;
			break;
		case __MPI_SENDRECV_REPLACE:
			return __MPI_SENDRECV_REPLACE__BARRIER;
			break;
		default:
			return NUM_MPI_TYPE;
			break;
	}
}

static MPI_Type_t get_p2p_barrier_type(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_SEND:
			return __MPI_SEND__BARRIER;
			break;
		case __MPI_SSEND:
			return __MPI_SSEND__BARRIER;
			break;
		case __MPI_BSEND:
			return __MPI_BSEND__BARRIER;
			break;
		case __MPI_RSEND:
			return __MPI_RSEND__BARRIER;
			break;
		case __MPI_RECV:
			return __MPI_RECV__BARRIER;
			break;
		default:
			return NUM_MPI_TYPE;
			break;
	}
}

int is_collective_barrier(MPI_Type_t mpi_type)
{
	MPI_Type_t barrier_type = get_collective_barrier_type(mpi_type);
	if(barrier_type != NUM_MPI_TYPE)
		return TRUE;
	else
		return FALSE;
}

int is_p2p_barrier(MPI_Type_t mpi_type)
{
	MPI_Type_t barrier_type = get_p2p_barrier_type(mpi_type);
	if(barrier_type != NUM_MPI_TYPE)
		return TRUE;
	else
		return FALSE;
}

int is_cntd_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_SEND__BARRIER:
			return TRUE;
		case __MPI_SSEND__BARRIER:
			return TRUE;
		case __MPI_BSEND__BARRIER:
			return TRUE;
		case __MPI_RSEND__BARRIER:
			return TRUE;
		case __MPI_RECV__BARRIER:
			return TRUE;
		case __MPI_SENDRECV__BARRIER:
			return TRUE;
		case __MPI_SENDRECV_REPLACE__BARRIER:
			return TRUE;
		case __MPI_CART_CREATE__BARRIER:
			return TRUE;
		case __MPI_COMM_CREATE__BARRIER:
			return TRUE;
		case __MPI_COMM_SPLIT__BARRIER:
			return TRUE;
		case __MPI_ALLGATHER__BARRIER:
			return TRUE;
		case __MPI_ALLGATHERV__BARRIER:
			return TRUE;
		case __MPI_ALLREDUCE__BARRIER:
			return TRUE;
		case __MPI_ALLTOALL__BARRIER:
			return TRUE;
		case __MPI_ALLTOALLV__BARRIER:
			return TRUE;
		case __MPI_ALLTOALLW__BARRIER:
			return TRUE;
		case __MPI_BCAST__BARRIER:
			return TRUE;
		case __MPI_EXSCAN__BARRIER:
			return TRUE;
		case __MPI_SCAN__BARRIER:
			return TRUE;
		case __MPI_GATHER__BARRIER:
			return TRUE;
		case __MPI_GATHERV__BARRIER:
			return TRUE;
		case __MPI_REDUCE__BARRIER:
			return TRUE;
		case __MPI_REDUCE_SCATTER__BARRIER:
			return TRUE;
		case __MPI_SCATTER__BARRIER:
			return TRUE;
		case __MPI_SCATTERV__BARRIER:
			return TRUE;
		case __MPI_NEIGHBOR_ALLGATHER__BARRIER:
			return TRUE;
		case __MPI_NEIGHBOR_ALLGATHERV__BARRIER:
			return TRUE;
		case __MPI_NEIGHBOR_ALLTOALL__BARRIER:
			return TRUE;
		case __MPI_NEIGHBOR_ALLTOALLV__BARRIER:
			return TRUE;
		case __MPI_NEIGHBOR_ALLTOALLW__BARRIER:
			return TRUE;
		default:
			return FALSE;
			break;
	}
}

int is_wait_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_INIT:
			return TRUE;
			break;
		case __MPI_FINALIZE:
			return TRUE;
			break;
		// Barrier
		case __MPI_BARRIER:
			return TRUE;
			break;
		// Synchronization
		case __MPI_FILE_SYNC:
			return TRUE;
			break;
		case __MPI_WIN_FLUSH:
			return TRUE;
			break;
		case __MPI_WIN_FLUSH_ALL:
			return TRUE;
			break;
		case __MPI_WIN_FLUSH_LOCAL:
			return TRUE;
			break;
		case __MPI_WIN_FLUSH_LOCAL_ALL:
			return TRUE;
			break;
		case __MPI_WIN_SYNC:
			return TRUE;
			break;
		case __MPI_WIN_LOCK:
			return TRUE;
			break;
		case __MPI_WIN_LOCK_ALL:
			return TRUE;
			break;
		// Wait requests
		case __MPI_WAITALL:
			return TRUE;
			break;
		case __MPI_WAITANY:
			return TRUE;
			break;
		case __MPI_WAIT:
			return TRUE;
			break;
		case __MPI_WAITSOME:
			return TRUE;
			break;
		case __MPI_WIN_WAIT:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}

static void add_collective_barrier(MPI_Type_t mpi_type, MPI_Comm comm)
{
	MPI_Type_t barrier_type = get_collective_barrier_type(mpi_type);

	CNTD_Call_t *call = add_cntd_call(barrier_type, comm);
	call_start(call);
	PMPI_Barrier(comm);
	call_end(call);
}

static void add_send_barrier(MPI_Type_t mpi_type, MPI_Comm comm, int dest)
{
	int buff;
	MPI_Request request;
	MPI_Status status;

	CNTD_Call_t *call = add_cntd_call(mpi_type, comm);
	call_start(call);
	PMPI_Isend(&buff, 0, MPI_INT, dest, 0, comm, &request);
	PMPI_Wait(&request, &status);
	call_end(call);
}

static void add_recv_barrier(MPI_Type_t mpi_type, MPI_Comm comm, int source)
{
	int buff;
	MPI_Request request;
	MPI_Status status;

	CNTD_Call_t *call = add_cntd_call(mpi_type, comm);
	call_start(call);
	PMPI_Irecv(&buff, 0, MPI_INT, source, 0, comm, &request);
	PMPI_Wait(&request, &status);
	call_end(call);
}

static void add_p2p_barrier(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	MPI_Type_t barrier_type = get_p2p_barrier_type(mpi_type);

	switch(barrier_type)
	{
		case __MPI_SEND__BARRIER:
			add_send_barrier(barrier_type, comm, addr);
			break;
		case __MPI_SSEND__BARRIER:
			add_send_barrier(barrier_type, comm, addr);
			break;
		case __MPI_BSEND__BARRIER:
			add_send_barrier(barrier_type, comm, addr);
			break;
		case __MPI_RSEND__BARRIER:
			add_send_barrier(barrier_type, comm, addr);
			break;
		case __MPI_RECV__BARRIER:
			add_recv_barrier(barrier_type, comm, addr);
			break;
		default:
			break;
	}
}

void add_barrier(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	int barrier_buf;
	MPI_Request barrier_request;

	if(is_wait_barrier(mpi_type))
		return;

	if(is_collective_barrier(mpi_type))
		add_collective_barrier(mpi_type, comm);
	else if(is_p2p_barrier(mpi_type))
		add_p2p_barrier(mpi_type, comm, addr);
	else
	{
		switch(mpi_type)
		{
			case __MPI_ISEND:
				PMPI_Isend(&barrier_buf, 0, MPI_INT, addr, 0, comm, &barrier_request);
				break;
			case __MPI_ISSEND:
				PMPI_Isend(&barrier_buf, 0, MPI_INT, addr, 0, comm, &barrier_request);
				break;
			case __MPI_IRSEND:
				PMPI_Isend(&barrier_buf, 0, MPI_INT, addr, 0, comm, &barrier_request);
				break;
			case __MPI_IBSEND:
				PMPI_Isend(&barrier_buf, 0, MPI_INT, addr, 0, comm, &barrier_request);
				break;
			case __MPI_IRECV:
				PMPI_Irecv(&barrier_buf, 0, MPI_INT, addr, 0, comm, &barrier_request);
				break;
			default:
				break;
		}
	}
}
