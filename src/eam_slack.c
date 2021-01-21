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

static int flag_eam = FALSE;

static int is_wait_mpi(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
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
		// Finalization
		case __MPI_FINALIZE:
			return TRUE;
			break;
	}
}

static int is_collective_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_COMM_SPLIT:
			return TRUE;
			break;
		case __MPI_COMM_SPLIT_TYPE:
			return TRUE;
			break;
		case __MPI_ALLGATHER:
			return TRUE;
			break;
		case __MPI_ALLGATHERV:
			return TRUE;
			break;
		case __MPI_ALLREDUCE:
			return TRUE;
			break;
		case __MPI_ALLTOALL:
			return TRUE;
			break;
		case __MPI_ALLTOALLV:
			return TRUE;
			break;
		case __MPI_ALLTOALLW:
			return TRUE;
			break;
		case __MPI_BCAST:
			return TRUE;
			break;
		case __MPI_EXSCAN:
			return TRUE;
			break;
		case __MPI_SCAN:
			return TRUE;
			break;
		case __MPI_GATHER:
			return TRUE;
			break;
		case __MPI_GATHERV:
			return TRUE;
			break;
		case __MPI_REDUCE:
			return TRUE;
			break;
		case __MPI_REDUCE_SCATTER:
			return TRUE;
			break;
		case __MPI_SCATTER:
			return TRUE;
			break;
		case __MPI_SCATTERV:
			return TRUE;
			break;
		case __MPI_NEIGHBOR_ALLGATHER:
			return TRUE;
			break;
		case __MPI_NEIGHBOR_ALLGATHERV:
			return TRUE;
			break;
		case __MPI_NEIGHBOR_ALLTOALL:
			return TRUE;
			break;
		case __MPI_NEIGHBOR_ALLTOALLV:
			return TRUE;
			break;
		case __MPI_NEIGHBOR_ALLTOALLW:
			return TRUE;
			break;
		case __MPI_SENDRECV:
			return TRUE;
			break;
		case __MPI_SENDRECV_REPLACE:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}

static int is_send_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_SEND:
			return TRUE;
			break;
		case __MPI_SSEND:
			return TRUE;
			break;
		case __MPI_BSEND:
			return TRUE;
			break;
		case __MPI_RSEND:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}

static int is_recv_barrier(MPI_Type_t mpi_type)
{
	switch(mpi_type)
	{
		case __MPI_RECV:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}	
}

static void eam_slack_callback(int signum)
{
	flag_eam = TRUE;
	if(!cntd->no_freq)
		set_min_pstate();
}

static void start_timer()
{
	struct itimerval timer = {{0}};
	timer.it_value.tv_usec = (unsigned long) cntd->timeout;
	setitimer(ITIMER_REAL, &timer, NULL);
}

static void reset_timer()
{
	struct itimerval timer = {{0}};
	setitimer(ITIMER_REAL, &timer, NULL);

	// Set maximum frequency if timer is expired
	if(flag_eam)
	{
		if(!cntd->no_freq)
			set_max_pstate();
		flag_eam = FALSE;
	}
}

void eam_slack_start_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(is_wait_mpi(mpi_type))
	{
		start_timer();
	}
	else if(is_collective_barrier(mpi_type))
	{
		start_timer();
		PMPI_Barrier(comm);
		reset_timer();
	}
	else if(is_send_barrier(mpi_type))
	{
		int send_buff;
		MPI_Request send_request;
		MPI_Status send_status;

		start_timer();
		PMPI_Issend(&send_buff, 0, MPI_INT, addr, 0, comm, &send_request);
		PMPI_Wait(&send_request, &send_status);
		reset_timer();
	}
	else if(is_recv_barrier(mpi_type))
	{
		int recv_buff;
		MPI_Request recv_request;
		MPI_Status recv_status;

		start_timer();
		PMPI_Irecv(&recv_buff, 0, MPI_INT, addr, 0, comm, &recv_request);
		PMPI_Wait(&recv_request, &recv_status);
		reset_timer();
	}
	else
	{
		int barrier_buf;
		MPI_Request barrier_request;

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
				fprintf(stderr, "Error: <countdown> The MPI type '%s' is not handled!\n", mpi_type_str[mpi_type]);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				break;
		}
	}
}

void eam_slack_end_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(is_wait_mpi(mpi_type))
		reset_timer();
}

void eam_slack_init()
{
	struct sigaction sa = {{0}};

	if(!cntd->no_freq)
	{
		// Init power manager
		pm_init();

		// Set maximum p-state
		set_max_pstate();
	}

	// Install timer_handler as the signal handler for SIGALRM.
	sa.sa_handler = &eam_slack_callback;
	sigaction(SIGALRM, &sa, NULL);
}

void eam_slack_finalize()
{
	reset_timer();

	if(!cntd->no_freq)
	{
		// Set maximum system p-state
		set_pstate(cntd->sys_pstate[MAX]);

		// Finalize power manager
		pm_finalize();
	}
}