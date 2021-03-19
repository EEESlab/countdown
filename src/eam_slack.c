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

static int flag_eam_slack = FALSE;

static void eam_slack_callback()
{
	flag_eam_slack = TRUE;
	set_min_pstate();
}

static int is_wait_mpi(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		// Barrier
		case __MPI_BARRIER:
			return TRUE;
		// Synchronization
		case __MPI_FILE_SYNC:
			return TRUE;
		case __MPI_WIN_FLUSH:
			return TRUE;
		case __MPI_WIN_FLUSH_ALL:
			return TRUE;
		case __MPI_WIN_FLUSH_LOCAL:
			return TRUE;
		case __MPI_WIN_FLUSH_LOCAL_ALL:
			return TRUE;
		case __MPI_WIN_SYNC:
			return TRUE;
		case __MPI_WIN_LOCK:
			return TRUE;
		case __MPI_WIN_LOCK_ALL:
			return TRUE;
		// Wait requests
		case __MPI_WAITALL:
			return TRUE;
		case __MPI_WAITANY:
			return TRUE;
		case __MPI_WAIT:
			return TRUE;
		case __MPI_WAITSOME:
			return TRUE;
		case __MPI_WIN_WAIT:
			return TRUE;
		// Finalization
		case __MPI_FINALIZE:
			return TRUE;
	}
	return FALSE;
}

static MPI_Type_t is_collective_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_CART_CREATE:
			return __MPI_CART_CREATE__BARRIER;
		case __MPI_COMM_CREATE:
			return __MPI_COMM_CREATE__BARRIER;
		case __MPI_COMM_SPLIT:
			return __MPI_COMM_SPLIT__BARRIER;
		case __MPI_ALLGATHER:
			return __MPI_ALLGATHER__BARRIER;
		case __MPI_ALLGATHERV:
			return __MPI_ALLGATHERV__BARRIER;
		case __MPI_ALLREDUCE:
			return __MPI_ALLREDUCE__BARRIER;
		case __MPI_ALLTOALL:
			return __MPI_ALLTOALL__BARRIER;
		case __MPI_ALLTOALLV:
			return __MPI_ALLTOALLV__BARRIER;
		case __MPI_ALLTOALLW:
			return __MPI_ALLTOALLW__BARRIER;
		case __MPI_BCAST:
			return __MPI_BCAST__BARRIER;
		case __MPI_EXSCAN:
			return __MPI_EXSCAN__BARRIER;
		case __MPI_SCAN:
			return __MPI_SCAN__BARRIER;
		case __MPI_GATHER:
			return __MPI_GATHER__BARRIER;
		case __MPI_GATHERV:
			return __MPI_GATHERV__BARRIER;
		case __MPI_REDUCE:
			return __MPI_REDUCE__BARRIER;
		case __MPI_REDUCE_SCATTER:
			return __MPI_REDUCE_SCATTER__BARRIER;
		case __MPI_SCATTER:
			return __MPI_SCATTER__BARRIER;
		case __MPI_SCATTERV:
			return __MPI_SCATTERV__BARRIER;
		case __MPI_NEIGHBOR_ALLGATHER:
			return __MPI_NEIGHBOR_ALLGATHER__BARRIER;
		case __MPI_NEIGHBOR_ALLGATHERV:
			return __MPI_NEIGHBOR_ALLGATHERV__BARRIER;
		case __MPI_NEIGHBOR_ALLTOALL:
			return __MPI_NEIGHBOR_ALLTOALL__BARRIER;
		case __MPI_NEIGHBOR_ALLTOALLV:
			return __MPI_NEIGHBOR_ALLTOALLV__BARRIER;
		case __MPI_NEIGHBOR_ALLTOALLW:
			return __MPI_NEIGHBOR_ALLTOALLW__BARRIER;
		case __MPI_SENDRECV:
			return __MPI_SENDRECV__BARRIER;
		case __MPI_SENDRECV_REPLACE:
			return __MPI_SENDRECV_REPLACE__BARRIER;
	}
	return NO_MPI;
}

static MPI_Type_t is_send_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_SEND:
			return __MPI_SEND__BARRIER;
		case __MPI_SSEND:
			return __MPI_SSEND__BARRIER;
		case __MPI_BSEND:
			return __MPI_BSEND__BARRIER;
		case __MPI_RSEND:
			return __MPI_RSEND__BARRIER;
	}
	return NO_MPI;
}

static MPI_Type_t is_recv_or_probe_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_RECV:
			return __MPI_RECV__BARRIER;
		case __MPI_PROBE:
			return __MPI_PROBE__BARRIER;
	}	
	return NO_MPI;
}

static int is_async_p2p(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_ISEND:
			return TRUE;
		case __MPI_ISSEND:
			return TRUE;
		case __MPI_IRSEND:
			return TRUE;
		case __MPI_IBSEND:
			return TRUE;
		case __MPI_IRECV:
			return TRUE;
	}
	return FALSE;
}

HIDDEN void eam_slack_start_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(is_wait_mpi(mpi_type))
	{
		flag_eam_slack = FALSE;
		if(cntd->eam_timeout > 0)
			start_timer();
		else
			eam_slack_callback();
	}
	else if(is_collective_barrier(mpi_type) != NO_MPI)
	{
		MPI_Type_t type = is_collective_barrier(mpi_type);

		event_sample_start(type);

		flag_eam_slack = FALSE;
		if(cntd->eam_timeout > 0)
			start_timer();
		else
			eam_slack_callback();

		PMPI_Barrier(comm);

		if(cntd->eam_timeout > 0)
			reset_timer();

		if(flag_eam_slack)
		{
			set_max_pstate();
			flag_eam_slack = FALSE;

			event_sample_end(type, TRUE);
		}
		else
			event_sample_end(type, FALSE);
	}
	else if(is_send_barrier(mpi_type) != NO_MPI)
	{
		int send_buff;
		MPI_Request send_request;
		MPI_Status send_status;
		MPI_Type_t type = is_send_barrier(mpi_type);

		event_sample_start(type);
	
		flag_eam_slack = FALSE;
		if(cntd->eam_timeout > 0)
			start_timer();
		else
			eam_slack_callback();

		PMPI_Issend(&send_buff, 0, MPI_INT, addr, 0, comm, &send_request);
		PMPI_Wait(&send_request, &send_status);

		if(cntd->eam_timeout > 0)
			reset_timer();

		if(flag_eam_slack)
		{
			set_max_pstate();
			flag_eam_slack = FALSE;

			event_sample_end(type, TRUE);
		}
		else
			event_sample_end(type, FALSE);
	}
	else if(is_recv_or_probe_barrier(mpi_type) != NO_MPI)
	{
		int recv_buff;
		MPI_Request recv_request;
		MPI_Status recv_status;
		MPI_Type_t type = is_recv_or_probe_barrier(mpi_type);
		static int probe_flag = FALSE;

		event_sample_start(type);

		if(!probe_flag)
		{
			flag_eam_slack = FALSE;
			if(cntd->eam_timeout > 0)
				start_timer();
			else
				eam_slack_callback();
		
			PMPI_Irecv(&recv_buff, 0, MPI_INT, addr, 0, comm, &recv_request);
			PMPI_Wait(&recv_request, &recv_status);

			if(cntd->eam_timeout > 0)
				reset_timer();

			if(flag_eam_slack)
			{
				set_max_pstate();
				flag_eam_slack = FALSE;

				event_sample_end(type, TRUE);
			}
			else
				event_sample_end(type, FALSE);
		}
		else
			event_sample_end(type, FALSE);

		if(mpi_type == __MPI_PROBE)
			probe_flag = TRUE;
		else
			probe_flag = FALSE;
	}
	else if(is_async_p2p(mpi_type))
	{
		int barrier_buf;
		MPI_Request barrier_request;
		MPI_Status status;
		int flag;

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
			case __MPI_IPROBE:
				PMPI_Iprobe(addr, 0, comm, &flag, &status);
				break;
			default:
				fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> The MPI type '%s' is not handled!\n", 
					cntd->node.hostname, cntd->rank->world_rank, mpi_type_str[mpi_type]);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				break;
		}
	}
}

HIDDEN int eam_slack_end_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(is_wait_mpi(mpi_type))
	{
		if(cntd->eam_timeout > 0)
			reset_timer();

		if(flag_eam_slack)
		{
			set_max_pstate();
			flag_eam_slack = FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

HIDDEN void eam_slack_init()
{
	// Init power manager
	pm_init();

	// Initialization of timer
	if(cntd->eam_timeout > 0)
		init_timer(eam_slack_callback);
}

HIDDEN void eam_slack_finalize()
{
	// Finalize timer
	if(cntd->eam_timeout > 0)
		finalize_timer();

	// Finalize power manager
	pm_finalize();
}
