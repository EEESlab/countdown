/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * m *			* Redistributions of source code must retain the above copyright notice, this
 *			list of conditions and the following disclaimer.
 *
 *			* Redistributions in binary form must reproduce the above copyright notice,
 *				this list of conditions and the following disclaimer in the documentation
 *				and/or other materials provided with the distribution.
 *
 *			* Neither the name of the copyright holder nor the names of its
 *				contributors may be used to endorse or promote products derived from
 *				this software without sp * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIE, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVE * Author: Daniele Cesarini, University of Bologna
*/

#include "cntd.h"

int MPI_Abort(MPI_Comm comm, int errorcode)
{
	stop_cntd();
	return PMPI_Abort(comm, errorcode);
}

int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLGATHER, comm, MPI_ALL);
	int err = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_ALLGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLGATHERV, comm, MPI_ALLV);
	int err = PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
	call_end(__MPI_ALLGATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_ALLREDUCE, comm, MPI_NONE);
	int err = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
	call_end(__MPI_ALLREDUCE, comm, MPI_NONE);
	return err;
}

int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLTOALL, comm, MPI_ALL);
	int err = PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_ALLTOALL, comm, MPI_ALL);
	return err;
}

int MPI_Alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_ALLTOALLV, comm, MPI_ALLV);
	int err = PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm);
	call_end(__MPI_ALLTOALLV, comm, MPI_ALLV);
	return err;
}

int MPI_Alltoallw(const void *sendbuf, const int sendcounts[], const int sdispls[], const MPI_Datatype sendtypes[], void *recvbuf, const int recvcounts[], const int rdispls[], const MPI_Datatype recvtypes[], MPI_Comm comm)
{
	call_start(__MPI_ALLTOALLW, comm, MPI_ALLW);
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
	int err = PMPI_Bcast(buffer, count, datatype, root, comm);
	call_end(__MPI_BCAST, comm, MPI_ALL);
	return err;
}

int MPI_Bsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_BSEND, comm, dest);
	int err = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_BSEND, comm, dest);
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

int MPI_Finalize(void)
{
	call_start(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	PMPI_Barrier(MPI_COMM_WORLD);

	call_end(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	stop_cntd();

	return PMPI_Finalize();
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_GATHER, comm, MPI_ALL);
	int err = PMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_GATHER, comm, MPI_ALL);
	return err;
}

int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_GATHERV, comm, MPI_ALLV);
	int err = PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
	call_end(__MPI_GATHERV, comm, MPI_ALLV);
	return err;
}

int MPI_Init(int *argc, char ***argv)
{
	int err = PMPI_Init(argc, argv);
	start_cntd();
	return err;
}

int MPI_Neighbor_allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLGATHER, comm, MPI_ALL);
	// network
	int err = PMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLGATHER, comm, MPI_ALL);
	return err;
}

int MPI_Neighbor_allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int displs[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
	// network
	int err = PMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLGATHERV, comm, MPI_ALLV);
	return err;
}


int MPI_Neighbor_alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLTOALL, comm, MPI_ALL);
	// network
	int err = PMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
	call_end(__MPI_NEIGHBOR_ALLTOALL, comm, MPI_ALL);
	return err;
}


int MPI_Neighbor_alltoallv(const void *sendbuf, const int sendcounts[], const int sdispls[], MPI_Datatype sendtype, void *recvbuf, const int recvcounts[], const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm)
{
	call_start(__MPI_NEIGHBOR_ALLTOALLV, comm, MPI_ALLV);
	// network
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

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_RECV, comm, source);
	int err = PMPI_Recv(buf, count, datatype, source, tag, comm, status);
	call_end(__MPI_RECV, comm, source);
	return err;
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
	call_start(__MPI_REDUCE, comm, MPI_ALL);
	int err = PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
	call_end(__MPI_REDUCE, comm, MPI_ALL);
	return err;
}

int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[], MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
	call_start(__MPI_REDUCE_SCATTER, comm, MPI_ALL);
	int err = PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
	call_end(__MPI_REDUCE_SCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Rsend(const void *ibuf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_RSEND, comm, dest);
	int err = PMPI_Rsend(ibuf, count, datatype, dest, tag, comm);
	call_end(__MPI_RSEND, comm, dest);
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
	int err = PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_SCATTER, comm, MPI_ALL);
	return err;
}

int MPI_Scatterv(const void *sendbuf, const int sendcounts[], const int displs[], MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
{
	call_start(__MPI_SCATTERV, comm, MPI_ALLV);
	int err = PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
	call_end(__MPI_SCATTERV, comm, MPI_ALLV);
	return err;
}

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_SEND, comm, dest);
	int err = PMPI_Send(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_SEND, comm, dest);
	return err;
}

int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_SENDRECV, comm, MPI_NONE);
	int err = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
	call_end(__MPI_SENDRECV, comm, MPI_NONE);
	return err;
}

int MPI_Sendrecv_replace(void * buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status)
{
	call_start(__MPI_SENDRECV_REPLACE, comm, MPI_NONE);
	int err = PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
	call_end(__MPI_SENDRECV_REPLACE, comm, MPI_NONE);
	return err;
}

int MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
	call_start(__MPI_SSEND, comm, dest);
	int err = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
	call_end(__MPI_SSEND, comm, dest);
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

int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISEND, comm, dest);
	int err = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_ISEND, comm, dest);
	return err;
}

int MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_ISSEND, comm, dest);
	int err = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_ISSEND, comm, dest);
	return err;
}

int MPI_Irsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IRSEND, comm, dest);
	int err = PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_IRSEND, comm, dest);
	return err;
}

int MPI_Ibsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IBSEND, comm, MPI_NONE);
	int err = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
	call_end(__MPI_IBSEND, comm, MPI_NONE);
	return err;
}

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
	call_start(__MPI_IRECV, comm, source);
	int err = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
	call_end(__MPI_IRECV, comm, source);
	return err;
}