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

int MPI_Init(int *argc, char ***argv)
{
	int err = PMPI_Init(argc, argv);
	start_cntd();
	call_start(__MPI_INIT, MPI_COMM_WORLD, MPI_NONE);
	call_end(__MPI_INIT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
	int err = PMPI_Init_thread(argc, argv, required, provided);
	start_cntd();
	call_start(__MPI_INIT_THREAD, MPI_COMM_WORLD, MPI_NONE);
    call_end(__MPI_INIT_THREAD, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Finalize(void)
{
	call_start(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	PMPI_Barrier(MPI_COMM_WORLD);

	call_end(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	stop_cntd();

	return PMPI_Finalize();
}

#ifndef DISABLE_PROFILING_MPI

int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLGATHER, comm, MPI_ALL);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, &recvcount, &recvtype, MPI_ALL);
	int err = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_ALLGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLGATHERV, comm, MPI_ALLV);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, recvcounts, &recvtype, MPI_ALLV);
	int err = PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
	call_end(__MPI_ALLGATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_ALLREDUCE, comm, MPI_NONE);
	add_network(comm, &count, &datatype, MPI_ALL, &count, &datatype, MPI_ALL);
	int err = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
	call_end(__MPI_ALLREDUCE, comm, MPI_NONE);
	return err;
}

int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLTOALL, comm, MPI_ALL);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, &recvcount, &recvtype, MPI_ALL);
	int err = PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_ALLTOALL, comm, MPI_ALL);
	return err;
}

int MPI_Alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLTOALLV, comm, MPI_ALLV);
	add_network(comm, sendcounts, &sendtype, MPI_ALLV, recvcounts, &recvtype, MPI_ALLV);
	int err = PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm);
	call_end(__MPI_ALLTOALLV, comm, MPI_ALLV);
	return err;
}

int MPI_Alltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm)
{
	call_start(__MPI_ALLTOALLW, comm, MPI_ALLW);
	add_network(comm, sendcounts, (MPI_Datatype*) sendtypes, MPI_ALLW, recvcounts, (MPI_Datatype*) recvtypes, MPI_ALLW);
	int err = PMPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm);
	call_end(__MPI_ALLTOALLW, comm, MPI_ALLW);
	return err;
}

int MPI_Barrier(MPI_Comm comm)
{
	call_start(__MPI_BARRIER, comm, MPI_NONE);
	int err = PMPI_Barrier(comm);
	call_end(__MPI_BARRIER, comm, MPI_NONE);
	return err;
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
	call_start(__MPI_BCAST, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, &count, &datatype, MPI_ALL, NULL, &datatype, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, root);
	int err = PMPI_Bcast(buffer, count, datatype, root, comm);
	call_end(__MPI_BCAST, comm, MPI_ALL);
	return err;
}

int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_SPLIT, comm, MPI_NONE);
	int err = PMPI_Comm_split(comm, color, key, newcomm);
	call_end(__MPI_COMM_SPLIT, comm, MPI_NONE);
	return err;
}

int MPI_Comm_split_type(MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_SPLIT_TYPE, comm, MPI_NONE);
	int err = PMPI_Comm_split_type(comm, split_type, key, info, newcomm);
	call_end(__MPI_COMM_SPLIT_TYPE, comm, MPI_NONE);
	return err;
}

int MPI_Exscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_EXSCAN, comm, MPI_NONE);
	int err = PMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
	call_end(__MPI_EXSCAN, comm, MPI_NONE);
	return err;
}

int MPI_File_sync(MPI_File fh)
{
	call_start(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_sync(fh);
	call_end(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_GATHER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, MPI_ALL);
	else
		add_network(comm, &sendcount, &sendtype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_GATHER, comm, MPI_ALL);
	return err;
}

int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_GATHERV, comm, MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, recvcounts, &recvtype, MPI_ALLV);
	else
		add_network(comm, &sendcount, &sendtype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
	call_end(__MPI_GATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Neighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLGATHER, comm, MPI_ALL);
	int err = PMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Neighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
	int err = PMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
	return err;
}


int MPI_Neighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLTOALL, comm, MPI_ALL);
	int err = PMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLTOALL, comm, MPI_ALL);
	return err;
}


int MPI_Neighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLTOALLV, comm, MPI_ALLV);
	int err = PMPI_Neighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLTOALLV, comm, MPI_ALLV);
	return err;
}


int MPI_Neighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLTOALLW, comm, MPI_ALLW);
	int err = PMPI_Neighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm);
	call_end(__MPI_NEIGHBOR_ALLTOALLW, comm, MPI_ALLW);
	return err;
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
	call_start(__MPI_REDUCE, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, MPI_ALL);
	else
		add_network(comm, &count, &datatype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
	call_end(__MPI_REDUCE, comm, MPI_ALL);
	return err;
}

int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_REDUCE_SCATTER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == 0)
		add_network(comm, recvcounts, &datatype, MPI_ALLV, recvcounts, &datatype, MPI_ALLV);
	else
		add_network(comm, &recvcounts[my_rank], &datatype, 0, &recvcounts[my_rank], &datatype, 0);
	int err = PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
	call_end(__MPI_REDUCE_SCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Scan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_SCAN, comm, MPI_NONE);
	int err = PMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);
	call_end(__MPI_SCAN, comm, MPI_NONE);
	return err;
}

int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_SCATTER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, &sendcount, &sendtype, MPI_ALL, NULL, NULL, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, root);
	int err = PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_SCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Scatterv(const void *sendbuf, const int sendcounts[], const int displs[], MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_SCATTERV, comm, MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, sendcounts, &sendtype, MPI_ALLV, NULL, NULL, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, root);
	int err = PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_SCATTERV, comm, MPI_ALLV);
	return err;
}

int MPI_Waitall(int count, MPI_Request array_of_requests[], MPI_Status *array_of_statuses)
{
	call_start(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Waitall(count, array_of_requests, array_of_statuses);
	call_end(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Waitany(int count, MPI_Request array_of_requests[], int *index, MPI_Status *status)
{
	call_start(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Waitany(count, array_of_requests, index, status);
	call_end(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
	call_start(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Wait(request, status);
	call_end(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Waitsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[])
{
	call_start(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
	call_end(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_flush(int rank, MPI_Win win)
{
	call_start(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_flush(rank, win);
	call_end(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_flush_all(MPI_Win win)
{
	call_start(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_flush_all(win);
	call_end(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_flush_local(int rank, MPI_Win win)
{
	call_start(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_flush_local(rank, win);
	call_end(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_flush_local_all(MPI_Win win)
{
	call_start(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_flush_local_all(win);
	call_end(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win)
{
	call_start(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_lock(lock_type, rank, assert, win);
	call_end(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_lock_all(int assert, MPI_Win win)
{
	call_start(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_lock_all(assert, win);
	call_end(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_sync(MPI_Win win)
{
	call_start(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_sync(win);
	call_end(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_wait(MPI_Win win)
{
	call_start(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_wait(win);
	call_end(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

#ifndef DISABLE_P2P_MPI

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_SEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Send(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_SEND, comm, dest);
	return err;
}

int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_SENDRECV, comm, MPI_NONE);
	add_network(comm, &sendcount, &sendtype, dest, &recvcount, &recvtype, source);
	int err = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
	call_end(__MPI_SENDRECV, comm, MPI_NONE);
	return err;
}

int MPI_Sendrecv_replace(void * buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_SENDRECV_REPLACE, comm, MPI_NONE);
	add_network(comm, &count, &datatype, dest, &count, &datatype, source);
	int err = PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
	call_end(__MPI_SENDRECV_REPLACE, comm, MPI_NONE);
	return err;
}

int MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_SSEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_SSEND, comm, dest);
	return err;
}

int MPI_Bsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_BSEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_BSEND, comm, dest);
	return err;
}

int MPI_Rsend(const void *ibuf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_RSEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Rsend(ibuf, count, datatype, dest, tag, comm);
	call_end(__MPI_RSEND, comm, dest);
	return err;
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_RECV, comm, source);
	int err = PMPI_Recv(buf, count, datatype, source, tag, comm, status);
	call_end(__MPI_RECV, comm, source);
	return err;
}

int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_PROBE, comm, source);
	int err = PMPI_Probe(source, tag, comm, status);
    call_end(__MPI_PROBE, comm, source);
	return err;
}

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_ISEND, comm, dest);
	return err;
}

int MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISSEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_ISSEND, comm, dest);
	return err;
}

int MPI_Irsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IRSEND, comm, dest);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_IRSEND, comm, dest);
	return err;
}

int MPI_Ibsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IBSEND, comm, MPI_NONE);
	add_network(comm, &count, &datatype, dest, NULL, NULL, MPI_NONE);
	int err = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_IBSEND, comm, MPI_NONE);
	return err;
}

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IRECV, comm, source);
	add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, source);
	int err = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
	call_end(__MPI_IRECV, comm, source);
	return err;
}

int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)
{
	call_start(__MPI_IPROBE, comm, MPI_NONE);
	int err = PMPI_Iprobe(source, tag, comm, flag, status);
    call_end(__MPI_IPROBE, comm, MPI_NONE);
	return err;
}

#endif

#ifndef DISABLE_ACCESSORY_MPI

int MPI_Accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win)
{
	call_start(__MPI_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win);
    call_end(__MPI_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Add_error_class(int *errorclass)
{
	call_start(__MPI_ADD_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Add_error_class(errorclass);
    call_end(__MPI_ADD_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Add_error_code(int errorclass, int *errorcode)
{
	call_start(__MPI_ADD_ERROR_CODE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Add_error_code(errorclass, errorcode);
    call_end(__MPI_ADD_ERROR_CODE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Add_error_string(int errorcode, const char *string)
{
	call_start(__MPI_ADD_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Add_error_string(errorcode, string);
    call_end(__MPI_ADD_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Iallgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLGATHER, comm, MPI_ALL);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, &recvcount, &recvtype, MPI_ALL);
	int err = PMPI_Iallgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request);
    call_end(__MPI_IALLGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Iallgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLGATHERV, comm, MPI_ALLV);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, recvcounts, &recvtype, MPI_ALLV);
	int err = PMPI_Iallgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request);
    call_end(__MPI_IALLGATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr)
{
	call_start(__MPI_ALLOC_MEM, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Alloc_mem(size, info, baseptr);
    call_end(__MPI_ALLOC_MEM, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Iallreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLREDUCE, comm, MPI_NONE);
	add_network(comm, &count, &datatype, MPI_ALL, &count, &datatype, MPI_ALL);
	int err = PMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request);
    call_end(__MPI_IALLREDUCE, comm, MPI_NONE);
	return err;
}

int MPI_Ialltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLTOALL, comm, MPI_ALL);
	add_network(comm, &sendcount, &sendtype, MPI_ALL, &recvcount, &recvtype, MPI_ALL);
	int err = PMPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request);
    call_end(__MPI_IALLTOALL, comm, MPI_ALL);
	return err;
}

int MPI_Ialltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLTOALLV, comm, MPI_ALLV);
	add_network(comm, sendcounts, &sendtype, MPI_ALLV, recvcounts, &recvtype, MPI_ALLV);
	int err = PMPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request);
    call_end(__MPI_IALLTOALLV, comm, MPI_ALLV);
	return err;
}

int MPI_Ialltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IALLTOALLW, comm, MPI_ALLW);
	add_network(comm, sendcounts, (MPI_Datatype*) sendtypes, MPI_ALLW, recvcounts, (MPI_Datatype*) recvtypes, MPI_ALLW);
	int err = PMPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request);
    call_end(__MPI_IALLTOALLW, comm, MPI_ALLW);
	return err;
}

int MPI_Ibarrier(MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IBARRIER, comm, MPI_NONE);
	int err = PMPI_Ibarrier(comm, request);
    call_end(__MPI_IBARRIER, comm, MPI_NONE);
	return err;
}

int MPI_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IBCAST, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, &count, &datatype, MPI_ALL, NULL, NULL, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, root);
	int err = PMPI_Ibcast(buffer, count, datatype, root, comm, request);
    call_end(__MPI_IBCAST, comm, MPI_ALL);
	return err;
}

int MPI_Bsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_BSEND_INIT, comm, dest);
	int err = PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
   	call_end(__MPI_BSEND_INIT, comm, dest);
	return err;
}

int MPI_Buffer_attach(void *buffer, int size)
{
	call_start(__MPI_BUFFER_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Buffer_attach(buffer, size);
    call_end(__MPI_BUFFER_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Buffer_detach(void *buffer, int *size)
{
	call_start(__MPI_BUFFER_DETACH, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Buffer_detach(buffer, size);
    call_end(__MPI_BUFFER_DETACH, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Cancel(MPI_Request *request)
{
	call_start(__MPI_CANCEL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Cancel(request);
    call_end(__MPI_CANCEL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[])
{
	call_start(__MPI_CART_COORDS, comm, MPI_NONE);
	int err = PMPI_Cart_coords(comm, rank, maxdims, coords);
    call_end(__MPI_CART_COORDS, comm, MPI_NONE);
	return err;
}

int MPI_Cart_create(MPI_Comm old_comm, int ndims, const int dims[], const int periods[], int reorder, MPI_Comm *comm_cart)
{
	call_start(__MPI_CART_CREATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Cart_create(old_comm, ndims, dims, periods, reorder, comm_cart);
    call_end(__MPI_CART_CREATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Cart_get(MPI_Comm comm, int maxdims, int dims[], int periods[], int coords[])
{
	call_start(__MPI_CART_GET, comm, MPI_NONE);
	int err = PMPI_Cart_get(comm, maxdims, dims, periods, coords);
    call_end(__MPI_CART_GET, comm, MPI_NONE);
	return err;
}

int MPI_Cart_map(MPI_Comm comm, int ndims, const int dims[], const int periods[], int *newrank)
{
	call_start(__MPI_CART_MAP, comm, MPI_NONE);
	int err = PMPI_Cart_map(comm, ndims, dims, periods, newrank);
    call_end(__MPI_CART_MAP, comm, MPI_NONE);
	return err;
}

int MPI_Cart_rank(MPI_Comm comm, const int coords[], int *rank)
{
	call_start(__MPI_CART_RANK, comm, MPI_NONE);
	int err = PMPI_Cart_rank(comm, coords, rank);
    call_end(__MPI_CART_RANK, comm, MPI_NONE);
	return err;
}

int MPI_Cart_shift(MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest)
{
	call_start(__MPI_CART_SHIFT, comm, MPI_NONE);
	int err = PMPI_Cart_shift(comm, direction, disp, rank_source, rank_dest);
    call_end(__MPI_CART_SHIFT, comm, MPI_NONE);
	return err;
}

int MPI_Cart_sub(MPI_Comm comm, const int remain_dims[], MPI_Comm *new_comm)
{
	call_start(__MPI_CART_SUB, comm, MPI_NONE);
	int err = PMPI_Cart_sub(comm, remain_dims, new_comm);
    call_end(__MPI_CART_SUB, comm, MPI_NONE);
	return err;
}

int MPI_Cartdim_get(MPI_Comm comm, int *ndims)
{
	call_start(__MPI_CARTDIM_GET, comm, MPI_NONE);
	int err = PMPI_Cartdim_get(comm, ndims);
    call_end(__MPI_CARTDIM_GET, comm, MPI_NONE);
	return err;
}

int MPI_Close_port(const char *port_name)
{
	call_start(__MPI_CLOSE_PORT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Close_port(port_name);
    call_end(__MPI_CLOSE_PORT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_accept(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_ACCEPT, comm, MPI_NONE);
	int err = PMPI_Comm_accept(port_name, info, root, comm, newcomm);
    call_end(__MPI_COMM_ACCEPT, comm, MPI_NONE);
	return err;
}

int MPI_Comm_call_errhandler(MPI_Comm comm, int errorcode)
{
	call_start(__MPI_COMM_CALL_ERRHANDLER, comm, MPI_NONE);
	int err = PMPI_Comm_call_errhandler(comm, errorcode);
    call_end(__MPI_COMM_CALL_ERRHANDLER, comm, MPI_NONE);
	return err;
}

int MPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result)
{
	call_start(__MPI_COMM_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_compare(comm1, comm2, result);
    call_end(__MPI_COMM_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_connect(const char *port_name, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_CONNECT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_connect(port_name, info, root, comm, newcomm);
    call_end(__MPI_COMM_CONNECT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_create_errhandler(MPI_Comm_errhandler_function *function, MPI_Errhandler *errhandler)
{
	call_start(__MPI_COMM_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_create_errhandler(function, errhandler);
    call_end(__MPI_COMM_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_create_keyval(MPI_Comm_copy_attr_function *comm_copy_attr_fn, MPI_Comm_delete_attr_function *comm_delete_attr_fn, int *comm_keyval, void *extra_state)
{
	call_start(__MPI_COMM_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state);
    call_end(__MPI_COMM_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_create_group(MPI_Comm comm, MPI_Group group, int tag, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_CREATE_GROUP, comm, MPI_NONE);
	int err = PMPI_Comm_create_group(comm, group, tag, newcomm);
    call_end(__MPI_COMM_CREATE_GROUP, comm, MPI_NONE);
	return err;
}

int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_CREATE, comm, MPI_NONE);
	int err = PMPI_Comm_create(comm, group, newcomm);
    call_end(__MPI_COMM_CREATE, comm, MPI_NONE);
	return err;
}

int MPI_Comm_delete_attr(MPI_Comm comm, int comm_keyval)
{
	call_start(__MPI_COMM_DELETE_ATTR, comm, MPI_NONE);
	int err = PMPI_Comm_delete_attr(comm, comm_keyval);
    call_end(__MPI_COMM_DELETE_ATTR, comm, MPI_NONE);
	return err;
}

int MPI_Comm_disconnect(MPI_Comm *comm)
{
	call_start(__MPI_COMM_DISCONNECT, *comm, MPI_NONE);
	int err = PMPI_Comm_disconnect(comm);
    call_end(__MPI_COMM_DISCONNECT, *comm, MPI_NONE);
	return err;
}

int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_DUP, comm, MPI_NONE);
	int err = PMPI_Comm_dup(comm, newcomm);
    call_end(__MPI_COMM_DUP, comm, MPI_NONE);
	return err;
}

int MPI_Comm_idup(MPI_Comm comm, MPI_Comm *newcomm, MPI_Request *request)
{
	call_start(__MPI_COMM_IDUP, comm, MPI_NONE);
	int err = PMPI_Comm_idup(comm, newcomm, request);
    call_end(__MPI_COMM_IDUP, comm, MPI_NONE);
	return err;
}

int MPI_Comm_dup_with_info(MPI_Comm comm, MPI_Info info, MPI_Comm *newcomm)
{
	call_start(__MPI_COMM_DUP_WITH_INFO, comm, MPI_NONE);
	int err = PMPI_Comm_dup_with_info(comm, info, newcomm);
    call_end(__MPI_COMM_DUP_WITH_INFO, comm, MPI_NONE);
	return err;
}

int MPI_Comm_free_keyval(int *comm_keyval)
{
	call_start(__MPI_COMM_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_free_keyval(comm_keyval);
    call_end(__MPI_COMM_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_free(MPI_Comm *comm)
{
	call_start(__MPI_COMM_FREE, *comm, MPI_NONE);
	int err = PMPI_Comm_free(comm);
    call_end(__MPI_COMM_FREE, *comm, MPI_NONE);
	return err;
}

int MPI_Comm_get_attr(MPI_Comm comm, int comm_keyval, void *attribute_val, int *flag)
{
	call_start(__MPI_COMM_GET_ATTR, comm, MPI_NONE);
	int err = PMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag);
    call_end(__MPI_COMM_GET_ATTR, comm, MPI_NONE);
	return err;
}

int MPI_Dist_graph_create(MPI_Comm comm_old, int n, const int nodes[], const int degrees[], const int targets[], const int weights[], MPI_Info info, int reorder, MPI_Comm *newcomm)
{
	call_start(__MPI_DIST_GRAPH_CREATE, comm_old, MPI_NONE);
	int err = PMPI_Dist_graph_create(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm);
    call_end(__MPI_DIST_GRAPH_CREATE, comm_old, MPI_NONE);
	return err;
}

int MPI_Dist_graph_create_adjacent(MPI_Comm comm_old, int indegree, const int sources[], const int sourceweights[], int outdegree, const int destinations[], const int destweights[], MPI_Info info, int reorder, MPI_Comm *comm_dist_graph)
{
	call_start(__MPI_DIST_GRAPH_CREATE_ADJACENT, comm_old, MPI_NONE);
	int err = PMPI_Dist_graph_create_adjacent(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph);
    call_end(__MPI_DIST_GRAPH_CREATE_ADJACENT, comm_old, MPI_NONE);
	return err;
}

int MPI_Dist_graph_neighbors(MPI_Comm comm, int maxindegree, int sources[], int sourceweights[], int maxoutdegree, int destinations[], int destweights[])
{
	call_start(__MPI_DIST_GRAPH_NEIGHBORS, comm, MPI_NONE);
	int err = PMPI_Dist_graph_neighbors(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights);
    call_end(__MPI_DIST_GRAPH_NEIGHBORS, comm, MPI_NONE);
	return err;
}

int MPI_Dist_graph_neighbors_count(MPI_Comm comm, int *inneighbors, int *outneighbors, int *weighted)
{
	call_start(__MPI_DIST_GRAPH_NEIGHBORS_COUNT, comm, MPI_NONE);
	int err = PMPI_Dist_graph_neighbors_count(comm, inneighbors, outneighbors, weighted);
    call_end(__MPI_DIST_GRAPH_NEIGHBORS_COUNT, comm, MPI_NONE);
	return err;
}

int MPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *erhandler)
{
	call_start(__MPI_COMM_GET_ERRHANDLER, comm, MPI_NONE);
	int err = PMPI_Comm_get_errhandler(comm, erhandler);
    call_end(__MPI_COMM_GET_ERRHANDLER, comm, MPI_NONE);
	return err;
}

int MPI_Comm_get_info(MPI_Comm comm, MPI_Info *info_used)
{
	call_start(__MPI_COMM_GET_INFO, comm, MPI_NONE);
	int err = PMPI_Comm_get_info(comm, info_used);
    call_end(__MPI_COMM_GET_INFO, comm, MPI_NONE);
	return err;
}

int MPI_Comm_get_name(MPI_Comm comm, char *comm_name, int *resultlen)
{
	call_start(__MPI_COMM_GET_NAME, comm, MPI_NONE);
	int err = PMPI_Comm_get_name(comm, comm_name, resultlen);
    call_end(__MPI_COMM_GET_NAME, comm, MPI_NONE);
	return err;
}

int MPI_Comm_get_parent(MPI_Comm *parent)
{
	call_start(__MPI_COMM_GET_PARENT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_get_parent(parent);
    call_end(__MPI_COMM_GET_PARENT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_group(MPI_Comm comm, MPI_Group *group)
{
	call_start(__MPI_COMM_GROUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_group(comm, group);
    call_end(__MPI_COMM_GROUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_join(int fd, MPI_Comm *intercomm)
{
	call_start(__MPI_COMM_JOIN, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_join(fd, intercomm);
    call_end(__MPI_COMM_JOIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
	call_start(__MPI_COMM_RANK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_rank(comm, rank);
    call_end(__MPI_COMM_RANK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group)
{
	call_start(__MPI_COMM_REMOTE_GROUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_remote_group(comm, group);
    call_end(__MPI_COMM_REMOTE_GROUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_remote_size(MPI_Comm comm, int *size)
{
	call_start(__MPI_COMM_REMOTE_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_remote_size(comm, size);
    call_end(__MPI_COMM_REMOTE_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_set_attr(MPI_Comm comm, int comm_keyval, void *attribute_val)
{
	call_start(__MPI_COMM_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_set_attr(comm, comm_keyval, attribute_val);
    call_end(__MPI_COMM_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler)
{
	call_start(__MPI_COMM_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_set_errhandler(comm, errhandler);
    call_end(__MPI_COMM_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_set_info(MPI_Comm comm, MPI_Info info)
{
	call_start(__MPI_COMM_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_set_info(comm, info);
    call_end(__MPI_COMM_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_set_name(MPI_Comm comm, const char *comm_name)
{
	call_start(__MPI_COMM_SET_NAME, comm, MPI_NONE);
	int err = PMPI_Comm_set_name(comm, comm_name);
    call_end(__MPI_COMM_SET_NAME, comm, MPI_NONE);
	return err;
}

int MPI_Comm_size(MPI_Comm comm, int *size)
{
	call_start(__MPI_COMM_SIZE, comm, MPI_NONE);
	int err = PMPI_Comm_size(comm, size);
    call_end(__MPI_COMM_SIZE, comm, MPI_NONE);
	return err;
}

int MPI_Comm_spawn(const char *command, char *argv[], int maxprocs, MPI_Info info, int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[])
{
	call_start(__MPI_COMM_SPAWN, comm, MPI_NONE);
	int err = PMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes);
    call_end(__MPI_COMM_SPAWN, comm, MPI_NONE);
	return err;
}

int MPI_Comm_spawn_multiple(int count, char *array_of_commands[], char **array_of_argv[], const int array_of_maxprocs[], const MPI_Info array_of_info[], int root, MPI_Comm comm, MPI_Comm *intercomm, int array_of_errcodes[])
{
	call_start(__MPI_COMM_SPAWN_MULTIPLE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes);
    call_end(__MPI_COMM_SPAWN_MULTIPLE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Comm_test_inter(MPI_Comm comm, int *flag)
{
	call_start(__MPI_COMM_TEST_INTER, comm, MPI_NONE);
	int err = PMPI_Comm_test_inter(comm, flag);
    call_end(__MPI_COMM_TEST_INTER, comm, MPI_NONE);
	return err;
}

int MPI_Compare_and_swap(const void *origin_addr, const void *compare_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Win win)
{
	call_start(__MPI_COMPARE_AND_SWAP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Compare_and_swap(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win);
    call_end(__MPI_COMPARE_AND_SWAP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Dims_create(int nnodes, int ndims, int dims[])
{
	call_start(__MPI_DIMS_CREATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Dims_create(nnodes, ndims, dims);
    call_end(__MPI_DIMS_CREATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Errhandler_free(MPI_Errhandler *errhandler)
{
	call_start(__MPI_ERRHANDLER_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Errhandler_free(errhandler);
    call_end(__MPI_ERRHANDLER_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Error_class(int errorcode, int *errorclass)
{
	call_start(__MPI_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Error_class(errorcode, errorclass);
    call_end(__MPI_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Error_string(int errorcode, char *string, int *resultlen)
{
	call_start(__MPI_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Error_string(errorcode, string, resultlen);
    call_end(__MPI_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Fetch_and_op(const void *origin_addr, void *result_addr, MPI_Datatype datatype, int target_rank, MPI_Aint target_disp, MPI_Op op, MPI_Win win)
{
	call_start(__MPI_FETCH_AND_OP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win);
    call_end(__MPI_FETCH_AND_OP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Iexscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IEXSCAN, comm, MPI_NONE);
	int err = PMPI_Iexscan(sendbuf, recvbuf, count, datatype, op, comm, request);
    call_end(__MPI_IEXSCAN, comm, MPI_NONE);
	return err;
}

int MPI_File_call_errhandler(MPI_File fh, int errorcode)
{
	call_start(__MPI_FILE_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_call_errhandler(fh, errorcode);
    call_end(__MPI_FILE_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_create_errhandler(MPI_File_errhandler_function *function, MPI_Errhandler *errhandler)
{
	call_start(__MPI_FILE_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_create_errhandler(function, errhandler);
    call_end(__MPI_FILE_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler)
{
	call_start(__MPI_FILE_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_set_errhandler(file, errhandler);
    call_end(__MPI_FILE_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler)
{
	call_start(__MPI_FILE_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_errhandler(file, errhandler);
    call_end(__MPI_FILE_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_open(MPI_Comm comm, const char *filename, int amode, MPI_Info info, MPI_File *fh)
{
	call_start(__MPI_FILE_OPEN, comm, MPI_NONE);
	int err = PMPI_File_open(comm, filename, amode, info,fh);
    call_end(__MPI_FILE_OPEN, comm, MPI_NONE);
	return err;
}

int MPI_File_close(MPI_File *fh)
{
	call_start(__MPI_FILE_CLOSE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_close(fh);
    call_end(__MPI_FILE_CLOSE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_delete(const char *filename, MPI_Info info)
{
	call_start(__MPI_FILE_DELETE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_delete(filename, info);
    call_end(__MPI_FILE_DELETE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_set_size(MPI_File fh, MPI_Offset size)
{
	call_start(__MPI_FILE_SET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_set_size(fh, size);
    call_end(__MPI_FILE_SET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_preallocate(MPI_File fh, MPI_Offset size)
{
	call_start(__MPI_FILE_PREALLOCATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_preallocate(fh, size);
    call_end(__MPI_FILE_PREALLOCATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_size(MPI_File fh, MPI_Offset *size)
{
	call_start(__MPI_FILE_GET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_size(fh, size);
    call_end(__MPI_FILE_GET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_group(MPI_File fh, MPI_Group *group)
{
	call_start(__MPI_FILE_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_group(fh, group);
    call_end(__MPI_FILE_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_amode(MPI_File fh, int *amode)
{
	call_start(__MPI_FILE_GET_AMODE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_amode(fh, amode);
    call_end(__MPI_FILE_GET_AMODE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_set_info(MPI_File fh, MPI_Info info)
{
	call_start(__MPI_FILE_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_set_info(fh, info);
    call_end(__MPI_FILE_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_info(MPI_File fh, MPI_Info *info_used)
{
	call_start(__MPI_FILE_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_info(fh, info_used);
    call_end(__MPI_FILE_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_set_view(MPI_File fh, MPI_Offset disp, MPI_Datatype etype, MPI_Datatype filetype, const char *datarep, MPI_Info info)
{
	call_start(__MPI_FILE_SET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_set_view(fh, disp, etype, filetype, datarep, info);
    call_end(__MPI_FILE_SET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_view(MPI_File fh, MPI_Offset *disp, MPI_Datatype *etype, MPI_Datatype *filetype, char *datarep)
{
	call_start(__MPI_FILE_GET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_view(fh, disp, etype, filetype, datarep);
    call_end(__MPI_FILE_GET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_AT, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_at(fh, offset, buf, count, datatype, status);
    call_end(__MPI_FILE_READ_AT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_at_all(fh, offset, buf, count, datatype, status);
    call_end(__MPI_FILE_READ_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_at(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_at(fh, offset, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_at_all(fh, offset, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iread_at(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IREAD_AT, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_iread_at(fh, offset, buf, count, datatype, request);
    call_end(__MPI_FILE_IREAD_AT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IWRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_iwrite_at(fh, offset, buf, count, datatype, request);
    call_end(__MPI_FILE_IWRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iread_at_all(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IREAD_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_iread_at_all(fh, offset, buf, count, datatype, request);
    call_end(__MPI_FILE_IREAD_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iwrite_at_all(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IWRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_iwrite_at_all(fh, offset, buf, count, datatype, request);
    call_end(__MPI_FILE_IWRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_READ, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_all(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_READ_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_all(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_all(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iread(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IREAD, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_iread(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IREAD, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iwrite(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IWRITE, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_iwrite(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IWRITE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iread_all(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IREAD_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_iread_all(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IREAD_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iwrite_all(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IWRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_iwrite_all(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IWRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence)
{
	call_start(__MPI_FILE_SEEK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_seek(fh, offset, whence);
    call_end(__MPI_FILE_SEEK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_position(MPI_File fh, MPI_Offset *offset)
{
	call_start(__MPI_FILE_GET_POSITION, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_position(fh, offset);
    call_end(__MPI_FILE_GET_POSITION, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_byte_offset(MPI_File fh, MPI_Offset offset, MPI_Offset *disp)
{
	call_start(__MPI_FILE_GET_BYTE_OFFSET, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_byte_offset(fh, offset, disp);
    call_end(__MPI_FILE_GET_BYTE_OFFSET, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_SHARED, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_shared(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_READ_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_shared(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_shared(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iread_shared(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IREAD_SHARED, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_iread_shared(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IREAD_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_iwrite_shared(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Request *request)
{
	call_start(__MPI_FILE_IWRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_iwrite_shared(fh, buf, count, datatype, request);
    call_end(__MPI_FILE_IWRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_ordered(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_ordered(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_READ_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_ordered(MPI_File fh, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_ordered(fh, buf, count, datatype, status);
    call_end(__MPI_FILE_WRITE_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_seek_shared(MPI_File fh, MPI_Offset offset, int whence)
{
	call_start(__MPI_FILE_SEEK_SHARED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_seek_shared(fh, offset, whence);
    call_end(__MPI_FILE_SEEK_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_position_shared(MPI_File fh, MPI_Offset *offset)
{
	call_start(__MPI_FILE_GET_POSITION_SHARED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_position_shared(fh, offset);
    call_end(__MPI_FILE_GET_POSITION_SHARED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_READ_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_at_all_begin(fh, offset, buf, count, datatype);
    call_end(__MPI_FILE_READ_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_read_at_all_end(fh, buf, status);
    call_end(__MPI_FILE_READ_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_WRITE_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_at_all_begin(fh, offset, buf, count, datatype);
    call_end(__MPI_FILE_WRITE_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_at_all_end(MPI_File fh, const void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_write_at_all_end(fh, buf, status);
    call_end(__MPI_FILE_WRITE_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_all_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_READ_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_all_begin(fh, buf, count, datatype);
    call_end(__MPI_FILE_READ_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_all_end(MPI_File fh, void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_read_all_end(fh, buf, status);
    call_end(__MPI_FILE_READ_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_all_begin(MPI_File fh, const void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_WRITE_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_all_begin(fh, buf, count, datatype);
    call_end(__MPI_FILE_WRITE_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_all_end(MPI_File fh, const void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_write_all_end(fh, buf, status);
    call_end(__MPI_FILE_WRITE_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_ordered_begin(MPI_File fh, void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_READ_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(count, datatype, 0, 0);
    int err = PMPI_File_read_ordered_begin(fh, buf, count, datatype);
    call_end(__MPI_FILE_READ_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_read_ordered_end(MPI_File fh, void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_READ_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_read_ordered_end(fh, buf, status);
    call_end(__MPI_FILE_READ_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_ordered_begin(MPI_File fh, const void *buf, int count, MPI_Datatype datatype)
{
	call_start(__MPI_FILE_WRITE_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	add_file(0, 0, count, datatype);
    int err = PMPI_File_write_ordered_begin(fh, buf, count, datatype);
    call_end(__MPI_FILE_WRITE_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_write_ordered_end(MPI_File fh, const void *buf, MPI_Status *status)
{
	call_start(__MPI_FILE_WRITE_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_write_ordered_end(fh, buf, status);
    call_end(__MPI_FILE_WRITE_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_type_extent(MPI_File fh, MPI_Datatype datatype, MPI_Aint *extent)
{
	call_start(__MPI_FILE_GET_TYPE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_type_extent(fh, datatype, extent);
    call_end(__MPI_FILE_GET_TYPE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_set_atomicity(MPI_File fh, int flag)
{
	call_start(__MPI_FILE_SET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_set_atomicity(fh, flag);
    call_end(__MPI_FILE_SET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_File_get_atomicity(MPI_File fh, int *flag)
{
	call_start(__MPI_FILE_GET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_File_get_atomicity(fh, flag);
    call_end(__MPI_FILE_GET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Finalized(int *flag)
{
	return PMPI_Finalized(flag);
}

int MPI_Free_mem(void *base)
{
	call_start(__MPI_FREE_MEM, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Free_mem(base);
    call_end(__MPI_FREE_MEM, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Igather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IGATHER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, MPI_ALL);
	else
		add_network(comm, &sendcount, &sendtype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request);
    call_end(__MPI_IGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Igatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IGATHERV, comm, MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, recvcounts, &recvtype, MPI_ALLV);
	else
		add_network(comm, &sendcount, &sendtype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Igatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request);
    call_end(__MPI_IGATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Get_address(const void *location, MPI_Aint *address)
{
	call_start(__MPI_GET_ADDRESS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_address(location, address);
    call_end(__MPI_GET_ADDRESS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)
{
	call_start(__MPI_GET_COUNT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_count(status, datatype, count);
    call_end(__MPI_GET_COUNT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_elements(const MPI_Status *status, MPI_Datatype datatype, int *count)
{
	call_start(__MPI_GET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_elements(status, datatype, count);
    call_end(__MPI_GET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_elements_x(const MPI_Status *status, MPI_Datatype datatype, MPI_Count *count)
{
	call_start(__MPI_GET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_elements_x(status, datatype, count);
    call_end(__MPI_GET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win)
{
	call_start(__MPI_GET, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win);
    call_end(__MPI_GET, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win)
{
	call_start(__MPI_GET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win);
    call_end(__MPI_GET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_library_version(char *version, int *resultlen)
{
	call_start(__MPI_GET_LIBRARY_VERSION, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_library_version(version, resultlen);
    call_end(__MPI_GET_LIBRARY_VERSION, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_processor_name(char *name, int *resultlen)
{
	call_start(__MPI_GET_PROCESSOR_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_processor_name(name, resultlen);
    call_end(__MPI_GET_PROCESSOR_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Get_version(int *version, int *subversion)
{
	call_start(__MPI_GET_VERSION, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Get_version(version, subversion);
    call_end(__MPI_GET_VERSION, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Graph_create(MPI_Comm comm_old, int nnodes, const int index[], const int edges[], int reorder, MPI_Comm *comm_graph)
{
	call_start(__MPI_GRAPH_CREATE, comm_old, MPI_NONE);
	int err = PMPI_Graph_create(comm_old, nnodes, index, edges, reorder, comm_graph);
    call_end(__MPI_GRAPH_CREATE, comm_old, MPI_NONE);
	return err;
}

int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int index[], int edges[])
{
	call_start(__MPI_GRAPH_GET, comm, MPI_NONE);
	int err = PMPI_Graph_get(comm, maxindex, maxedges, index, edges);
    call_end(__MPI_GRAPH_GET, comm, MPI_NONE);
	return err;
}

int MPI_Graph_map(MPI_Comm comm, int nnodes, const int index[], const int edges[], int *newrank)
{
	call_start(__MPI_GRAPH_MAP, comm, MPI_NONE);
	int err = PMPI_Graph_map(comm, nnodes, index, edges, newrank);
    call_end(__MPI_GRAPH_MAP, comm, MPI_NONE);
	return err;
}

int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors)
{
	call_start(__MPI_GRAPH_NEIGHBORS_COUNT, comm, MPI_NONE);
	int err = PMPI_Graph_neighbors_count(comm, rank, nneighbors);
    call_end(__MPI_GRAPH_NEIGHBORS_COUNT, comm, MPI_NONE);
	return err;
}

int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int neighbors[])
{
	call_start(__MPI_GRAPH_NEIGHBORS, comm, MPI_NONE);
	int err = PMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);
    call_end(__MPI_GRAPH_NEIGHBORS, comm, MPI_NONE);
	return err;
}

int MPI_Graphdims_get(MPI_Comm comm, int *nnodes, int *nedges)
{
	call_start(__MPI_GRAPHDIMS_GET, comm, MPI_NONE);
	int err = PMPI_Graphdims_get(comm, nnodes, nedges);
    call_end(__MPI_GRAPHDIMS_GET, comm, MPI_NONE);
	return err;
}

int MPI_Grequest_complete(MPI_Request request)
{
	call_start(__MPI_GREQUEST_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Grequest_complete(request);
    call_end(__MPI_GREQUEST_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Grequest_start(MPI_Grequest_query_function *query_fn, MPI_Grequest_free_function *free_fn, MPI_Grequest_cancel_function *cancel_fn, void *extra_state, MPI_Request *request)
{
	call_start(__MPI_GREQUEST_START, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request);
    call_end(__MPI_GREQUEST_START, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result)
{
	call_start(__MPI_GROUP_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_compare(group1, group2, result);
    call_end(__MPI_GROUP_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_DIFFERENCE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_difference(group1, group2, newgroup);
    call_end(__MPI_GROUP_DIFFERENCE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_excl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_EXCL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_excl(group, n, ranks, newgroup);
    call_end(__MPI_GROUP_EXCL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_free(MPI_Group *group)
{
	call_start(__MPI_GROUP_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_free(group);
    call_end(__MPI_GROUP_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_incl(MPI_Group group, int n, const int ranks[], MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_INCL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_incl(group, n, ranks, newgroup);
    call_end(__MPI_GROUP_INCL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_INTERSECTION, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_intersection(group1, group2, newgroup);
    call_end(__MPI_GROUP_INTERSECTION, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_RANGE_EXCL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_range_excl(group, n, ranges, newgroup);
    call_end(__MPI_GROUP_RANGE_EXCL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3], MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_RANGE_INCL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_range_incl(group, n, ranges, newgroup);
    call_end(__MPI_GROUP_RANGE_INCL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_rank(MPI_Group group, int *rank)
{
	call_start(__MPI_GROUP_RANK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_rank(group, rank);
    call_end(__MPI_GROUP_RANK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_size(MPI_Group group, int *size)
{
	call_start(__MPI_GROUP_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_size(group, size);
    call_end(__MPI_GROUP_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_translate_ranks(MPI_Group group1, int n, const int ranks1[], MPI_Group group2, int ranks2[])
{
	call_start(__MPI_GROUP_TRANSLATE_RANKS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2);
    call_end(__MPI_GROUP_TRANSLATE_RANKS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup)
{
	call_start(__MPI_GROUP_UNION, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Group_union(group1, group2, newgroup);
    call_end(__MPI_GROUP_UNION, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Improbe(int source, int tag, MPI_Comm comm, int *flag, MPI_Message *message, MPI_Status *status)
{
	call_start(__MPI_IMPROBE, comm, MPI_NONE);
	int err = PMPI_Improbe(source, tag, comm, flag, message, status);
    call_end(__MPI_IMPROBE, comm, MPI_NONE);
	return err;
}

int MPI_Imrecv(void *buf, int count, MPI_Datatype type, MPI_Message *message, MPI_Request *request)
{
	call_start(__MPI_IMRECV, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Imrecv(buf, count, type, message, request);
    call_end(__MPI_IMRECV, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_create(MPI_Info *info)
{
	call_start(__MPI_INFO_CREATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_create(info);
    call_end(__MPI_INFO_CREATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_delete(MPI_Info info, const char *key)
{
	call_start(__MPI_INFO_DELETE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_delete(info, key);
    call_end(__MPI_INFO_DELETE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_dup(MPI_Info info, MPI_Info *newinfo)
{
	call_start(__MPI_INFO_DUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_dup(info, newinfo);
    call_end(__MPI_INFO_DUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_free(MPI_Info *info)
{
	call_start(__MPI_INFO_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_free(info);
    call_end(__MPI_INFO_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_get(MPI_Info info, const char *key, int valuelen, char *value, int *flag)
{
	call_start(__MPI_INFO_GET, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_get(info, key, valuelen, value, flag);
    call_end(__MPI_INFO_GET, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_get_nkeys(MPI_Info info, int *nkeys)
{
	call_start(__MPI_INFO_GET_NKEYS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_get_nkeys(info, nkeys);
    call_end(__MPI_INFO_GET_NKEYS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_get_nthkey(MPI_Info info, int n, char *key)
{
	call_start(__MPI_INFO_GET_NTHKEY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_get_nthkey(info, n, key);
    call_end(__MPI_INFO_GET_NTHKEY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_get_valuelen(MPI_Info info, const char *key, int *valuelen, int *flag)
{
	call_start(__MPI_INFO_GET_VALUELEN, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_get_valuelen(info, key, valuelen, flag);
    call_end(__MPI_INFO_GET_VALUELEN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Info_set(MPI_Info info, const char *key, const char *value)
{
	call_start(__MPI_INFO_SET, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Info_set(info, key, value);
    call_end(__MPI_INFO_SET, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Initialized(int *flag)
{
	return PMPI_Initialized(flag);
}

int MPI_Intercomm_create(MPI_Comm local_comm, int local_leader, MPI_Comm bridge_comm, int remote_leader, int tag, MPI_Comm *newintercomm)
{
	call_start(__MPI_INTERCOMM_CREATE, local_comm, MPI_NONE);
	int err = PMPI_Intercomm_create(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm);
    call_end(__MPI_INTERCOMM_CREATE, local_comm, MPI_NONE);
	return err;
}

int MPI_Intercomm_merge(MPI_Comm intercomm, int high, MPI_Comm *newintercomm)
{
	call_start(__MPI_INTERCOMM_MERGE, intercomm, MPI_NONE);
	int err = PMPI_Intercomm_merge(intercomm, high, newintercomm);
    call_end(__MPI_INTERCOMM_MERGE, intercomm, MPI_NONE);
	return err;
}

int MPI_Is_thread_main(int *flag)
{
	call_start(__MPI_IS_THREAD_MAIN, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Is_thread_main(flag);
    call_end(__MPI_IS_THREAD_MAIN, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Lookup_name(const char *service_name, MPI_Info info, char *port_name)
{
	call_start(__MPI_LOOKUP_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Lookup_name(service_name, info, port_name);
    call_end(__MPI_LOOKUP_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Mprobe(int source, int tag, MPI_Comm comm, MPI_Message *message, MPI_Status *status)
{
	call_start(__MPI_MPROBE, comm, source);
	int err = PMPI_Mprobe(source, tag, comm, message, status);
    call_end(__MPI_MPROBE, comm, source);
	return err;
}

int MPI_Mrecv(void *buf, int count, MPI_Datatype type, MPI_Message *message, MPI_Status *status)
{
	call_start(__MPI_MRECV, MPI_COMM_WORLD, MPI_NONE);
    call_end(__MPI_MRECV, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Mrecv(buf, count, type, message, status);
	return err;
}

int MPI_Ineighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_INEIGHBOR_ALLGATHER, comm, MPI_ALL);
    call_end(__MPI_INEIGHBOR_ALLGATHER, comm, MPI_ALL);
	int err = PMPI_Ineighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request);
	return err;
}

int MPI_Ineighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_INEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
    call_end(__MPI_INEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
	int err = PMPI_Ineighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request);
	return err;
}

int MPI_Ineighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_INEIGHBOR_ALLTOALL, comm, MPI_ALL);
    call_end(__MPI_INEIGHBOR_ALLTOALL, comm, MPI_ALL);
	int err = PMPI_Ineighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request);
	return err;
}

int MPI_Ineighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_INEIGHBOR_ALLTOALLV, comm, MPI_ALLV);
    call_end(__MPI_INEIGHBOR_ALLTOALLV, comm, MPI_ALLV);
	int err = PMPI_Ineighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request);
	return err;
}

int MPI_Ineighbor_alltoallw(const void *sendbuf, const int sendcounts[], const MPI_Aint sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const MPI_Aint rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_INEIGHBOR_ALLTOALLW, comm, MPI_ALLW);
    call_end(__MPI_INEIGHBOR_ALLTOALLW, comm, MPI_ALLW);
	int err = PMPI_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request);
	return err;
}

int MPI_Op_commutative(MPI_Op op, int *commute)
{
	call_start(__MPI_OP_COMMUTATIVE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Op_commutative(op, commute);
    call_end(__MPI_OP_COMMUTATIVE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Op_create(MPI_User_function *function, int commute, MPI_Op *op)
{
	call_start(__MPI_OP_CREATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Op_create(function, commute, op);
    call_end(__MPI_OP_CREATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Open_port(MPI_Info info, char *port_name)
{
	call_start(__MPI_OPEN_PORT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Open_port(info, port_name);
    call_end(__MPI_OPEN_PORT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Op_free(MPI_Op *op)
{
	call_start(__MPI_OP_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Op_free(op);
    call_end(__MPI_OP_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Pack_external(const char datarep[], const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, MPI_Aint outsize, MPI_Aint *position)
{
	call_start(__MPI_PACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outsize, position);
    call_end(__MPI_PACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Pack_external_size(const char datarep[], int incount, MPI_Datatype datatype, MPI_Aint *size)
{
	call_start(__MPI_PACK_EXTERNAL_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Pack_external_size(datarep, incount, datatype, size);
    call_end(__MPI_PACK_EXTERNAL_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Pack(const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outsize, int *position, MPI_Comm comm)
{
	call_start(__MPI_PACK, comm, MPI_NONE);
	int err = PMPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm);
    call_end(__MPI_PACK, comm, MPI_NONE);
	return err;
}

int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size)
{
	call_start(__MPI_PACK_SIZE, comm, MPI_NONE);
	int err = PMPI_Pack_size(incount, datatype, comm, size);
    call_end(__MPI_PACK_SIZE, comm, MPI_NONE);
	return err;
}

int MPI_Pcontrol(const int level, ...)
{
	call_start(__MPI_PCONTROL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Pcontrol(level);
    call_end(__MPI_PCONTROL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Publish_name(const char *service_name, MPI_Info info, const char *port_name)
{
	call_start(__MPI_PUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Publish_name(service_name, info, port_name);
    call_end(__MPI_PUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Put(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win)
{
	call_start(__MPI_PUT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win);
    call_end(__MPI_PUT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Query_thread(int *provided)
{
	call_start(__MPI_QUERY_THREAD, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Query_thread(provided);
    call_end(__MPI_QUERY_THREAD, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Raccumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request)
{
	call_start(__MPI_RACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request);
    call_end(__MPI_RACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_RECV_INIT, comm, source);
	add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, source);
	int err = PMPI_Recv_init(buf, count, datatype, source, tag, comm, request);
   	call_end(__MPI_RECV_INIT, comm, source);
	return err;
}

int MPI_Ireduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IREDUCE, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, NULL, NULL, MPI_NONE, &count, &datatype, MPI_ALL);
	else
		add_network(comm, &count, &datatype, root, NULL, NULL, MPI_NONE);
	int err = PMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request);
    call_end(__MPI_IREDUCE, comm, MPI_ALL);
	return err;
}

int MPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
	call_start(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op);
    call_end(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IREDUCE_SCATTER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == 0)
		add_network(comm, recvcounts, &datatype, MPI_ALLV, recvcounts, &datatype, MPI_ALLV);
	else
		add_network(comm, &recvcounts[my_rank], &datatype, 0, &recvcounts[my_rank], &datatype, 0);
	int err = PMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request);
    call_end(__MPI_IREDUCE_SCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_REDUCE_SCATTER_BLOCK, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == 0)
		add_network(comm, &recvcount, &datatype, MPI_ALL, &recvcount, &datatype, MPI_ALLV);
	else
		add_network(comm, &recvcount, &datatype, 0, &recvcount, &datatype, 0);
	int err = PMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm);
    call_end(__MPI_REDUCE_SCATTER_BLOCK, comm, MPI_ALL);
	return err;
}

int MPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, int recvcount, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IREDUCE_SCATTER_BLOCK, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == 0)
		add_network(comm, &recvcount, &datatype, MPI_ALL, &recvcount, &datatype, MPI_ALLV);
	else
		add_network(comm, &recvcount, &datatype, 0, &recvcount, &datatype, 0);
	int err = PMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request);
    call_end(__MPI_IREDUCE_SCATTER_BLOCK, comm, MPI_ALL);
	return err;
}

int MPI_Register_datarep(const char *datarep, MPI_Datarep_conversion_function *read_conversion_fn, MPI_Datarep_conversion_function *write_conversion_fn, MPI_Datarep_extent_function *dtype_file_extent_fn, void *extra_state)
{
	call_start(__MPI_REGISTER_DATAREP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Register_datarep(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state);
    call_end(__MPI_REGISTER_DATAREP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Request_free(MPI_Request *request)
{
	call_start(__MPI_REQUEST_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Request_free(request);
    call_end(__MPI_REQUEST_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status)
{
	call_start(__MPI_REQUEST_GET_STATUS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Request_get_status(request, flag, status);
    call_end(__MPI_REQUEST_GET_STATUS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Rget(void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request)
{
	call_start(__MPI_RGET, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request);
    call_end(__MPI_RGET, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Rget_accumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, void *result_addr, int result_count, MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request)
{
	call_start(__MPI_RGET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request);
    call_end(__MPI_RGET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Rput(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, int target_rank, MPI_Aint target_disp, int target_count, MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request)
{
	call_start(__MPI_RPUT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request);
    call_end(__MPI_RPUT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Rsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_RSEND_INIT, comm, dest);
	int err = PMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
   	call_end(__MPI_RSEND_INIT, comm, dest);
	return err;
}

int MPI_Iscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISCAN, comm, MPI_NONE);
	int err = PMPI_Iscan(sendbuf, recvbuf, count, datatype, op, comm, request);
    call_end(__MPI_ISCAN, comm, MPI_NONE);
	return err;
}

int MPI_Iscatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISCATTER, comm, MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, &sendcount, &sendtype, MPI_ALL, NULL, NULL, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, root);
	int err = PMPI_Iscatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request);
    call_end(__MPI_ISCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Iscatterv(const void *sendbuf, const int sendcounts[], const int displs[], MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISCATTERV, comm, MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(comm, &my_rank);
	if(my_rank == root)
		add_network(comm, sendcounts, &sendtype, MPI_ALLV, NULL, NULL, MPI_NONE);
	else
		add_network(comm, NULL, NULL, MPI_NONE, &recvcount, &recvtype, root);
	int err = MPI_Iscatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request);
    call_end(__MPI_ISCATTERV, comm, MPI_ALLV);
	return err;
}

int MPI_Send_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_SEND_INIT, comm, dest);
	int err = PMPI_Send_init(buf, count, datatype, dest, tag, comm, request);
   	call_end(__MPI_SEND_INIT, comm, dest);
	return err;
}

int MPI_Ssend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_SSEND_INIT, comm, dest);
	int err = PMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
   	call_end(__MPI_SSEND_INIT, comm, dest);
	return err;
}

int MPI_Start(MPI_Request *request)
{
	call_start(__MPI_START, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Start(request);
    call_end(__MPI_START, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Startall(int count, MPI_Request array_of_requests[])
{
	call_start(__MPI_STARTALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Startall(count, array_of_requests);
    call_end(__MPI_STARTALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Status_set_cancelled(MPI_Status *status, int flag)
{
	call_start(__MPI_STATUS_SET_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Status_set_cancelled(status, flag);
    call_end(__MPI_STATUS_SET_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count)
{
	call_start(__MPI_STATUS_SET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Status_set_elements(status, datatype, count);
    call_end(__MPI_STATUS_SET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Status_set_elements_x(MPI_Status *status, MPI_Datatype datatype, MPI_Count count)
{
	call_start(__MPI_STATUS_SET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Status_set_elements_x(status, datatype, count);
    call_end(__MPI_STATUS_SET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Testall(int count, MPI_Request array_of_requests[], int *flag, MPI_Status array_of_statuses[])
{
	call_start(__MPI_TESTALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Testall(count, array_of_requests, flag, array_of_statuses);
    call_end(__MPI_TESTALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Testany(int count, MPI_Request array_of_requests[], int *index, int *flag, MPI_Status *status)
{
	call_start(__MPI_TESTANY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Testany(count, array_of_requests, index, flag, status);
    call_end(__MPI_TESTANY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
	call_start(__MPI_TEST, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Test(request, flag, status);
    call_end(__MPI_TEST, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Test_cancelled(const MPI_Status *status, int *flag)
{
	call_start(__MPI_TEST_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Test_cancelled(status, flag);
    call_end(__MPI_TEST_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Testsome(int incount, MPI_Request array_of_requests[], int *outcount, int array_of_indices[], MPI_Status array_of_statuses[])
{
	call_start(__MPI_TESTSOME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses);
    call_end(__MPI_TESTSOME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Topo_test(MPI_Comm comm, int *status)
{
	call_start(__MPI_TOPO_TEST, comm, MPI_NONE);
	int err = PMPI_Topo_test(comm, status);
    call_end(__MPI_TOPO_TEST, comm, MPI_NONE);
	return err;
}

int MPI_Type_commit(MPI_Datatype *type)
{
	call_start(__MPI_TYPE_COMMIT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_commit(type);
    call_end(__MPI_TYPE_COMMIT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CONTIGUOUS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_contiguous(count, oldtype, newtype);
    call_end(__MPI_TYPE_CONTIGUOUS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_darray(int size, int rank, int ndims, const int gsize_array[], const int distrib_array[], const int darg_array[], const int psize_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_DARRAY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_darray(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_DARRAY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_f90_complex(int p, int r, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_F90_COMPLEX, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_f90_complex(p, r, newtype);
    call_end(__MPI_TYPE_CREATE_F90_COMPLEX, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_f90_integer(int r, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_F90_INTEGER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_f90_integer(r, newtype);
    call_end(__MPI_TYPE_CREATE_F90_INTEGER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_f90_real(int p, int r, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_F90_REAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_f90_real(p, r, newtype);
    call_end(__MPI_TYPE_CREATE_F90_REAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_hindexed_block(int count, int blocklength, const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_HINDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_hindexed_block(count, blocklength, array_of_displacements, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_HINDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_hindexed(int count, const int array_of_blocklengths[], const MPI_Aint array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_HINDEXED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_hindexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_HINDEXED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_HVECTOR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_HVECTOR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_keyval(MPI_Type_copy_attr_function *type_copy_attr_fn, MPI_Type_delete_attr_function *type_delete_attr_fn, int *type_keyval, void *extra_state)
{
	call_start(__MPI_TYPE_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state);
    call_end(__MPI_TYPE_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_indexed_block(int count, int blocklength, const int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_INDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_INDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_struct(int count, const int array_of_block_lengths[], const MPI_Aint array_of_displacements[], const MPI_Datatype array_of_types[], MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_STRUCT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_struct(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype);
    call_end(__MPI_TYPE_CREATE_STRUCT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_subarray(int ndims, const int size_array[], const int subsize_array[], const int start_array[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_SUBARRAY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_subarray(ndims, size_array, subsize_array, start_array, order, oldtype, newtype);
    call_end(__MPI_TYPE_CREATE_SUBARRAY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_CREATE_RESIZED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_create_resized(oldtype, lb, extent, newtype);
    call_end(__MPI_TYPE_CREATE_RESIZED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_delete_attr(MPI_Datatype type, int type_keyval)
{
	call_start(__MPI_TYPE_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_delete_attr(type, type_keyval);
    call_end(__MPI_TYPE_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_dup(MPI_Datatype type, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_DUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_dup(type, newtype);
    call_end(__MPI_TYPE_DUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_free(MPI_Datatype *type)
{
	call_start(__MPI_TYPE_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_free(type);
    call_end(__MPI_TYPE_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_free_keyval(int *type_keyval)
{
	call_start(__MPI_TYPE_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_free_keyval(type_keyval);
    call_end(__MPI_TYPE_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_attr(MPI_Datatype type, int type_keyval, void *attribute_val, int *flag)
{
	call_start(__MPI_TYPE_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_attr(type, type_keyval, attribute_val, flag);
    call_end(__MPI_TYPE_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_contents(MPI_Datatype mtype, int max_integers, int max_addresses, int max_datatypes, int array_of_integers[], MPI_Aint array_of_addresses[], MPI_Datatype array_of_datatypes[])
{
	call_start(__MPI_TYPE_GET_CONTENTS, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_contents(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes);
    call_end(__MPI_TYPE_GET_CONTENTS, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_envelope(MPI_Datatype type, int *num_integers, int *num_addresses, int *num_datatypes, int *combiner)
{
	call_start(__MPI_TYPE_GET_ENVELOPE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_envelope(type, num_integers, num_addresses, num_datatypes, combiner);
    call_end(__MPI_TYPE_GET_ENVELOPE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_extent(MPI_Datatype type, MPI_Aint *lb, MPI_Aint *extent)
{
	call_start(__MPI_TYPE_GET_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_extent(type, lb, extent);
    call_end(__MPI_TYPE_GET_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_extent_x(MPI_Datatype type, MPI_Count *lb, MPI_Count *extent)
{
	call_start(__MPI_TYPE_GET_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_extent_x(type, lb, extent);
    call_end(__MPI_TYPE_GET_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_name(MPI_Datatype type, char *type_name, int *resultlen)
{
	call_start(__MPI_TYPE_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_name(type, type_name, resultlen);
    call_end(__MPI_TYPE_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *true_lb, MPI_Aint *true_extent)
{
	call_start(__MPI_TYPE_GET_TRUE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_true_extent(datatype, true_lb, true_extent);
    call_end(__MPI_TYPE_GET_TRUE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_get_true_extent_x(MPI_Datatype datatype, MPI_Count *true_lb, MPI_Count *true_extent)
{
	call_start(__MPI_TYPE_GET_TRUE_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_get_true_extent_x(datatype, true_lb, true_extent);
    call_end(__MPI_TYPE_GET_TRUE_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_indexed(int count, const int array_of_blocklengths[], const int array_of_displacements[], MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_INDEXED, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_indexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype);
    call_end(__MPI_TYPE_INDEXED, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_match_size(int typeclass, int size, MPI_Datatype *type)
{
	call_start(__MPI_TYPE_MATCH_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_match_size(typeclass, size, type);
    call_end(__MPI_TYPE_MATCH_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_set_attr(MPI_Datatype type, int type_keyval, void *attr_val)
{
	call_start(__MPI_TYPE_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_set_attr(type, type_keyval, attr_val);
    call_end(__MPI_TYPE_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_set_name(MPI_Datatype type, const char *type_name)
{
	call_start(__MPI_TYPE_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_set_name(type, type_name);
    call_end(__MPI_TYPE_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_size(MPI_Datatype type, int *size)
{
	call_start(__MPI_TYPE_SIZE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_size(type, size);
    call_end(__MPI_TYPE_SIZE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_size_x(MPI_Datatype type, MPI_Count *size)
{
	call_start(__MPI_TYPE_SIZE_X, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_size_x(type, size);
    call_end(__MPI_TYPE_SIZE_X, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	call_start(__MPI_TYPE_VECTOR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Type_vector(count, blocklength, stride, oldtype, newtype);
    call_end(__MPI_TYPE_VECTOR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Unpack(const void *inbuf, int insize, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm)
{
	call_start(__MPI_UNPACK, comm, MPI_NONE);
	int err = PMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm);
    call_end(__MPI_UNPACK, comm, MPI_NONE);
	return err;
}

int MPI_Unpublish_name(const char *service_name, MPI_Info info, const char *port_name)
{
	call_start(__MPI_UNPUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Unpublish_name(service_name, info, port_name);
    call_end(__MPI_UNPUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Unpack_external(const char datarep[], const void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype)
{
	call_start(__MPI_UNPACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype);
    call_end(__MPI_UNPACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_allocate(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win)
{
	call_start(__MPI_WIN_ALLOCATE, comm, MPI_NONE);
	int err = PMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win);
    call_end(__MPI_WIN_ALLOCATE, comm, MPI_NONE);
	return err;
}

int MPI_Win_allocate_shared(MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, void *baseptr, MPI_Win *win)
{
	call_start(__MPI_WIN_ALLOCATE_SHARED, comm, MPI_NONE);
	int err = PMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win);
    call_end(__MPI_WIN_ALLOCATE_SHARED, comm, MPI_NONE);
	return err;
}

int MPI_Win_attach(MPI_Win win, void *base, MPI_Aint size)
{
	call_start(__MPI_WIN_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_attach(win, base, size);
    call_end(__MPI_WIN_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_call_errhandler(MPI_Win win, int errorcode)
{
	call_start(__MPI_WIN_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_call_errhandler(win, errorcode);
    call_end(__MPI_WIN_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_complete(MPI_Win win)
{
	call_start(__MPI_WIN_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_complete(win);
    call_end(__MPI_WIN_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, MPI_Comm comm, MPI_Win *win)
{
	call_start(__MPI_WIN_CREATE, comm, MPI_NONE);
	int err = PMPI_Win_create(base, size, disp_unit, info, comm, win);
    call_end(__MPI_WIN_CREATE, comm, MPI_NONE);
	return err;
}

int MPI_Win_create_dynamic(MPI_Info info, MPI_Comm comm, MPI_Win *win)
{
	call_start(__MPI_WIN_CREATE_DYNAMIC, comm, MPI_NONE);
	int err = PMPI_Win_create_dynamic(info, comm, win);
    call_end(__MPI_WIN_CREATE_DYNAMIC, comm, MPI_NONE);
	return err;
}

int MPI_Win_create_errhandler(MPI_Win_errhandler_function *function, MPI_Errhandler *errhandler)
{
	call_start(__MPI_WIN_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_create_errhandler(function, errhandler);
    call_end(__MPI_WIN_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_create_keyval(MPI_Win_copy_attr_function *win_copy_attr_fn, MPI_Win_delete_attr_function *win_delete_attr_fn, int *win_keyval, void *extra_state)
{
	call_start(__MPI_WIN_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state);
    call_end(__MPI_WIN_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_delete_attr(MPI_Win win, int win_keyval)
{
	call_start(__MPI_WIN_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_delete_attr(win, win_keyval);
    call_end(__MPI_WIN_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_detach(MPI_Win win, const void *base)
{
	call_start(__MPI_WIN_DETACH, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_detach(win, base);
    call_end(__MPI_WIN_DETACH, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_fence(int assert, MPI_Win win)
{
	call_start(__MPI_WIN_FENCE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_fence(assert, win);
    call_end(__MPI_WIN_FENCE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_free(MPI_Win *win)
{
	call_start(__MPI_WIN_FREE, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_free(win);
    call_end(__MPI_WIN_FREE, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_free_keyval(int *win_keyval)
{
	call_start(__MPI_WIN_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_free_keyval(win_keyval);
    call_end(__MPI_WIN_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_get_attr(MPI_Win win, int win_keyval, void *attribute_val, int *flag)
{
	call_start(__MPI_WIN_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_get_attr(win, win_keyval, attribute_val, flag);
    call_end(__MPI_WIN_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler)
{
	call_start(__MPI_WIN_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_get_errhandler(win, errhandler);
    call_end(__MPI_WIN_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_get_group(MPI_Win win, MPI_Group *group)
{
	call_start(__MPI_WIN_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_get_group(win, group);
    call_end(__MPI_WIN_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_get_info(MPI_Win win, MPI_Info *info_used)
{
	call_start(__MPI_WIN_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_get_info(win, info_used);
    call_end(__MPI_WIN_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_get_name(MPI_Win win, char *win_name, int *resultlen)
{
	call_start(__MPI_WIN_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_get_name(win, win_name, resultlen);
    call_end(__MPI_WIN_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_post(MPI_Group group, int assert, MPI_Win win)
{
	call_start(__MPI_WIN_POST, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_post(group, assert, win);
    call_end(__MPI_WIN_POST, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_set_attr(MPI_Win win, int win_keyval, void *attribute_val)
{
	call_start(__MPI_WIN_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_set_attr(win, win_keyval, attribute_val);
    call_end(__MPI_WIN_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler)
{
	call_start(__MPI_WIN_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_set_errhandler(win, errhandler);
    call_end(__MPI_WIN_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_set_info(MPI_Win win, MPI_Info info)
{
	call_start(__MPI_WIN_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_set_info(win, info);
    call_end(__MPI_WIN_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_set_name(MPI_Win win, const char *win_name)
{
	call_start(__MPI_WIN_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_set_name(win, win_name);
    call_end(__MPI_WIN_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_shared_query(MPI_Win win, int rank, MPI_Aint *size, int *disp_unit, void *baseptr)
{
	call_start(__MPI_WIN_SHARED_QUERY, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_shared_query(win, rank, size, disp_unit, baseptr);
    call_end(__MPI_WIN_SHARED_QUERY, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_start(MPI_Group group, int assert, MPI_Win win)
{
	call_start(__MPI_WIN_START, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_start(group, assert, win);
    call_end(__MPI_WIN_START, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_test(MPI_Win win, int *flag)
{
	call_start(__MPI_WIN_TEST, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_test(win, flag);
    call_end(__MPI_WIN_TEST, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_unlock(int rank, MPI_Win win)
{
	call_start(__MPI_WIN_UNLOCK, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_unlock(rank, win);
    call_end(__MPI_WIN_UNLOCK, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

int MPI_Win_unlock_all(MPI_Win win)
{
	call_start(__MPI_WIN_UNLOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	int err = PMPI_Win_unlock_all(win);
    call_end(__MPI_WIN_UNLOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	return err;
}

#endif // DISABLE_ACCESSORY_MPI

#endif // DISABLE_PROFILING_MPI
