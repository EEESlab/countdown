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

#ifdef OMPI_MPI_H

// Comment functions are deprecated from MPI-1 version
extern void pmpi_abort_(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr);
extern void pmpi_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_allreduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_barrier_(MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_bcast_(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_bsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_comm_split_(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_comm_split_type_(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_exscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_file_sync_(MPI_Fint *fh, MPI_Fint *ierr);
extern void pmpi_finalize_(MPI_Fint *ierr);
extern void pmpi_gather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_gatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_init_(MPI_Fint *argc, char ***argv, MPI_Fint *ierr);
extern void pmpi_neighbor_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_neighbor_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_neighbor_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_neighbor_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_neighbor_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_recv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_reduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_reduce_local_(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr);
extern void pmpi_reduce_scatter_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_rsend_(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_scan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_scatter_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_scatterv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_send_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_sendrecv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_sendrecv_replace_(MPI_Fint * buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_ssend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_waitall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr);
extern void pmpi_waitany_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_wait_(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_waitsome_(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr);
extern void pmpi_win_flush_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_flush_all_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_flush_local_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_flush_local_all_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_lock_(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_lock_all_(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_sync_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_wait_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_isend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_issend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_irsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ibsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_irecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);


static void FMPI_Abort(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	stop_cntd();
	return pmpi_abort_(comm, errorcode, ierr);
}

static void FMPI_Allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_allgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
  	call_start(__MPI_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_allgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
	call_end(__MPI_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Allreduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_ALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_allreduce_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_ALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_alltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_alltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
	call_end(__MPI_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	pmpi_alltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
	call_end(__MPI_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
}

static void FMPI_Barrier(MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_BARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_barrier_(comm, ierr);
	call_end(__MPI_BARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Bcast(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_BCAST, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_bcast_(buffer, count, datatype, root, comm, ierr);
	call_end(__MPI_BCAST, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Bsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_BSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_bsend_(buf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_BSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Comm_split(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	call_start(__MPI_COMM_SPLIT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_split_(comm, color, key, newcomm, ierr);
	call_end(__MPI_COMM_SPLIT, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Comm_split_type(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	call_start(__MPI_COMM_SPLIT_TYPE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_split_type_(comm, split_type, key, info, newcomm, ierr);
	call_end(__MPI_COMM_SPLIT_TYPE, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Exscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_EXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_exscan_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_EXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_File_sync(MPI_Fint *fh, MPI_Fint *ierr)
{
	call_start(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_sync_(fh, ierr);
	call_end(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Finalize(MPI_Fint *ierr)
{
	call_start(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	int local_ierr;
	MPI_Fint world = MPI_Comm_c2f(MPI_COMM_WORLD);
	pmpi_barrier_(&world, &local_ierr);

	call_end(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);
	stop_cntd();

	pmpi_finalize_(ierr);
}

static void FMPI_Gather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_GATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_gather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_GATHER, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Gatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_GATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_gatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
	call_end(__MPI_GATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Init(MPI_Fint *argc, char ***argv, MPI_Fint *ierr)
{
	pmpi_init_(argc, argv, ierr);
	start_cntd();
}

static void FMPI_Neighbor_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_NEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_neighbor_allgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Neighbor_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_NEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_neighbor_allgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Neighbor_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_NEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_neighbor_alltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Neighbor_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_NEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_neighbor_alltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Neighbor_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_NEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	pmpi_neighbor_alltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
}

static void FMPI_Recv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_RECV, MPI_Comm_f2c(*comm), *source);
	pmpi_recv_(buf, count, datatype, source, tag, comm, status, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_RECV, MPI_Comm_f2c(*comm), *source);
}

static void FMPI_Reduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_REDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_reduce_(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
	call_end(__MPI_REDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Reduce_local(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
	call_start(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_ALL);
	pmpi_reduce_local_(inbuf, inoutbuf, count, datatype, op, ierr);
	call_end(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_ALL);
}

static void FMPI_Reduce_scatter(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_REDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_reduce_scatter_(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
	call_end(__MPI_REDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Rsend(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_RSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_rsend_(ibuf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_RSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Scan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_SCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_scan_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_SCAN, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Scatter(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_scatter_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
}

static void FMPI_Scatterv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_SCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_scatterv_(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_SCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
}

static void FMPI_Send(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_SEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_send_(buf, count, datatype, dest, tag, comm, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_SEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Sendrecv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	call_start(__MPI_SENDRECV, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_sendrecv_(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
	call_end(__MPI_SENDRECV, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Sendrecv_replace(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	call_start(__MPI_SENDRECV_REPLACE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_sendrecv_replace_(buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
	call_end(__MPI_SENDRECV_REPLACE, MPI_Comm_f2c(*comm), MPI_NONE);
}

static void FMPI_Ssend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	call_start(__MPI_SSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_ssend_(buf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_SSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Waitall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	call_start(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitall_(count, array_of_requests, array_of_statuses, ierr);
	call_end(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Waitany(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
	call_start(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitany_(count, array_of_requests, index, status, ierr);
	call_end(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Wait(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
	call_start(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_wait_(request, status, ierr);
	call_end(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Waitsome(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	call_start(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitsome_(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
	call_end(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_flush(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_(rank, win, ierr);
	call_end(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_flush_all(MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_all_(win, ierr);
	call_end(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_flush_local(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_local_(rank, win, ierr);
	call_end(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_flush_local_all(MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_local_all_(win, ierr);
	call_end(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_lock(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_lock_(lock_type, rank, assert, win, ierr);
	call_end(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_lock_all(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_lock_all_(assert, win, ierr);
	call_end(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_sync(MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_sync_(win, ierr);
	call_end(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Win_wait(MPI_Fint *win, MPI_Fint *ierr)
{
	call_start(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_wait_(win, ierr);
	call_end(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
}

static void FMPI_Isend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_ISEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_isend_(buf, count, datatype, dest, tag, comm, request, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_ISEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Issend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_ISSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_issend_(buf, count, datatype, dest, tag, comm, request, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_ISSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Irsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_IRSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_irsend_(buf, count, datatype, dest, tag, comm, request, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_IRSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Ibsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_IBSEND, MPI_Comm_f2c(*comm), *dest);
	pmpi_ibsend_(buf, count, datatype, dest, tag, comm, request, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_IBSEND, MPI_Comm_f2c(*comm), *dest);
}

static void FMPI_Irecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	if(!cntd->no_p2p)
		call_start(__MPI_IRECV, MPI_Comm_f2c(*comm), *source);
	pmpi_irecv_(buf, count, datatype, source, tag, comm, request, ierr);
	if(!cntd->no_p2p)
		call_end(__MPI_IRECV, MPI_Comm_f2c(*comm), *source);
}

// FORTRAN ABI Interfaces

// Lowercase
void mpi_abort(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Abort(comm, errorcode, ierr);
}

void mpi_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_allreduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_barrier(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Barrier(comm, ierr);
}

void mpi_bcast(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bcast(buffer, count, datatype, root, comm, ierr);
}

void mpi_bsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_comm_split(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split(comm, color, key, newcomm, ierr);
}

void mpi_comm_split_type(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split_type(comm, split_type, key, info, newcomm, ierr);
}

void mpi_exscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_file_sync(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_sync(fh, ierr);
}

void mpi_finalize(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

void mpi_gather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_gatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
}

void mpi_neighbor_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_neighbor_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_recv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_reduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
}

void mpi_reduce_local(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op, ierr);
}

void mpi_reduce_scatter(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
}

void mpi_rsend(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_scan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_scatter(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_scatterv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_send(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Send(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_sendrecv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
}

void mpi_sendrecv_replace(MPI_Fint * buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv_replace( buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
}

void mpi_ssend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Ssend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_waitall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitall(count, array_of_requests, array_of_statuses, ierr);
}

void mpi_waitany(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Waitany(count, array_of_requests, index, status, ierr);
}

void mpi_wait(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Wait(request, status, ierr);
}

void mpi_waitsome(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_win_flush(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush(rank, win, ierr);
}

void mpi_win_flush_all(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_all(win, ierr);
}

void mpi_win_flush_local(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local(rank, win, ierr);
}

void mpi_win_flush_local_all(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local_all(win, ierr);
}

void mpi_win_lock(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock(lock_type, rank, assert, win, ierr);
}

void mpi_win_lock_all(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock_all(assert, win, ierr);
}

void mpi_win_sync(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_sync(win, ierr);
}

void mpi_win_wait(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_wait(win, ierr);
}

void mpi_isend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Isend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_issend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Issend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ibsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irecv(buf, count, datatype, source, tag, comm, request, ierr);
}


// Lowercase - single underscore
void mpi_abort_(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Abort(comm, errorcode, ierr);
}

void mpi_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_allreduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_barrier_(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Barrier(comm, ierr);
}

void mpi_bcast_(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bcast(buffer, count, datatype, root, comm, ierr);
}

void mpi_bsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_comm_split_(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split(comm, color, key, newcomm, ierr);
}

void mpi_comm_split_type_(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split_type(comm, split_type, key, info, newcomm, ierr);
}

void mpi_exscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_file_sync_(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_sync(fh, ierr);
}

void mpi_finalize_(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

void mpi_gather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_gatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
}

void mpi_neighbor_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_neighbor_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_recv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_reduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
}

void mpi_reduce_local_(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op, ierr);
}

void mpi_reduce_scatter_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
}

void mpi_rsend_(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_scan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_scatter_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_scatterv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_send_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Send(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_sendrecv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
}

void mpi_sendrecv_replace_(MPI_Fint * buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv_replace( buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
}

void mpi_ssend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Ssend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_waitall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitall(count, array_of_requests, array_of_statuses, ierr);
}

void mpi_waitany_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Waitany(count, array_of_requests, index, status, ierr);
}

void mpi_wait_(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Wait(request, status, ierr);
}

void mpi_waitsome_(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_win_flush_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush(rank, win, ierr);
}

void mpi_win_flush_all_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_all(win, ierr);
}

void mpi_win_flush_local_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local(rank, win, ierr);
}

void mpi_win_flush_local_all_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local_all(win, ierr);
}

void mpi_win_lock_(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock(lock_type, rank, assert, win, ierr);
}

void mpi_win_lock_all_(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock_all(assert, win, ierr);
}

void mpi_win_sync_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_sync(win, ierr);
}

void mpi_win_wait_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_wait(win, ierr);
}

void mpi_isend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Isend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_issend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Issend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ibsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irecv(buf, count, datatype, source, tag, comm, request, ierr);
}


// Lowercase - double underscore
void mpi_abort__(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Abort(comm, errorcode, ierr);
}

void mpi_allgather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_allgatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_allreduce__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_alltoall__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_alltoallv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_alltoallw__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_barrier__(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Barrier(comm, ierr);
}

void mpi_bcast__(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bcast(buffer, count, datatype, root, comm, ierr);
}

void mpi_bsend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_comm_split__(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split(comm, color, key, newcomm, ierr);
}

void mpi_comm_split_type__(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split_type(comm, split_type, key, info, newcomm, ierr);
}

void mpi_exscan__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_file_sync__(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_sync(fh, ierr);
}

void mpi_finalize__(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

void mpi_gather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_gatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
}

void mpi_neighbor_allgather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_allgatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void mpi_neighbor_alltoall__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void mpi_neighbor_alltoallw__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void mpi_recv__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_reduce__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
}

void mpi_reduce_local__(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op, ierr);
}

void mpi_reduce_scatter__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
}

void mpi_rsend__(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_scan__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void mpi_scatter__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_scatterv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void mpi_send__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Send(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_sendrecv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
}

void mpi_sendrecv_replace__(MPI_Fint * buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv_replace( buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
}

void mpi_ssend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Ssend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_waitall__(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitall(count, array_of_requests, array_of_statuses, ierr);
}

void mpi_waitany__(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Waitany(count, array_of_requests, index, status, ierr);
}

void mpi_wait__(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Wait(request, status, ierr);
}

void mpi_waitsome__(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_win_flush__(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush(rank, win, ierr);
}

void mpi_win_flush_all__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_all(win, ierr);
}

void mpi_win_flush_local__(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local(rank, win, ierr);
}

void mpi_win_flush_local_all__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local_all(win, ierr);
}

void mpi_win_lock__(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock(lock_type, rank, assert, win, ierr);
}

void mpi_win_lock_all__(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock_all(assert, win, ierr);
}

void mpi_win_sync__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_sync(win, ierr);
}

void mpi_win_wait__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_wait(win, ierr);
}

void mpi_isend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Isend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_issend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Issend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irsend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ibsend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_irecv__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irecv(buf, count, datatype, source, tag, comm, request, ierr);
}


// Uppercase
void MPI_ABORT(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Abort(comm, errorcode, ierr);
}

void MPI_ALLGATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void MPI_ALLGATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void MPI_ALLREDUCE(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void MPI_ALLTOALL(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void MPI_ALLTOALLV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void MPI_ALLTOALLW(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void MPI_BARRIER(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Barrier(comm, ierr);
}

void MPI_BCAST(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bcast(buffer, count, datatype, root, comm, ierr);
}

void MPI_BSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void MPI_COMM_SPLIT(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split(comm, color, key, newcomm, ierr);
}

void MPI_COMM_SPLIT_TYPE(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_split_type(comm, split_type, key, info, newcomm, ierr);
}

void MPI_EXSCAN(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void MPI_FILE_SYNC(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_sync(fh, ierr);
}

void MPI_FINALIZE(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

void MPI_GATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void MPI_GATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
}

void MPI_NEIGHBOR_ALLGATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void MPI_NEIGHBOR_ALLGATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
}

void MPI_NEIGHBOR_ALLTOALL(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
}

void MPI_NEIGHBOR_ALLTOALLV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
}

void MPI_NEIGHBOR_ALLTOALLW(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Neighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
}

void MPI_RECV(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void MPI_REDUCE(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
}

void MPI_REDUCE_LOCAL(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op, ierr);
}

void MPI_REDUCE_SCATTER(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
}

void MPI_RSEND(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void MPI_SCAN(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm, ierr);
}

void MPI_SCATTER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void MPI_SCATTERV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
}

void MPI_SEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Send(buf, count, datatype, dest, tag, comm, ierr);
}

void MPI_SENDRECV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
}

void MPI_SENDRECV_REPLACE(MPI_Fint * buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Sendrecv_replace( buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
}

void MPI_SSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Ssend(buf, count, datatype, dest, tag, comm, ierr);
}

void MPI_WAITALL(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitall(count, array_of_requests, array_of_statuses, ierr);
}

void MPI_WAITANY(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Waitany(count, array_of_requests, index, status, ierr);
}

void MPI_WAIT(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Wait(request, status, ierr);
}

void MPI_WAITSOME(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Waitsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void MPI_WIN_FLUSH(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush(rank, win, ierr);
}

void MPI_WIN_FLUSH_ALL(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_all(win, ierr);
}

void MPI_WIN_FLUSH_LOCAL(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local(rank, win, ierr);
}

void MPI_WIN_FLUSH_LOCAL_ALL(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_flush_local_all(win, ierr);
}

void MPI_WIN_LOCK(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock(lock_type, rank, assert, win, ierr);
}

void MPI_WIN_LOCK_ALL(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_lock_all(assert, win, ierr);
}

void MPI_WIN_SYNC(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_sync(win, ierr);
}

void MPI_WIN_WAIT(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_wait(win, ierr);
}

void MPI_ISEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Isend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_ISSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Issend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_IRSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_IBSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibsend(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_IRECV(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Irecv(buf, count, datatype, source, tag, comm, request, ierr);
}

#endif