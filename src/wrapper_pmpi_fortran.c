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
 *				contributors may be used to rank endorse or promote products derived from
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

#ifdef OMPI_MPI_H

extern void pmpi_init_(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len);
extern void pmpi_init_thread_(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len);
extern void pmpi_finalize_(MPI_Fint *ierr);
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
extern void pmpi_gather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_gatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
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
extern void pmpi_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_add_error_class_(MPI_Fint *errorclass, MPI_Fint *ierr);
extern void pmpi_add_error_code_(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr);
extern void pmpi_add_error_string_(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len);
extern void pmpi_iallgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_iallgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_alloc_mem_(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr);
extern void pmpi_iallreduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ialltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ialltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ialltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ibarrier_(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ibcast_(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_bsend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_buffer_attach_(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_buffer_detach_(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_cancel_(MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_cart_coords_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr);
extern void pmpi_cart_create_(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr);
extern void pmpi_cart_get_(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr);
extern void pmpi_cart_map_(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr);
extern void pmpi_cart_rank_(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr);
extern void pmpi_cart_shift_(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr);
extern void pmpi_cart_sub_(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr);
extern void pmpi_cartdim_get_(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr);
extern void pmpi_close_port_(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len);
extern void pmpi_comm_accept_(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len);
extern void pmpi_comm_call_errhandler_(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr);
extern void pmpi_comm_compare_(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr);
extern void pmpi_comm_connect_(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len);
extern void pmpi_comm_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_comm_create_keyval_(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr);
extern void pmpi_comm_create_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_comm_create_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_comm_delete_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr);
extern void pmpi_comm_disconnect_(MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_comm_dup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_comm_idup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_comm_dup_with_info_(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr);
extern void pmpi_comm_free_keyval_(MPI_Fint *comm_keyval, MPI_Fint *ierr);
extern void pmpi_comm_free_(MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_comm_get_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_dist_graph_create_(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint * newcomm, MPI_Fint *ierr);
extern void pmpi_dist_graph_create_adjacent_(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr);
extern void pmpi_dist_graph_neighbors_(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr);
extern void pmpi_dist_graph_neighbors_count_(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr);
extern void pmpi_comm_get_errhandler_(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr);
extern void pmpi_comm_get_info_(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr);
extern void pmpi_comm_get_name_(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len);
extern void pmpi_comm_get_parent_(MPI_Fint *parent, MPI_Fint *ierr);
extern void pmpi_comm_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr);
extern void pmpi_comm_join_(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr);
extern void pmpi_comm_rank_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr);
extern void pmpi_comm_remote_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr);
extern void pmpi_comm_remote_size_(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_comm_set_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr);
extern void pmpi_comm_set_errhandler_(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_comm_set_info_(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr);
extern void pmpi_comm_set_name_(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len);
extern void pmpi_comm_size_(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_comm_spawn_(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len);
extern void pmpi_comm_spawn_multiple_(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len);
extern void pmpi_comm_test_inter_(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_compare_and_swap_(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_dims_create_(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr);
extern void pmpi_errhandler_free_(MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_error_class_(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr);
extern void pmpi_error_string_(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len);
extern void pmpi_fetch_and_op_(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_iexscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_call_errhandler_(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr);
extern void pmpi_file_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_file_set_errhandler_( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_file_get_errhandler_( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_file_open_(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len);
extern void pmpi_file_close_(MPI_Fint *fh, MPI_Fint *ierr);
extern void pmpi_file_delete_(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len);
extern void pmpi_file_set_size_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_file_preallocate_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_file_get_size_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_file_get_group_(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr);
extern void pmpi_file_get_amode_(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr);
extern void pmpi_file_set_info_(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr);
extern void pmpi_file_get_info_(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr);
extern void pmpi_file_set_view_(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_file_get_view_(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_file_read_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_read_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_iread_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iwrite_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iread_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iwrite_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_read_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_read_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_iread_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iwrite_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iread_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iwrite_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_seek_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr);
extern void pmpi_file_get_position_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr);
extern void pmpi_file_get_byte_offset_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr);
extern void pmpi_file_read_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_iread_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_iwrite_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_file_read_ordered_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_ordered_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_seek_shared_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr);
extern void pmpi_file_get_position_shared_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr);
extern void pmpi_file_read_at_all_begin_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_read_at_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_at_all_begin_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_write_at_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_read_all_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_read_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_all_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_write_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_read_ordered_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_read_ordered_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_write_ordered_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
extern void pmpi_file_write_ordered_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_file_get_type_extent_(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr);
extern void pmpi_file_set_atomicity_(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_file_get_atomicity_(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_finalized_(MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_free_mem_(MPI_Fint *base, MPI_Fint *ierr);
extern void pmpi_igather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_igatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_get_address_(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr);
extern void pmpi_get_count_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
extern void pmpi_get_elements_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
extern void pmpi_get_elements_x_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
extern void pmpi_get_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_get_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_get_library_version_(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len);
extern void pmpi_get_processor_name_(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len);
extern void pmpi_get_version_(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr);
extern void pmpi_graph_create_(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr);
extern void pmpi_graph_get_(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr);
extern void pmpi_graph_map_(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr);
extern void pmpi_graph_neighbors_count_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr);
extern void pmpi_graph_neighbors_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr);
extern void pmpi_graphdims_get_(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr);
extern void pmpi_grequest_complete_(MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_grequest_start_(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_group_compare_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr);
extern void pmpi_group_difference_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_excl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_free_(MPI_Fint *group, MPI_Fint *ierr);
extern void pmpi_group_incl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_intersection_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_range_excl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_range_incl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_group_rank_(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr);
extern void pmpi_group_size_(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_group_translate_ranks_(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr);
extern void pmpi_group_union_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
extern void pmpi_improbe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_imrecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_info_create_(MPI_Fint *info, MPI_Fint *ierr);
extern void pmpi_info_delete_(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len);
extern void pmpi_info_dup_(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr);
extern void pmpi_info_free_(MPI_Fint *info, MPI_Fint *ierr);
extern void pmpi_info_get_(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len);
extern void pmpi_info_get_nkeys_(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr);
extern void pmpi_info_get_nthkey_(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len);
extern void pmpi_info_get_valuelen_(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len);
extern void pmpi_info_set_(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len);
extern void pmpi_initialized_(MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_intercomm_create_(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr);
extern void pmpi_intercomm_merge_(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr);
extern void pmpi_iprobe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_is_thread_main_(MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_lookup_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len);
extern void pmpi_mprobe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_mrecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_ineighbor_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ineighbor_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ineighbor_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ineighbor_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ineighbor_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_op_commutative_(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr);
extern void pmpi_op_create_(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr);
extern void pmpi_open_port_(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len);
extern void pmpi_op_free_(MPI_Fint *op, MPI_Fint *ierr);
extern void pmpi_pack_external_(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_pack_external_size_(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_pack_(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_pack_size_(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_pcontrol_(MPI_Fint *level, MPI_Fint *ierr);
extern void pmpi_probe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_publish_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len);
extern void pmpi_put_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_query_thread_(MPI_Fint *provided, MPI_Fint *ierr);
extern void pmpi_raccumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_recv_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ireduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ireduce_scatter_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_reduce_scatter_block_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_ireduce_scatter_block_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_register_datarep_(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_request_free_(MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_request_get_status_(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_rget_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_rget_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_rput_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_cout, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_rsend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_iscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_iscatter_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_iscatterv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_send_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_ssend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_start_(MPI_Fint *request, MPI_Fint *ierr);
extern void pmpi_startall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr);
extern void pmpi_status_set_cancelled_(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_status_set_elements_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
extern void pmpi_status_set_elements_x_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
extern void pmpi_testall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr);
extern void pmpi_testany_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_test_(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_test_cancelled_(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_testsome_(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr);
extern void pmpi_topo_test_(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
extern void pmpi_type_commit_(MPI_Fint *type, MPI_Fint *ierr);
extern void pmpi_type_contiguous_(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_darray_(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_f90_complex_(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_f90_integer_(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_f90_real_(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_hindexed_block_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_hindexed_(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_hvector_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_keyval_(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr);
extern void pmpi_type_create_indexed_block_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_struct_(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_subarray_(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_create_resized_(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_delete_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr);
extern void pmpi_type_dup_(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_free_(MPI_Fint *type, MPI_Fint *ierr);
extern void pmpi_type_free_keyval_(MPI_Fint *type_keyval, MPI_Fint *ierr);
extern void pmpi_type_get_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_type_get_contents_(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr);
extern void pmpi_type_get_envelope_(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr);
extern void pmpi_type_get_extent_(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr);
extern void pmpi_type_get_extent_x_(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr);
extern void pmpi_type_get_name_(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len);
extern void pmpi_type_get_true_extent_(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr);
extern void pmpi_type_get_true_extent_x_(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr);
extern void pmpi_type_indexed_(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_type_match_size_(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr);
extern void pmpi_type_set_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr);
extern void pmpi_type_set_name_(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len);
extern void pmpi_type_size_(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_type_size_x_(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_type_vector_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
extern void pmpi_unpack_(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr);
extern void pmpi_unpublish_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len);
extern void pmpi_unpack_external_(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len);
extern void pmpi_win_allocate_(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_allocate_shared_(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_attach_(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr);
extern void pmpi_win_call_errhandler_(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr);
extern void pmpi_win_complete_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_create_(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_create_dynamic_(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_win_create_keyval_(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr);
extern void pmpi_win_delete_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr);
extern void pmpi_win_detach_(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr);
extern void pmpi_win_fence_(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_free_(MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_free_keyval_(MPI_Fint *win_keyval, MPI_Fint *ierr);
extern void pmpi_win_get_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_win_get_errhandler_(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_win_get_group_(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr);
extern void pmpi_win_get_info_(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr);
extern void pmpi_win_get_name_(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len);
extern void pmpi_win_post_(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_set_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr);
extern void pmpi_win_set_errhandler_(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr);
extern void pmpi_win_set_info_(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr);
extern void pmpi_win_set_name_(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len);
extern void pmpi_win_shared_query_(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr);
extern void pmpi_win_start_(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_test_(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr);
extern void pmpi_win_unlock_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr);
extern void pmpi_win_unlock_all_(MPI_Fint *win, MPI_Fint *ierr);

static void FMPI_Init(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len)
{
	pmpi_init_(argc, argv, ierr, argv_len);
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Init()\n", debug_rank);
#endif
	start_cntd();
	call_start(__MPI_INIT, MPI_COMM_WORLD, MPI_NONE);
	call_end(__MPI_INIT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Init()\n", debug_rank);
#endif
}

static void FMPI_Init_thread(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len)
{
	pmpi_init_thread_(argc, argv, required, provided, ierr, argv_len);
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Init_thread()\n", debug_rank);
#endif
	start_cntd();
	call_start(__MPI_INIT_THREAD, MPI_COMM_WORLD, MPI_NONE);
    call_end(__MPI_INIT_THREAD, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Init_thread()\n", debug_rank);
#endif
}

static void FMPI_Finalize(MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Finalize()\n", debug_rank);
#endif
	call_start(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);

	int local_ierr;
	MPI_Fint world = MPI_Comm_c2f(MPI_COMM_WORLD);
	pmpi_barrier_(&world, &local_ierr);

	call_end(__MPI_FINALIZE, MPI_COMM_WORLD, MPI_NONE);
	stop_cntd();

#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Finalize()\n", debug_rank);
#endif

	pmpi_finalize_(ierr);
}

// FORTRAN ABI Interfaces
// Lowercase
void mpi_init(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init(argc, argv, ierr, argv_len);
}

void mpi_init_thread(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init_thread(argc, argv, required, provided, ierr, argv_len);
}

void mpi_finalize(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

// Lowercase - single underscore
void mpi_init_(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init(argc, argv, ierr, argv_len);
}

void mpi_init_thread_(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init_thread(argc, argv, required, provided, ierr, argv_len);
}

void mpi_finalize_(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

// Lowercase - double underscore
void mpi_init__(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init(argc, argv, ierr, argv_len);
}

void mpi_init_thread__(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init_thread(argc, argv, required, provided, ierr, argv_len);
}

void mpi_finalize__(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

// Uppercase
void MPI_INIT(MPI_Fint *argc, char *argv, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init(argc, argv, ierr, argv_len);
}

void MPI_INIT_THREAD(MPI_Fint *argc, char *argv, MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr, MPI_Fint argv_len)
{
	FMPI_Init_thread(argc, argv, required, provided, ierr, argv_len);
}

void MPI_FINALIZE(MPI_Fint *ierr)
{
	FMPI_Finalize(ierr);
}

#ifndef DISABLE_PROFILING_MPI

static void FMPI_Allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Allgather()\n", debug_rank);
#endif
	call_start(__MPI_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLGATHER, sendcount, &sendtype_f2c, MPI_ALL, recvcount, &recvtype_f2c, MPI_ALL);
	pmpi_allgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Allgather()\n", debug_rank);
#endif
}

static void FMPI_Allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Allgatherv()\n", debug_rank);
#endif
  	call_start(__MPI_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLGATHERV, sendcount, &sendtype_f2c, MPI_ALL, recvcounts, &recvtype_f2c, MPI_ALLV);
	pmpi_allgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
	call_end(__MPI_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Allgatherv()\n", debug_rank);
#endif
}

static void FMPI_Allreduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Allreduce()\n", debug_rank);
#endif
	call_start(__MPI_ALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLREDUCE, count, &datatype_f2c, MPI_ALL, count, &datatype_f2c, MPI_ALL);
	pmpi_allreduce_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_ALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Allreduce()\n", debug_rank);
#endif
}

static void FMPI_Alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Alltoall()\n", debug_rank);
#endif
	call_start(__MPI_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLTOALL, sendcount, &sendtype_f2c, MPI_ALL, recvcount, &recvtype_f2c, MPI_ALL);
	pmpi_alltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Alltoall()\n", debug_rank);
#endif
}

static void FMPI_Alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Alltoallv()\n", debug_rank);
#endif
	call_start(__MPI_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLTOALLV, sendcounts, &sendtype_f2c, MPI_ALLV, recvcounts, &recvtype_f2c, MPI_ALLV);
	pmpi_alltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
	call_end(__MPI_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Alltoallv()\n", debug_rank);
#endif
}

static void FMPI_Alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Alltoallw()\n", debug_rank);
#endif
	call_start(__MPI_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	int my_rank, comm_size;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	PMPI_Comm_size(MPI_Comm_f2c(*comm), &comm_size);
	MPI_Datatype sendtypes_f2c[comm_size], recvtypes_f2c[comm_size];
	int i;
	for(i = 0; i < comm_size; i++)
	{
		sendtypes_f2c[i] = MPI_Type_f2c(sendtypes[i]);
		recvtypes_f2c[i] = MPI_Type_f2c(recvtypes[i]);
	}
	add_network(MPI_Comm_f2c(*comm), __MPI_ALLTOALLW, sendcounts, sendtypes_f2c, MPI_ALLW, recvcounts, recvtypes_f2c, MPI_ALLW);
	pmpi_alltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
	call_end(__MPI_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Alltoallw()\n", debug_rank);
#endif
}

static void FMPI_Barrier(MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Barrier()\n", debug_rank);
#endif
	call_start(__MPI_BARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_barrier_(comm, ierr);
	call_end(__MPI_BARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Barrier()\n", debug_rank);
#endif
}

static void FMPI_Bcast(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Bcast()\n", debug_rank);
#endif
	call_start(__MPI_BCAST, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == *root)
		add_network(MPI_Comm_f2c(*comm), __MPI_BCAST, count, &datatype_f2c, MPI_ALL, NULL, NULL, MPI_NONE);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_BCAST, NULL, NULL, MPI_NONE, count, &datatype_f2c, *root);
	pmpi_bcast_(buffer, count, datatype, root, comm, ierr);
	call_end(__MPI_BCAST, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Bcast()\n", debug_rank);
#endif
}

static void FMPI_Comm_split(MPI_Fint *comm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_split()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SPLIT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_split_(comm, color, key, newcomm, ierr);
	call_end(__MPI_COMM_SPLIT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_split()\n", debug_rank);
#endif
}

static void FMPI_Comm_split_type(MPI_Fint *comm, MPI_Fint *split_type, MPI_Fint *key, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_split_type()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SPLIT_TYPE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_split_type_(comm, split_type, key, info, newcomm, ierr);
	call_end(__MPI_COMM_SPLIT_TYPE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_split_type()\n", debug_rank);
#endif
}

static void FMPI_Exscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Exscan()\n", debug_rank);
#endif
	call_start(__MPI_EXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_exscan_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_EXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Exscan()\n", debug_rank);
#endif
}

static void FMPI_File_sync(MPI_Fint *fh, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_sync()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_sync_(fh, ierr);
	call_end(__MPI_FILE_SYNC, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_sync()\n", debug_rank);
#endif
}

static void FMPI_Gather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Gather()\n", debug_rank);
#endif
	call_start(__MPI_GATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_GATHER, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, MPI_ALL);
	}
	else
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_GATHER, sendcount, &sendtype_f2c, *root, NULL, NULL, MPI_NONE);
	}
	pmpi_gather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_GATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Gather()\n", debug_rank);
#endif
}

static void FMPI_Gatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Gatherv()\n", debug_rank);
#endif
	call_start(__MPI_GATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_GATHERV, NULL, NULL, MPI_NONE, recvcounts, &recvtype_f2c, MPI_ALLV);
	}
	else
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_GATHERV, sendcount, &sendtype_f2c, *root, NULL, NULL, MPI_NONE);
	}
	pmpi_gatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, ierr);
	call_end(__MPI_GATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Gatherv()\n", debug_rank);
#endif
}

static void FMPI_Neighbor_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Neighbor_allgather()\n", debug_rank);
#endif
	call_start(__MPI_NEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_neighbor_allgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Neighbor_allgather()\n", debug_rank);
#endif
}

static void FMPI_Neighbor_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Neighbor_allgatherv()\n", debug_rank);
#endif
	call_start(__MPI_NEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_neighbor_allgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Neighbor_allgatherv()\n", debug_rank);
#endif
}

static void FMPI_Neighbor_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Neighbor_alltoall()\n", debug_rank);
#endif
	call_start(__MPI_NEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_neighbor_alltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Neighbor_alltoall()\n", debug_rank);
#endif
}

static void FMPI_Neighbor_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Neighbor_alltoallv()\n", debug_rank);
#endif
	call_start(__MPI_NEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_neighbor_alltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Neighbor_alltoallv()\n", debug_rank);
#endif
}

static void FMPI_Neighbor_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Neighbor_alltoallw()\n", debug_rank);
#endif
	call_start(__MPI_NEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	pmpi_neighbor_alltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, ierr);
	call_end(__MPI_NEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Neighbor_alltoallw()\n", debug_rank);
#endif
}

static void FMPI_Reduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Reduce()\n", debug_rank);
#endif
	call_start(__MPI_REDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == *root)
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE, NULL, NULL, MPI_NONE, count, &datatype_f2c, MPI_ALL);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE, count, &datatype_f2c, *root, NULL, NULL, MPI_NONE);
	pmpi_reduce_(sendbuf, recvbuf, count, datatype, op, root, comm, ierr);
	call_end(__MPI_REDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Reduce()\n", debug_rank);
#endif
}

static void FMPI_Reduce_local(MPI_Fint *inbuf, MPI_Fint *inoutbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Reduce_local()\n", debug_rank);
#endif
	call_start(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_ALL);
	pmpi_reduce_local_(inbuf, inoutbuf, count, datatype, op, ierr);
	call_end(__MPI_REDUCE_LOCAL, MPI_COMM_WORLD, MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Reduce_local()\n", debug_rank);
#endif
}

static void FMPI_Reduce_scatter(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Reduce_scatter()\n", debug_rank);
#endif
	call_start(__MPI_REDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == 0)
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE_SCATTER, recvcounts, &datatype_f2c, MPI_ALLV, recvcounts, &datatype_f2c, MPI_ALLV);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE_SCATTER, &recvcounts[my_rank], &datatype_f2c, 0, &recvcounts[my_rank], &datatype_f2c, 0);
	pmpi_reduce_scatter_(sendbuf, recvbuf, recvcounts, datatype, op, comm, ierr);
	call_end(__MPI_REDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Reduce_scatter()\n", debug_rank);
#endif
}

static void FMPI_Scan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Scan()\n", debug_rank);
#endif
	call_start(__MPI_SCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_scan_(sendbuf, recvbuf, count, datatype, op, comm, ierr);
	call_end(__MPI_SCAN, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Scan()\n", debug_rank);
#endif
}

static void FMPI_Scatter(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Scatter()\n", debug_rank);
#endif
	call_start(__MPI_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_SCATTER, sendcount, &sendtype_f2c, MPI_ALL, NULL, NULL, MPI_NONE);
	}
	else
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_SCATTER, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, *root);
	}
	pmpi_scatter_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Scatter()\n", debug_rank);
#endif
}

static void FMPI_Scatterv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Scatterv()\n", debug_rank);
#endif
	call_start(__MPI_SCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_SCATTERV, sendcounts, &sendtype_f2c, MPI_ALLV, NULL, NULL, MPI_NONE);
	}
	else
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_SCATTERV, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, *root);
	}
	pmpi_scatterv_(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, ierr);
	call_end(__MPI_SCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Scatterv()\n", debug_rank);
#endif
}

static void FMPI_Waitall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Waitall()\n", debug_rank);
#endif
	call_start(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitall_(count, array_of_requests, array_of_statuses, ierr);
	call_end(__MPI_WAITALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Waitall()\n", debug_rank);
#endif
}

static void FMPI_Waitany(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Waitany()\n", debug_rank);
#endif
	call_start(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitany_(count, array_of_requests, index, status, ierr);
	call_end(__MPI_WAITANY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Waitany()\n", debug_rank);
#endif
}

static void FMPI_Wait(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Wait()\n", debug_rank);
#endif
	call_start(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_wait_(request, status, ierr);
	call_end(__MPI_WAIT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Wait()\n", debug_rank);
#endif
}

static void FMPI_Waitsome(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Waitsome()\n", debug_rank);
#endif
	call_start(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_waitsome_(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
	call_end(__MPI_WAITSOME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Waitsome()\n", debug_rank);
#endif
}

static void FMPI_Win_flush(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_flush()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_(rank, win, ierr);
	call_end(__MPI_WIN_FLUSH, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_flush()\n", debug_rank);
#endif
}

static void FMPI_Win_flush_all(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_flush_all()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_all_(win, ierr);
	call_end(__MPI_WIN_FLUSH_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_flush_all()\n", debug_rank);
#endif
}

static void FMPI_Win_flush_local(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_flush_local()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_local_(rank, win, ierr);
	call_end(__MPI_WIN_FLUSH_LOCAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_flush_local()\n", debug_rank);
#endif
}

static void FMPI_Win_flush_local_all(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_flush_local_all()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_flush_local_all_(win, ierr);
	call_end(__MPI_WIN_FLUSH_LOCAL_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_flush_local_all()\n", debug_rank);
#endif
}

static void FMPI_Win_lock(MPI_Fint *lock_type, MPI_Fint *rank, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_lock()\n", debug_rank);
#endif
	call_start(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_lock_(lock_type, rank, assert, win, ierr);
	call_end(__MPI_WIN_LOCK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_lock()\n", debug_rank);
#endif
}

static void FMPI_Win_lock_all(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_lock_all()\n", debug_rank);
#endif
	call_start(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_lock_all_(assert, win, ierr);
	call_end(__MPI_WIN_LOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_lock_all()\n", debug_rank);
#endif
}

static void FMPI_Win_sync(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_sync()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_sync_(win, ierr);
	call_end(__MPI_WIN_SYNC, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_sync()\n", debug_rank);
#endif
}

static void FMPI_Win_wait(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_wait()\n", debug_rank);
#endif
	call_start(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_wait_(win, ierr);
	call_end(__MPI_WIN_WAIT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_wait()\n", debug_rank);
#endif
}

// FORTRAN ABI Interfaces
// Lowercase
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

// Lowercase - single underscore
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

// Lowercase - double underscore
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

// Uppercase
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

#ifndef DISABLE_P2P_MPI

static void FMPI_Send(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Send(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_SEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_SEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_send_(buf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_SEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Send(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Sendrecv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Sendrecv(FROM_RANK:%d-TO_RANK:%d)\n", debug_rank, *source, *dest);
#endif
	call_start(__MPI_SENDRECV, MPI_Comm_f2c(*comm), MPI_NONE);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_SENDRECV, sendcount, &sendtype_f2c, *dest, recvcount, &recvtype_f2c, *source);
	pmpi_sendrecv_(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status, ierr);
	call_end(__MPI_SENDRECV, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Sendrecv(FROM_RANK:%d-TO_RANK:%d)\n", debug_rank, *source, *dest);
#endif
}

static void FMPI_Sendrecv_replace(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Sendrecv_replace(FROM_RANK:%d-TO_RANK:%d)\n", debug_rank, *source, *dest);
#endif
	call_start(__MPI_SENDRECV_REPLACE, MPI_Comm_f2c(*comm), MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_SENDRECV_REPLACE, count, &datatype_f2c, *dest, count, &datatype_f2c, *source);
	pmpi_sendrecv_replace_(buf, count, datatype, dest, sendtag, source, recvtag, comm, status, ierr);
	call_end(__MPI_SENDRECV_REPLACE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Sendrecv_replace(FROM_RANK:%d-TO_RANK:%d)\n", debug_rank, *source, *dest);
#endif
}

static void FMPI_Ssend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ssend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_SSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_SSEND, (int*) count, &datatype_f2c, *dest, NULL, &datatype_f2c, MPI_NONE);
	pmpi_ssend_(buf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_SSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ssend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Bsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Bsend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_BSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_BSEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_bsend_(buf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_BSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Bsend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Rsend(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Rsend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_RSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_RSEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_rsend_(ibuf, count, datatype, dest, tag, comm, ierr);
	call_end(__MPI_RSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Rsend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Recv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Recv(FROM_RANK:%d-TO_RANK:%d\n"), *source);
#endif
	call_start(__MPI_RECV, MPI_Comm_f2c(*comm), *source);
    MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
    add_network(MPI_Comm_f2c(*comm), __MPI_RECV, NULL, NULL, MPI_NONE, count, &datatype_f2c, *source);
	pmpi_recv_(buf, count, datatype, source, tag, comm, status, ierr);
	call_end(__MPI_RECV, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Recv(FROM_RANK:%d-TO_RANK:%d\n"), *sorce);
#endif
}

static void FMPI_Probe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Probe(FROM_RANK:%d-TO_RANK:%d\n"), *source);
#endif
	call_start(__MPI_PROBE, MPI_Comm_f2c(*comm), *source);
	pmpi_probe_(source, tag, comm, status, ierr);
    call_end(__MPI_PROBE, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Probe(FROM_RANK:%d-TO_RANK:%d\n"), *source);
#endif
}

static void FMPI_Isend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Isend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_ISEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ISEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_isend_(buf, count, datatype, dest, tag, comm, request, ierr);
	call_end(__MPI_ISEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Isend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Issend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Issend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_ISSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_ISSEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_issend_(buf, count, datatype, dest, tag, comm, request, ierr);
	call_end(__MPI_ISSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Issend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Irsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Irsend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_IRSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IRSEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_irsend_(buf, count, datatype, dest, tag, comm, request, ierr);
	call_end(__MPI_IRSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Irsend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Ibsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ibsend(RANK:%d)\n", debug_rank, *dest);
#endif
	call_start(__MPI_IBSEND, MPI_Comm_f2c(*comm), *dest);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IBSEND, count, &datatype_f2c, *dest, NULL, NULL, MPI_NONE);
	pmpi_ibsend_(buf, count, datatype, dest, tag, comm, request, ierr);
	call_end(__MPI_IBSEND, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ibsend(RANK:%d)\n", debug_rank, *dest);
#endif
}

static void FMPI_Irecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Irecv(FROM_RANK:%d-TO_RANK:%d)\n", *source);
#endif
	call_start(__MPI_IRECV, MPI_Comm_f2c(*comm), *source);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IRECV, NULL, NULL, MPI_NONE, count, &datatype_f2c, *source);
	pmpi_irecv_(buf, count, datatype, source, tag, comm, request, ierr);
	call_end(__MPI_IRECV, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Irecv(FROM_RANK:%d-TO_RANK:%d)\n", *source);
#endif
}

static void FMPI_Iprobe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iprobe(FROM_RANK:%d-TO_RANK:%d)\n", *source);
#endif
	call_start(__MPI_IPROBE, MPI_Comm_f2c(*comm), *source);
	pmpi_iprobe_(source, tag, comm, flag, status, ierr);
    call_end(__MPI_IPROBE, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iprobe(FROM_RANK:%d-TO_RANK:%d)\n", *source);
#endif
}

// FORTRAN ABI Interfaces
// Lowercase
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

void mpi_bsend(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_rsend(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_recv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_probe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Probe(source, tag, comm, status, ierr);
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

void mpi_iprobe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Iprobe(source, tag, comm, flag, status, ierr);
}

// Lowercase - single underscore
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

void mpi_bsend_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_rsend_(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_recv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_probe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Probe(source, tag, comm, status, ierr);
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

void mpi_iprobe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Iprobe(source, tag, comm, flag, status, ierr);
}

// Lowercase - double underscore
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

void mpi_bsend__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void mpi_rsend__(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void mpi_recv__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void mpi_probe__(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Probe(source, tag, comm, status, ierr);
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

void mpi_iprobe__(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Iprobe(source, tag, comm, flag, status, ierr);
}

// Uppercase
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

void MPI_BSEND(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Bsend(buf, count, datatype, dest, tag, comm, ierr);
}

void MPI_RSEND(MPI_Fint *ibuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Rsend(ibuf, count, datatype, dest, tag, comm, ierr);
}

void MPI_RECV(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Recv(buf, count, datatype, source, tag, comm, status, ierr);
}

void MPI_PROBE(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Probe(source, tag, comm, status, ierr);
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

void MPI_IPROBE(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Iprobe(source, tag, comm, flag, status, ierr);
}

#endif

#ifndef DISABLE_ACCESSORY_MPI

static void FMPI_Accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Accumulate()\n", debug_rank);
#endif
	call_start(__MPI_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_accumulate_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
    call_end(__MPI_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Accumulate()\n", debug_rank);
#endif
}

static void FMPI_Add_error_class(MPI_Fint *errorclass, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Add_error_class()\n", debug_rank);
#endif
	call_start(__MPI_ADD_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_add_error_class_(errorclass, ierr);
    call_end(__MPI_ADD_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Add_error_class()\n", debug_rank);
#endif
}

static void FMPI_Add_error_code(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Add_error_code()\n", debug_rank);
#endif
	call_start(__MPI_ADD_ERROR_CODE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_add_error_code_(errorclass, errorcode, ierr);
    call_end(__MPI_ADD_ERROR_CODE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Add_error_code()\n", debug_rank);
#endif
}

static void FMPI_Add_error_string(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Add_error_string()\n", debug_rank);
#endif
	call_start(__MPI_ADD_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	pmpi_add_error_string_(errorcode, string, ierr, string_len);
    call_end(__MPI_ADD_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Add_error_string()\n", debug_rank);
#endif
}

static void FMPI_Iallgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iallgather()\n", debug_rank);
#endif
	call_start(__MPI_IALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLGATHER, sendcount, &sendtype_f2c, MPI_ALL, recvcount, &recvtype_f2c, MPI_ALL);
	pmpi_iallgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
    call_end(__MPI_IALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iallgather()\n", debug_rank);
#endif
}

static void FMPI_Iallgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iallgatherv()\n", debug_rank);
#endif
	call_start(__MPI_IALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLGATHERV, sendcount, &sendtype_f2c, MPI_ALL, recvcounts, &recvtype_f2c, MPI_ALLV);
	pmpi_iallgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
    call_end(__MPI_IALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iallgatherv()\n", debug_rank);
#endif
}

static void FMPI_Alloc_mem(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Alloc_mem()\n", debug_rank);
#endif
	call_start(__MPI_ALLOC_MEM, MPI_COMM_WORLD, MPI_NONE);
	pmpi_alloc_mem_(size, info, baseptr, ierr);
    call_end(__MPI_ALLOC_MEM, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Alloc_mem()\n", debug_rank);
#endif
}

static void FMPI_Iallreduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iallreduce()\n", debug_rank);
#endif
	call_start(__MPI_IALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLREDUCE, count, &datatype_f2c, MPI_ALL, count, &datatype_f2c, MPI_ALL);
	pmpi_iallreduce_(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
    call_end(__MPI_IALLREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iallreduce()\n", debug_rank);
#endif
}

static void FMPI_Ialltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ialltoall()\n", debug_rank);
#endif
	call_start(__MPI_IALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLTOALL, sendcount, &sendtype_f2c, MPI_ALL, recvcount, &recvtype_f2c, MPI_ALL);
	pmpi_ialltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
    call_end(__MPI_IALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ialltoall()\n", debug_rank);
#endif
}

static void FMPI_Ialltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ialltoallv()\n", debug_rank);
#endif
	call_start(__MPI_IALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
	MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLTOALLV, sendcounts, &sendtype_f2c, MPI_ALLV, recvcounts, &recvtype_f2c, MPI_ALLV);
	pmpi_ialltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
    call_end(__MPI_IALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ialltoallv()\n", debug_rank);
#endif
}

static void FMPI_Ialltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ialltoallw()\n", debug_rank);
#endif
	call_start(__MPI_IALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	int comm_size;
	PMPI_Comm_size(MPI_Comm_f2c(*comm), &comm_size);
	MPI_Datatype sendtypes_f2c[comm_size], recvtypes_f2c[comm_size];
	int i;
	for(i = 0; i < comm_size; i++)
	{
		sendtypes_f2c[i] = MPI_Type_f2c(sendtypes[i]);
		recvtypes_f2c[i] = MPI_Type_f2c(recvtypes[i]);
	}
	add_network(MPI_Comm_f2c(*comm), __MPI_IALLTOALLW, sendcounts, sendtypes_f2c, MPI_ALLW, recvcounts, recvtypes_f2c, MPI_ALLW);
	pmpi_ialltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
    call_end(__MPI_IALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ialltoallw()\n", debug_rank);
#endif
}

static void FMPI_Ibarrier(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ibarrier()\n", debug_rank);
#endif
	call_start(__MPI_IBARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_ibarrier_(comm, request, ierr);
    call_end(__MPI_IBARRIER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ibarrier()\n", debug_rank);
#endif
}

static void FMPI_Ibcast(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ibcast()\n", debug_rank);
#endif
	call_start(__MPI_IBCAST, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == *root)
		add_network(MPI_Comm_f2c(*comm), __MPI_IBCAST, count, &datatype_f2c, MPI_ALL, NULL, NULL, MPI_NONE);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_IBCAST, NULL, NULL, MPI_NONE, count, &datatype_f2c, *root);
	pmpi_ibcast_(buffer, count, datatype, root, comm, request, ierr);
    call_end(__MPI_IBCAST, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ibcast()\n", debug_rank);
#endif
}

static void FMPI_Bsend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Bsend_init()\n", debug_rank);
#endif
    call_start(__MPI_BSEND_INIT, MPI_Comm_f2c(*comm), *dest);
	pmpi_bsend_init_(buf, count, datatype, dest, tag, comm, request, ierr);
       call_end(__MPI_BSEND_INIT, MPI_Comm_f2c(*comm), *dest);
	#ifdef DEBUG_MPI
		printf("[DEBUG][RANK:%d] End MPI_Bsend_init()\n", debug_rank);
	#endif
}

static void FMPI_Buffer_attach(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Buffer_attach()\n", debug_rank);
#endif
	call_start(__MPI_BUFFER_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_buffer_attach_(buffer, size, ierr);
    call_end(__MPI_BUFFER_ATTACH, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Buffer_attach()\n", debug_rank);
#endif
}

static void FMPI_Buffer_detach(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Buffer_detach()\n", debug_rank);
#endif
	call_start(__MPI_BUFFER_DETACH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_buffer_detach_(buffer, size, ierr);
    call_end(__MPI_BUFFER_DETACH, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Buffer_detach()\n", debug_rank);
#endif
}

static void FMPI_Cancel(MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cancel()\n", debug_rank);
#endif
	call_start(__MPI_CANCEL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_cancel_(request, ierr);
    call_end(__MPI_CANCEL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cancel()\n", debug_rank);
#endif
}

static void FMPI_Cart_coords(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_coords()\n", debug_rank);
#endif
	call_start(__MPI_CART_COORDS, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_coords_(comm, rank, maxdims, coords, ierr);
    call_end(__MPI_CART_COORDS, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_coords()\n", debug_rank);
#endif
}

static void FMPI_Cart_create(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_create()\n", debug_rank);
#endif
	call_start(__MPI_CART_CREATE, MPI_Comm_f2c(*old_comm), MPI_NONE);
	pmpi_cart_create_(old_comm, ndims, dims, periods, reorder, comm_cart, ierr);
    call_end(__MPI_CART_CREATE, MPI_Comm_f2c(*old_comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_create()\n", debug_rank);
#endif
}

static void FMPI_Cart_get(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_get()\n", debug_rank);
#endif
	call_start(__MPI_CART_GET, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_get_(comm, maxdims, dims, periods, coords, ierr);
    call_end(__MPI_CART_GET, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_get()\n", debug_rank);
#endif
}

static void FMPI_Cart_map(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_map()\n", debug_rank);
#endif
	call_start(__MPI_CART_MAP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_map_(comm, ndims, dims, periods, newrank, ierr);
    call_end(__MPI_CART_MAP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_map()\n", debug_rank);
#endif
}

static void FMPI_Cart_rank(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_rank()\n", debug_rank);
#endif
	call_start(__MPI_CART_RANK, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_rank_(comm, coords, rank, ierr);
    call_end(__MPI_CART_RANK, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_rank()\n", debug_rank);
#endif
}

static void FMPI_Cart_shift(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_shift()\n", debug_rank);
#endif
	call_start(__MPI_CART_SHIFT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_shift_(comm, direction, disp, rank_source, rank_dest, ierr);
    call_end(__MPI_CART_SHIFT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_shift()\n", debug_rank);
#endif
}

static void FMPI_Cart_sub(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cart_sub()\n", debug_rank);
#endif
	call_start(__MPI_CART_SUB, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cart_sub_(comm, remain_dims, new_comm, ierr);
    call_end(__MPI_CART_SUB, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cart_sub()\n", debug_rank);
#endif
}

static void FMPI_Cartdim_get(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Cartdim_get()\n", debug_rank);
#endif
	call_start(__MPI_CARTDIM_GET, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_cartdim_get_(comm, ndims, ierr);
    call_end(__MPI_CARTDIM_GET, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Cartdim_get()\n", debug_rank);
#endif
}

static void FMPI_Close_port(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Close_port()\n", debug_rank);
#endif
	call_start(__MPI_CLOSE_PORT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_close_port_(port_name, ierr, port_name_len);
    call_end(__MPI_CLOSE_PORT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI__()\n", debug_rank);
#endif
}

static void FMPI_Comm_accept(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_accept()\n", debug_rank);
#endif
	call_start(__MPI_COMM_ACCEPT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_accept_(port_name, info, root, comm, newcomm, ierr, port_name_len);
    call_end(__MPI_COMM_ACCEPT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_accept()\n", debug_rank);
#endif
}

static void FMPI_Comm_call_errhandler(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_call_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CALL_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_call_errhandler_(comm, errorcode, ierr);
    call_end(__MPI_COMM_CALL_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_call_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Comm_compare(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_compare()\n", debug_rank);
#endif
	call_start(__MPI_COMM_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_compare_(comm1, comm2, result, ierr);
    call_end(__MPI_COMM_COMPARE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_compare()\n", debug_rank);
#endif
}

static void FMPI_Comm_connect(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_connect()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CONNECT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_connect_(port_name, info, root, comm, newcomm, ierr, port_name_len);
    call_end(__MPI_COMM_CONNECT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_connect()\n", debug_rank);
#endif
}

static void FMPI_Comm_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_create_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_create_errhandler_(function, errhandler, ierr);
    call_end(__MPI_COMM_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_create_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Comm_create_keyval(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_create_keyval()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_create_keyval_(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr);
    call_end(__MPI_COMM_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_create_keyval()\n", debug_rank);
#endif
}

static void FMPI_Comm_create_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_create_group()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CREATE_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_create_group_(comm, group, tag, newcomm, ierr);
    call_end(__MPI_COMM_CREATE_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_create_group()\n", debug_rank);
#endif
}

static void FMPI_Comm_create(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_create()\n", debug_rank);
#endif
	call_start(__MPI_COMM_CREATE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_create_(comm, group, newcomm, ierr);
    call_end(__MPI_COMM_CREATE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_create()\n", debug_rank);
#endif
}

static void FMPI_Comm_delete_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_delete_attr()\n", debug_rank);
#endif
	call_start(__MPI_COMM_DELETE_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_delete_attr_(comm, comm_keyval, ierr);
    call_end(__MPI_COMM_DELETE_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_delete_attr()\n", debug_rank);
#endif
}

static void FMPI_Comm_disconnect(MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_disconnect()\n", debug_rank);
#endif
	call_start(__MPI_COMM_DISCONNECT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_disconnect_(comm, ierr);
    call_end(__MPI_COMM_DISCONNECT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_disconnect()\n", debug_rank);
#endif
}

static void FMPI_Comm_dup(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_dup()\n", debug_rank);
#endif
	call_start(__MPI_COMM_DUP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_dup_(comm, newcomm, ierr);
    call_end(__MPI_COMM_DUP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_dup()\n", debug_rank);
#endif
}

static void FMPI_Comm_idup(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_idup()\n", debug_rank);
#endif
	call_start(__MPI_COMM_IDUP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_idup_(comm, newcomm, request, ierr);
    call_end(__MPI_COMM_IDUP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_idup()\n", debug_rank);
#endif
}

static void FMPI_Comm_dup_with_info(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_dup_with_info()\n", debug_rank);
#endif
	call_start(__MPI_COMM_DUP_WITH_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_dup_with_info_(comm, info, newcomm, ierr);
    call_end(__MPI_COMM_DUP_WITH_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_dup_with_info()\n", debug_rank);
#endif
}

static void FMPI_Comm_free_keyval(MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_free_keyval()\n", debug_rank);
#endif
	call_start(__MPI_COMM_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_free_keyval_(comm_keyval, ierr);
    call_end(__MPI_COMM_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_free_keyval()\n", debug_rank);
#endif
}

static void FMPI_Comm_free(MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_free()\n", debug_rank);
#endif
	call_start(__MPI_COMM_FREE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_free_(comm, ierr);
    call_end(__MPI_COMM_FREE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_free()\n", debug_rank);
#endif
}

static void FMPI_Comm_get_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_get_attr()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GET_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_get_attr_(comm, comm_keyval, attribute_val, flag, ierr);
    call_end(__MPI_COMM_GET_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_get_attr()\n", debug_rank);
#endif
}

static void FMPI_Dist_graph_create(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *newcomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Dist_graph_create()\n", debug_rank);
#endif
	call_start(__MPI_DIST_GRAPH_CREATE, MPI_Comm_f2c(*comm_old), MPI_NONE);
	pmpi_dist_graph_create_(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm, ierr);
    call_end(__MPI_DIST_GRAPH_CREATE, MPI_Comm_f2c(*comm_old), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Dist_graph_create()\n", debug_rank);
#endif
}

static void FMPI_Dist_graph_create_adjacent(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Dist_graph_create_adjacent()\n", debug_rank);
#endif
	call_start(__MPI_DIST_GRAPH_CREATE_ADJACENT, MPI_Comm_f2c(*comm_old), MPI_NONE);
	pmpi_dist_graph_create_adjacent_(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph, ierr);
    call_end(__MPI_DIST_GRAPH_CREATE_ADJACENT, MPI_Comm_f2c(*comm_old), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Dist_graph_create_adjacent()\n", debug_rank);
#endif
}

static void FMPI_Dist_graph_neighbors(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Dist_graph_neighbors()\n", debug_rank);
#endif
	call_start(__MPI_DIST_GRAPH_NEIGHBORS, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_dist_graph_neighbors_(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights, ierr);
    call_end(__MPI_DIST_GRAPH_NEIGHBORS, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Dist_graph_neighbors()\n", debug_rank);
#endif
}

static void FMPI_Dist_graph_neighbors_count(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Dist_graph_neighbors_count()\n", debug_rank);
#endif
	call_start(__MPI_DIST_GRAPH_NEIGHBORS_COUNT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_dist_graph_neighbors_count_(comm, inneighbors, outneighbors, weighted, ierr);
    call_end(__MPI_DIST_GRAPH_NEIGHBORS_COUNT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Dist_graph_neighbors_count()\n", debug_rank);
#endif
}

static void FMPI_Comm_get_errhandler(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_get_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GET_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_get_errhandler_(comm, erhandler, ierr);
    call_end(__MPI_COMM_GET_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_get_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Comm_get_info(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_get_info()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GET_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_get_info_(comm, info_used, ierr);
    call_end(__MPI_COMM_GET_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_get_info()\n", debug_rank);
#endif
}

static void FMPI_Comm_get_name(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_get_name()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GET_NAME, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_get_name_(comm, comm_name, resultlen, ierr, name_len);
    call_end(__MPI_COMM_GET_NAME, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_get_name()\n", debug_rank);
#endif
}

static void FMPI_Comm_get_parent(MPI_Fint *parent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_get_parent()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GET_PARENT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_comm_get_parent_(parent, ierr);
    call_end(__MPI_COMM_GET_PARENT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_get_parent()\n", debug_rank);
#endif
}

static void FMPI_Comm_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_group()\n", debug_rank);
#endif
	call_start(__MPI_COMM_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_group_(comm, group, ierr);
    call_end(__MPI_COMM_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_group()\n", debug_rank);
#endif
}

static void FMPI_Comm_join(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_join()\n", debug_rank);
#endif
	call_start(__MPI_COMM_JOIN, MPI_Comm_f2c(*intercomm), MPI_NONE);
	pmpi_comm_join_(fd, intercomm, ierr);
    call_end(__MPI_COMM_JOIN, MPI_Comm_f2c(*intercomm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_join()\n", debug_rank);
#endif
}

static void FMPI_Comm_rank(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_rank()\n", debug_rank);
#endif
	call_start(__MPI_COMM_RANK, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_rank_(comm, rank, ierr);
    call_end(__MPI_COMM_RANK, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_rank()\n", debug_rank);
#endif
}

static void FMPI_Comm_remote_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_remote_group()\n", debug_rank);
#endif
	call_start(__MPI_COMM_REMOTE_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_remote_group_(comm, group, ierr);
    call_end(__MPI_COMM_REMOTE_GROUP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_remote_group()\n", debug_rank);
#endif
}

static void FMPI_Comm_remote_size(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_remote_size()\n", debug_rank);
#endif
	call_start(__MPI_COMM_REMOTE_SIZE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_remote_size_(comm, size, ierr);
    call_end(__MPI_COMM_REMOTE_SIZE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_remote_size()\n", debug_rank);
#endif
}

static void FMPI_Comm_set_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_set_attr()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SET_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_set_attr_(comm, comm_keyval, attribute_val, ierr);
    call_end(__MPI_COMM_SET_ATTR, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_set_attr()\n", debug_rank);
#endif
}

static void FMPI_Comm_set_errhandler(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_set_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SET_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_set_errhandler_(comm, errhandler, ierr);
    call_end(__MPI_COMM_SET_ERRHANDLER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_set_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Comm_set_info(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_set_info()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SET_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_set_info_(comm, info, ierr);
    call_end(__MPI_COMM_SET_INFO, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_set_info()\n", debug_rank);
#endif
}

static void FMPI_Comm_set_name(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_set_name()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SET_NAME, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_set_name_(comm, comm_name, ierr, name_len);
    call_end(__MPI_COMM_SET_NAME, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_set_name()\n", debug_rank);
#endif
}

static void FMPI_Comm_size(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_size()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SIZE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_size_(comm, size, ierr);
    call_end(__MPI_COMM_SIZE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_size()\n", debug_rank);
#endif
}

static void FMPI_Comm_spawn(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_spawn()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SPAWN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_spawn_(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes, ierr, cmd_len, string_len);
    call_end(__MPI_COMM_SPAWN, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_spawn()\n", debug_rank);
#endif
}

static void FMPI_Comm_spawn_multiple(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_spawn_multiple()\n", debug_rank);
#endif
	call_start(__MPI_COMM_SPAWN_MULTIPLE, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_spawn_multiple_(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes, ierr, cmd_string_len, argv_string_len);
    call_end(__MPI_COMM_SPAWN_MULTIPLE, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_spawn_multiple()\n", debug_rank);
#endif
}

static void FMPI_Comm_test_inter(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Comm_test_inter()\n", debug_rank);
#endif
	call_start(__MPI_COMM_TEST_INTER, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_comm_test_inter_(comm, flag, ierr);
    call_end(__MPI_COMM_TEST_INTER, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Comm_test_inter()\n", debug_rank);
#endif
}

static void FMPI_Compare_and_swap(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Compare_and_swap()\n", debug_rank);
#endif
	call_start(__MPI_COMPARE_AND_SWAP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_compare_and_swap_(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win, ierr);
    call_end(__MPI_COMPARE_AND_SWAP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Compare_and_swap()\n", debug_rank);
#endif
}

static void FMPI_Dims_create(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Dims_create()\n", debug_rank);
#endif
	call_start(__MPI_DIMS_CREATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_dims_create_(nnodes, ndims, dims, ierr);
    call_end(__MPI_DIMS_CREATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Dims_create()\n", debug_rank);
#endif
}

static void FMPI_Errhandler_free(MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Errhandler_free()\n", debug_rank);
#endif
	call_start(__MPI_ERRHANDLER_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_errhandler_free_(errhandler, ierr);
    call_end(__MPI_ERRHANDLER_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Errhandler_free()\n", debug_rank);
#endif
}

static void FMPI_Error_class(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Error_class()\n", debug_rank);
#endif
	call_start(__MPI_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_error_class_(errorcode, errorclass, ierr);
    call_end(__MPI_ERROR_CLASS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Error_class()\n", debug_rank);
#endif
}

static void FMPI_Error_string(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Error_string()\n", debug_rank);
#endif
	call_start(__MPI_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
	pmpi_error_string_(errorcode, string, resultlen, ierr, string_len);
    call_end(__MPI_ERROR_STRING, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Error_string()\n", debug_rank);
#endif
}

static void FMPI_Fetch_and_op(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Fetch_and_op()\n", debug_rank);
#endif
	call_start(__MPI_FETCH_AND_OP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_fetch_and_op_(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, ierr);
    call_end(__MPI_FETCH_AND_OP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Fetch_and_op()\n", debug_rank);
#endif
}

static void FMPI_Iexscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iexscan()\n", debug_rank);
#endif
	call_start(__MPI_IEXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_iexscan_(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
    call_end(__MPI_IEXSCAN, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iexscan()\n", debug_rank);
#endif
}

static void FMPI_File_call_errhandler(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_call_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_FILE_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_call_errhandler_(fh, errorcode, ierr);
    call_end(__MPI_FILE_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_call_errhandler()\n", debug_rank);
#endif
}

static void FMPI_File_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_create_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_FILE_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_create_errhandler_(function, errhandler, ierr);
    call_end(__MPI_FILE_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_create_errhandler()\n", debug_rank);
#endif
}

static void FMPI_File_set_errhandler( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_set_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_set_errhandler_(file, errhandler, ierr);
    call_end(__MPI_FILE_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_set_errhandler()\n", debug_rank);
#endif
}

static void FMPI_File_get_errhandler( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_errhandler_(file, errhandler, ierr);
    call_end(__MPI_FILE_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_errhandler()\n", debug_rank);
#endif
}

static void FMPI_File_open(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_open()\n", debug_rank);
#endif
	call_start(__MPI_FILE_OPEN, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_open_(comm, filename, amode, info,fh, ierr, name_len);
    call_end(__MPI_FILE_OPEN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_open()\n", debug_rank);
#endif
}

static void FMPI_File_close(MPI_Fint *fh, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_close()\n", debug_rank);
#endif
	call_start(__MPI_FILE_CLOSE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_close_(fh, ierr);
    call_end(__MPI_FILE_CLOSE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_close()\n", debug_rank);
#endif
}

static void FMPI_File_delete(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_delete()\n", debug_rank);
#endif
	call_start(__MPI_FILE_DELETE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_delete_(filename, info, ierr, filename_len);
    call_end(__MPI_FILE_DELETE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_delete()\n", debug_rank);
#endif
}

static void FMPI_File_set_size(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_set_size()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_set_size_(fh, size, ierr);
    call_end(__MPI_FILE_SET_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_set_size()\n", debug_rank);
#endif
}

static void FMPI_File_preallocate(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_preallocate()\n", debug_rank);
#endif
	call_start(__MPI_FILE_PREALLOCATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_preallocate_(fh, size, ierr);
    call_end(__MPI_FILE_PREALLOCATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_preallocate()\n", debug_rank);
#endif
}

static void FMPI_File_get_size(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_size()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_size_(fh, size, ierr);
    call_end(__MPI_FILE_GET_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_size()\n", debug_rank);
#endif
}

static void FMPI_File_get_group(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_group()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_group_(fh, group, ierr);
    call_end(__MPI_FILE_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_group()\n", debug_rank);
#endif
}

static void FMPI_File_get_amode(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_amode()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_AMODE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_amode_(fh, amode, ierr);
    call_end(__MPI_FILE_GET_AMODE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_amode()\n", debug_rank);
#endif
}

static void FMPI_File_set_info(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_set_info()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_set_info_(fh, info, ierr);
    call_end(__MPI_FILE_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_set_info()\n", debug_rank);
#endif
}

static void FMPI_File_get_info(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_info()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_info_(fh, info_used, ierr);
    call_end(__MPI_FILE_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_info()\n", debug_rank);
#endif
}

static void FMPI_File_set_view(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_set_view()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_set_view_(fh, disp, etype, filetype, datarep, info, ierr, datarep_len);
    call_end(__MPI_FILE_SET_VIEW, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_set_view()\n", debug_rank);
#endif
}

static void FMPI_File_get_view(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_view()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_VIEW, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_view_(fh, disp, etype, filetype, datarep, ierr, datarep_len);
    call_end(__MPI_FILE_GET_VIEW, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_view()\n", debug_rank);
#endif
}

static void FMPI_File_read_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_at()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_AT, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_AT, *count, datatype_f2c, 0, 0);
	pmpi_file_read_at_(fh, offset, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ_AT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_at()\n", debug_rank);
#endif
}

static void FMPI_File_read_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_at_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_AT_ALL, *count, datatype_f2c, 0, 0);
	pmpi_file_read_at_all_(fh, offset, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_at_all()\n", debug_rank);
#endif
}

static void FMPI_File_write_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_at()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_AT, 0, 0, *count, datatype_f2c);
	pmpi_file_write_at_(fh, offset, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE_AT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_at()\n", debug_rank);
#endif
}

static void FMPI_File_write_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_at_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_AT_ALL, 0, 0, *count, datatype_f2c);
	pmpi_file_write_at_all_(fh, offset, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_at_all()\n", debug_rank);
#endif
}

static void FMPI_File_iread_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iread_at()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IREAD_AT, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IREAD_AT, *count, datatype_f2c, 0, 0);
	pmpi_file_iread_at_(fh, offset, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IREAD_AT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iread_at()\n", debug_rank);
#endif
}

static void FMPI_File_iwrite_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iwrite_at()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IWRITE_AT, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IWRITE_AT, 0, 0, *count, datatype_f2c);
	pmpi_file_iwrite_at_(fh, offset, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IWRITE_AT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iwrite_at()\n", debug_rank);
#endif
}

static void FMPI_File_iread_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iread_at_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IREAD_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IREAD_AT_ALL, *count, datatype_f2c, 0, 0);
	pmpi_file_iread_at_all_(fh, offset, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IREAD_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iread_at_all()\n", debug_rank);
#endif
}

static void FMPI_File_iwrite_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iwrite_at_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IWRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IWRITE_AT_ALL, 0, 0, *count, datatype_f2c);
	pmpi_file_iwrite_at_all_(fh, offset, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IWRITE_AT_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iwrite_at_all()\n", debug_rank);
#endif
}

static void FMPI_File_read(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ, *count, datatype_f2c, 0, 0);
	pmpi_file_read_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read()\n", debug_rank);
#endif
}

static void FMPI_File_read_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_ALL, *count, datatype_f2c, 0, 0);
	pmpi_file_read_all_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_all()\n", debug_rank);
#endif
}

static void FMPI_File_write(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE, 0, 0, *count, datatype_f2c);
	pmpi_file_write_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write()\n", debug_rank);
#endif
}

static void FMPI_File_write_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_ALL, 0, 0, *count, datatype_f2c);
	pmpi_file_write_all_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_all()\n", debug_rank);
#endif
}

static void FMPI_File_iread(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iread()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IREAD, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IREAD, *count, datatype_f2c, 0, 0);
	pmpi_file_iread_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IREAD, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iread()\n", debug_rank);
#endif
}

static void FMPI_File_iwrite(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iwrite()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IWRITE, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IWRITE, 0, 0, *count, datatype_f2c);
	pmpi_file_iwrite_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IWRITE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iwrite()\n", debug_rank);
#endif
}

static void FMPI_File_iread_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iread_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IREAD_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IREAD_ALL, *count, datatype_f2c, 0, 0);
	pmpi_file_iread_all_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IREAD_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iread_all()\n", debug_rank);
#endif
}

static void FMPI_File_iwrite_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iwrite_all()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IWRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IWRITE_ALL, 0, 0, *count, datatype_f2c);
	pmpi_file_iwrite_all_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IWRITE_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iwrite_all()\n", debug_rank);
#endif
}

static void FMPI_File_seek(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_seek()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SEEK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_seek_(fh, offset, whence, ierr);
    call_end(__MPI_FILE_SEEK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_seek()\n", debug_rank);
#endif
}

static void FMPI_File_get_position(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_position()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_POSITION, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_position_(fh, offset, ierr);
    call_end(__MPI_FILE_GET_POSITION, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_position()\n", debug_rank);
#endif
}

static void FMPI_File_get_byte_offset(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_byte_offset()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_BYTE_OFFSET, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_byte_offset_(fh, offset, disp, ierr);
    call_end(__MPI_FILE_GET_BYTE_OFFSET, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_byte_offset()\n", debug_rank);
#endif
}

static void FMPI_File_read_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_SHARED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_SHARED, *count, datatype_f2c, 0, 0);
	pmpi_file_read_shared_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_shared()\n", debug_rank);
#endif
}

static void FMPI_File_write_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_SHARED, 0, 0, *count, datatype_f2c);
	pmpi_file_write_shared_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_shared()\n", debug_rank);
#endif
}

static void FMPI_File_iread_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iread_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IREAD_SHARED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IREAD_SHARED, *count, datatype_f2c, 0, 0);
	pmpi_file_iread_shared_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IREAD_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iread_shared()\n", debug_rank);
#endif
}

static void FMPI_File_iwrite_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_iwrite_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_IWRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_IWRITE_SHARED, 0, 0, *count, datatype_f2c);
	pmpi_file_iwrite_shared_(fh, buf, count, datatype, request, ierr);
    call_end(__MPI_FILE_IWRITE_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_iwrite_shared()\n", debug_rank);
#endif
}

static void FMPI_File_read_ordered(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_ordered()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_ORDERED, *count, datatype_f2c, 0, 0);
	pmpi_file_read_ordered_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_READ_ORDERED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_ordered()\n", debug_rank);
#endif
}

static void FMPI_File_write_ordered(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_ordered()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ORDERED, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_ORDERED, 0, 0, *count, datatype_f2c);
	pmpi_file_write_ordered_(fh, buf, count, datatype, status, ierr);
    call_end(__MPI_FILE_WRITE_ORDERED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_ordered()\n", debug_rank);
#endif
}

static void FMPI_File_seek_shared(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_seek_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SEEK_SHARED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_seek_shared_(fh, offset, whence, ierr);
    call_end(__MPI_FILE_SEEK_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_seek_shared()\n", debug_rank);
#endif
}

static void FMPI_File_get_position_shared(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_position_shared()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_POSITION_SHARED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_position_shared_(fh, offset, ierr);
    call_end(__MPI_FILE_GET_POSITION_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_position_shared()\n", debug_rank);
#endif
}

static void FMPI_File_read_at_all_begin(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_at_all_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_AT_ALL_BEGIN, *count, datatype_f2c, 0, 0);
	pmpi_file_read_at_all_begin_(fh, offset, buf, count, datatype, ierr);
    call_end(__MPI_FILE_READ_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_at_all_begin()\n", debug_rank);
#endif
}

static void FMPI_File_read_at_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_at_all_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_read_at_all_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_READ_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_at_all_end()\n", debug_rank);
#endif
}

static void FMPI_File_write_at_all_begin(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_at_all_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_AT_ALL_BEGIN, 0, 0, *count, datatype_f2c);
	pmpi_file_write_at_all_begin_(fh, offset, buf, count, datatype, ierr);
    call_end(__MPI_FILE_WRITE_AT_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_at_all_begin()\n", debug_rank);
#endif
}

static void FMPI_File_write_at_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_at_all_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_write_at_all_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_WRITE_AT_ALL_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_at_all_end()\n", debug_rank);
#endif
}

static void FMPI_File_read_all_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_all_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_ALL_BEGIN, *count, datatype_f2c, 0, 0);
	pmpi_file_read_all_begin_(fh, buf, count, datatype, ierr);
    call_end(__MPI_FILE_READ_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_all_begin()\n", debug_rank);
#endif
}

static void FMPI_File_read_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_all_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_read_all_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_READ_ALL_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_all_end()\n", debug_rank);
#endif
}

static void FMPI_File_write_all_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_all_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_ALL_BEGIN, 0, 0, *count, datatype_f2c);
	pmpi_file_write_all_begin_(fh, buf, count, datatype, ierr);
    call_end(__MPI_FILE_WRITE_ALL_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_all_begin()\n", debug_rank);
#endif
}

static void FMPI_File_write_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_all_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ALL_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_write_all_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_WRITE_ALL_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_all_end()\n", debug_rank);
#endif
}

static void FMPI_File_read_ordered_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_ordered_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_READ_ORDERED_BEGIN, *count, datatype_f2c, 0, 0);
	pmpi_file_read_ordered_begin_(fh, buf, count, datatype, ierr);
    call_end(__MPI_FILE_READ_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_ordered_begin()\n", debug_rank);
#endif
}

static void FMPI_File_read_ordered_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_read_ordered_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_READ_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_read_ordered_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_READ_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_read_ordered_end()\n", debug_rank);
#endif
}

static void FMPI_File_write_ordered_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_ordered_begin()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	add_file(__MPI_FILE_WRITE_ORDERED_BEGIN, 0, 0, *count, datatype_f2c);
	pmpi_file_write_ordered_begin_(fh, buf, count, datatype, ierr);
    call_end(__MPI_FILE_WRITE_ORDERED_BEGIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_ordered_begin()\n", debug_rank);
#endif
}

static void FMPI_File_write_ordered_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_write_ordered_end()\n", debug_rank);
#endif
	call_start(__MPI_FILE_WRITE_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_write_ordered_end_(fh, buf, status, ierr);
    call_end(__MPI_FILE_WRITE_ORDERED_END, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_write_ordered_end()\n", debug_rank);
#endif
}

static void FMPI_File_get_type_extent(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_type_extent()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_TYPE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_type_extent_(fh, datatype, extent, ierr);
    call_end(__MPI_FILE_GET_TYPE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_type_extent()\n", debug_rank);
#endif
}

static void FMPI_File_set_atomicity(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_set_atomicity()\n", debug_rank);
#endif
	call_start(__MPI_FILE_SET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_set_atomicity_(fh, flag, ierr);
    call_end(__MPI_FILE_SET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_set_atomicity()\n", debug_rank);
#endif
}

static void FMPI_File_get_atomicity(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_File_get_atomicity()\n", debug_rank);
#endif
	call_start(__MPI_FILE_GET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_file_get_atomicity_(fh, flag, ierr);
    call_end(__MPI_FILE_GET_ATOMICITY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_File_get_atomicity()\n", debug_rank);
#endif
}

static void FMPI_Finalized(MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Finalized()\n", debug_rank);
	printf("[DEBUG][RANK:%d] End MPI_Finalized()\n", debug_rank);
#endif
	return pmpi_finalized_(flag, ierr);
}

static void FMPI_Free_mem(MPI_Fint *base, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Free_mem()\n", debug_rank);
#endif
	call_start(__MPI_FREE_MEM, MPI_COMM_WORLD, MPI_NONE);
	pmpi_free_mem_(base, ierr);
    call_end(__MPI_FREE_MEM, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Free_mem()\n", debug_rank);
#endif
}

static void FMPI_Igather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Igather()\n", debug_rank);
#endif
	call_start(__MPI_IGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_IGATHER, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, MPI_ALL);
	}
	else
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_IGATHER, sendcount, &sendtype_f2c, *root, NULL, NULL, MPI_NONE);
	}
	pmpi_igather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
    call_end(__MPI_IGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Igather()\n", debug_rank);
#endif
}

static void FMPI_Igatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Igatherv()\n", debug_rank);
#endif
	call_start(__MPI_IGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_IGATHERV, NULL, NULL, MPI_NONE, recvcounts, &recvtype_f2c, MPI_ALLV);
	}
	else
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_IGATHERV, sendcount, &sendtype_f2c, *root, NULL, NULL, MPI_NONE);
	}
	pmpi_igatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request, ierr);
    call_end(__MPI_IGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Igatherv()\n", debug_rank);
#endif
}

static void FMPI_Get_address(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_address()\n", debug_rank);
#endif
	call_start(__MPI_GET_ADDRESS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_address_(location, address, ierr);
    call_end(__MPI_GET_ADDRESS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_address()\n", debug_rank);
#endif
}

static void FMPI_Get_count(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_count()\n", debug_rank);
#endif
	call_start(__MPI_GET_COUNT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_count_(status, datatype, count, ierr);
    call_end(__MPI_GET_COUNT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_count()\n", debug_rank);
#endif
}

static void FMPI_Get_elements(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_elements()\n", debug_rank);
#endif
	call_start(__MPI_GET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_elements_(status, datatype, count, ierr);
    call_end(__MPI_GET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_elements()\n", debug_rank);
#endif
}

static void FMPI_Get_elements_x(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_elements_x()\n", debug_rank);
#endif
	call_start(__MPI_GET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_elements_x_(status, datatype, count, ierr);
    call_end(__MPI_GET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_elements_x()\n", debug_rank);
#endif
}

static void FMPI_Get(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get()\n", debug_rank);
#endif
	call_start(__MPI_GET, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
    call_end(__MPI_GET, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get()\n", debug_rank);
#endif
}

static void FMPI_Get_accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_accumulate()\n", debug_rank);
#endif
	call_start(__MPI_GET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_accumulate_(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
    call_end(__MPI_GET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_accumulate()\n", debug_rank);
#endif
}

static void FMPI_Get_library_version(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_library_version()\n", debug_rank);
#endif
	call_start(__MPI_GET_LIBRARY_VERSION, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_library_version_(version, resultlen, ierr, version_len);
    call_end(__MPI_GET_LIBRARY_VERSION, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_library_version()\n", debug_rank);
#endif
}

static void FMPI_Get_processor_name(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_processor_name()\n", debug_rank);
#endif
	call_start(__MPI_GET_PROCESSOR_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_processor_name_(name, resultlen, ierr, name_len);
    call_end(__MPI_GET_PROCESSOR_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_processor_name()\n", debug_rank);
#endif
}

static void FMPI_Get_version(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Get_version()\n", debug_rank);
#endif
	call_start(__MPI_GET_VERSION, MPI_COMM_WORLD, MPI_NONE);
	pmpi_get_version_(version, subversion, ierr);
    call_end(__MPI_GET_VERSION, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Get_version()\n", debug_rank);
#endif
}

static void FMPI_Graph_create(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graph_create()\n", debug_rank);
#endif
	call_start(__MPI_GRAPH_CREATE, MPI_Comm_f2c(*comm_old), MPI_NONE);
	pmpi_graph_create_(comm_old, nnodes, index, edges, reorder, comm_graph, ierr);
    call_end(__MPI_GRAPH_CREATE, MPI_Comm_f2c(*comm_old), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graph_create()\n", debug_rank);
#endif
}

static void FMPI_Graph_get(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graph_get()\n", debug_rank);
#endif
	call_start(__MPI_GRAPH_GET, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_graph_get_(comm, maxindex, maxedges, index, edges, ierr);
    call_end(__MPI_GRAPH_GET, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graph_get()\n", debug_rank);
#endif
}

static void FMPI_Graph_map(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graph_map()\n", debug_rank);
#endif
	call_start(__MPI_GRAPH_MAP, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_graph_map_(comm, nnodes, index, edges, newrank, ierr);
    call_end(__MPI_GRAPH_MAP, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graph_map()\n", debug_rank);
#endif
}

static void FMPI_Graph_neighbors_count(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graph_neighbors_count()\n", debug_rank);
#endif
	call_start(__MPI_GRAPH_NEIGHBORS_COUNT, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_graph_neighbors_count_(comm, rank, nneighbors, ierr);
    call_end(__MPI_GRAPH_NEIGHBORS_COUNT, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graph_neighbors_count()\n", debug_rank);
#endif
}

static void FMPI_Graph_neighbors(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graph_neighbors()\n", debug_rank);
#endif
	call_start(__MPI_GRAPH_NEIGHBORS, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_graph_neighbors_(comm, rank, maxneighbors, neighbors, ierr);
    call_end(__MPI_GRAPH_NEIGHBORS, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graph_neighbors()\n", debug_rank);
#endif
}

static void FMPI_Graphdims_get(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Graphdims_get()\n", debug_rank);
#endif
	call_start(__MPI_GRAPHDIMS_GET, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_graphdims_get_(comm, nnodes, nedges, ierr);
    call_end(__MPI_GRAPHDIMS_GET, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Graphdims_get()\n", debug_rank);
#endif
}

static void FMPI_Grequest_complete(MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Grequest_complete()\n", debug_rank);
#endif
	call_start(__MPI_GREQUEST_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_grequest_complete_(request, ierr);
    call_end(__MPI_GREQUEST_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Grequest_complete()\n", debug_rank);
#endif
}

static void FMPI_Grequest_start(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Grequest_start()\n", debug_rank);
#endif
	call_start(__MPI_GREQUEST_START, MPI_COMM_WORLD, MPI_NONE);
	pmpi_grequest_start_(query_fn, free_fn, cancel_fn, extra_state, request, ierr);
    call_end(__MPI_GREQUEST_START, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Grequest_start()\n", debug_rank);
#endif
}

static void FMPI_Group_compare(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_compare()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_COMPARE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_compare_(group1, group2, result, ierr);
    call_end(__MPI_GROUP_COMPARE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_compare()\n", debug_rank);
#endif
}

static void FMPI_Group_difference(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_difference()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_DIFFERENCE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_difference_(group1, group2, newgroup, ierr);
    call_end(__MPI_GROUP_DIFFERENCE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_difference()\n", debug_rank);
#endif
}

static void FMPI_Group_excl(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_excl()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_EXCL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_excl_(group, n, ranks, newgroup, ierr);
    call_end(__MPI_GROUP_EXCL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_excl()\n", debug_rank);
#endif
}

static void FMPI_Group_free(MPI_Fint *group, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_free()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_free_(group, ierr);
    call_end(__MPI_GROUP_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_free()\n", debug_rank);
#endif
}

static void FMPI_Group_incl(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_incl()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_INCL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_incl_(group, n, ranks, newgroup, ierr);
    call_end(__MPI_GROUP_INCL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_incl()\n", debug_rank);
#endif
}

static void FMPI_Group_intersection(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_intersection()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_INTERSECTION, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_intersection_(group1, group2, newgroup, ierr);
    call_end(__MPI_GROUP_INTERSECTION, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_intersection()\n", debug_rank);
#endif
}

static void FMPI_Group_range_excl(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_range_excl()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_RANGE_EXCL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_range_excl_(group, n, ranges, newgroup, ierr);
    call_end(__MPI_GROUP_RANGE_EXCL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_range_excl()\n", debug_rank);
#endif
}

static void FMPI_Group_range_incl(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_range_incl()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_RANGE_INCL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_range_incl_(group, n, ranges, newgroup, ierr);
    call_end(__MPI_GROUP_RANGE_INCL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_range_incl()\n", debug_rank);
#endif
}

static void FMPI_Group_rank(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_rank()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_RANK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_rank_(group, rank, ierr);
    call_end(__MPI_GROUP_RANK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_rank()\n", debug_rank);
#endif
}

static void FMPI_Group_size(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_size()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_size_(group, size, ierr);
    call_end(__MPI_GROUP_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_size()\n", debug_rank);
#endif
}

static void FMPI_Group_translate_ranks(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_translate_ranks()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_TRANSLATE_RANKS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_translate_ranks_(group1, n, ranks1, group2, ranks2, ierr);
    call_end(__MPI_GROUP_TRANSLATE_RANKS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_translate_ranks()\n", debug_rank);
#endif
}

static void FMPI_Group_union(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Group_union()\n", debug_rank);
#endif
	call_start(__MPI_GROUP_UNION, MPI_COMM_WORLD, MPI_NONE);
	pmpi_group_union_(group1, group2, newgroup, ierr);
    call_end(__MPI_GROUP_UNION, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Group_union()\n", debug_rank);
#endif
}

static void FMPI_Improbe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Improbe()\n", debug_rank);
#endif
	call_start(__MPI_IMPROBE, MPI_Comm_f2c(*comm), *source);
	pmpi_improbe_(source, tag, comm, flag, message, status, ierr);
    call_end(__MPI_IMPROBE, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Improbe()\n", debug_rank);
#endif
}

static void FMPI_Imrecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Imrecv()\n", debug_rank);
#endif
	call_start(__MPI_IMRECV, MPI_COMM_WORLD, MPI_NONE);
	pmpi_imrecv_(buf, count, type, message, request, ierr);
    call_end(__MPI_IMRECV, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Imrecv()\n", debug_rank);
#endif
}

static void FMPI_Info_create(MPI_Fint *info, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_create()\n", debug_rank);
#endif
	call_start(__MPI_INFO_CREATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_create_(info, ierr);
    call_end(__MPI_INFO_CREATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_create()\n", debug_rank);
#endif
}

static void FMPI_Info_delete(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_delete()\n", debug_rank);
#endif
	call_start(__MPI_INFO_DELETE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_delete_(info, key, ierr, key_len);
    call_end(__MPI_INFO_DELETE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_delete()\n", debug_rank);
#endif
}

static void FMPI_Info_dup(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_dup()\n", debug_rank);
#endif
	call_start(__MPI_INFO_DUP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_dup_(info, newinfo, ierr);
    call_end(__MPI_INFO_DUP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_dup()\n", debug_rank);
#endif
}

static void FMPI_Info_free(MPI_Fint *info, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_free()\n", debug_rank);
#endif
	call_start(__MPI_INFO_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_free_(info, ierr);
    call_end(__MPI_INFO_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_free()\n", debug_rank);
#endif
}

static void FMPI_Info_get(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_get()\n", debug_rank);
#endif
	call_start(__MPI_INFO_GET, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_get_(info, key, valuelen, value, flag, ierr, key_len, value_len);
    call_end(__MPI_INFO_GET, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_get()\n", debug_rank);
#endif
}

static void FMPI_Info_get_nkeys(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_get_nkeys()\n", debug_rank);
#endif
	call_start(__MPI_INFO_GET_NKEYS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_get_nkeys_(info, nkeys, ierr);
    call_end(__MPI_INFO_GET_NKEYS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_get_nkeys()\n", debug_rank);
#endif
}

static void FMPI_Info_get_nthkey(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_get_nthkey()\n", debug_rank);
#endif
	call_start(__MPI_INFO_GET_NTHKEY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_get_nthkey_(info, n, key, ierr, key_len);
    call_end(__MPI_INFO_GET_NTHKEY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_get_nthkey()\n", debug_rank);
#endif
}

static void FMPI_Info_get_valuelen(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_get_valuelen()\n", debug_rank);
#endif
	call_start(__MPI_INFO_GET_VALUELEN, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_get_valuelen_(info, key, valuelen, flag, ierr, key_len);
    call_end(__MPI_INFO_GET_VALUELEN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_get_valuelen()\n", debug_rank);
#endif
}

static void FMPI_Info_set(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Info_set()\n", debug_rank);
#endif
	call_start(__MPI_INFO_SET, MPI_COMM_WORLD, MPI_NONE);
	pmpi_info_set_(info, key, value, ierr, key_len, value_len);
    call_end(__MPI_INFO_SET, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Info_set()\n", debug_rank);
#endif
}

static void FMPI_Initialized(MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Initialized()\n", debug_rank);
	printf("[DEBUG][RANK:%d] End MPI_Initialized()\n", debug_rank);
#endif
	pmpi_initialized_(flag, ierr);
#ifdef DEBUG_MPI
#endif
}

static void FMPI_Intercomm_create(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Intercomm_create()\n", debug_rank);
#endif
	call_start(__MPI_INTERCOMM_CREATE, MPI_Comm_f2c(*local_comm), MPI_NONE);
	pmpi_intercomm_create_(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr);
    call_end(__MPI_INTERCOMM_CREATE, MPI_Comm_f2c(*local_comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Intercomm_create()\n", debug_rank);
#endif
}

static void FMPI_Intercomm_merge(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Intercomm_merge()\n", debug_rank);
#endif
	call_start(__MPI_INTERCOMM_MERGE, MPI_Comm_f2c(*intercomm), MPI_NONE);
	pmpi_intercomm_merge_(intercomm, high, newintercomm, ierr);
    call_end(__MPI_INTERCOMM_MERGE, MPI_Comm_f2c(*intercomm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Intercomm_merge()\n", debug_rank);
#endif
}

static void FMPI_Is_thread_main(MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Is_thread_main()\n", debug_rank);
#endif
	call_start(__MPI_IS_THREAD_MAIN, MPI_COMM_WORLD, MPI_NONE);
	pmpi_is_thread_main_(flag, ierr);
    call_end(__MPI_IS_THREAD_MAIN, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Is_thread_main()\n", debug_rank);
#endif
}

static void FMPI_Lookup_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Lookup_name()\n", debug_rank);
#endif
	call_start(__MPI_LOOKUP_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_lookup_name_(service_name, info, port_name, ierr, service_name_len, port_name_len);
    call_end(__MPI_LOOKUP_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Lookup_name()\n", debug_rank);
#endif
}

static void FMPI_Mprobe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Mprobe()\n", debug_rank);
#endif
	call_start(__MPI_MPROBE, MPI_Comm_f2c(*comm), *source);
	pmpi_mprobe_(source, tag, comm, message, status, ierr);
    call_end(__MPI_MPROBE, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Mprobe()\n", debug_rank);
#endif
}

static void FMPI_Mrecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Mrecv()\n", debug_rank);
#endif
	call_start(__MPI_MRECV, MPI_COMM_WORLD, MPI_NONE);
	pmpi_mrecv_(buf, count, type, message, status, ierr);
    call_end(__MPI_MRECV, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Mrecv()\n", debug_rank);
#endif
}

static void FMPI_Ineighbor_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ineighbor_allgather()\n", debug_rank);
#endif
	call_start(__MPI_INEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_ineighbor_allgather_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
    call_end(__MPI_INEIGHBOR_ALLGATHER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ineighbor_allgather()\n", debug_rank);
#endif
}

static void FMPI_Ineighbor_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ineighbor_allgatherv()\n", debug_rank);
#endif
	call_start(__MPI_INEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_ineighbor_allgatherv_(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
    call_end(__MPI_INEIGHBOR_ALLGATHERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ineighbor_allgatherv()\n", debug_rank);
#endif
}

static void FMPI_Ineighbor_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ineighbor_alltoall()\n", debug_rank);
#endif
	call_start(__MPI_INEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
	pmpi_ineighbor_alltoall_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
    call_end(__MPI_INEIGHBOR_ALLTOALL, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ineighbor_alltoall()\n", debug_rank);
#endif
}

static void FMPI_Ineighbor_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ineighbor_alltoallv()\n", debug_rank);
#endif
	call_start(__MPI_INEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
	pmpi_ineighbor_alltoallv_(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
    call_end(__MPI_INEIGHBOR_ALLTOALLV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ineighbor_alltoallv()\n", debug_rank);
#endif
}

static void FMPI_Ineighbor_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ineighbor_alltoallw()\n", debug_rank);
#endif
	call_start(__MPI_INEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
	pmpi_ineighbor_alltoallw_(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
    call_end(__MPI_INEIGHBOR_ALLTOALLW, MPI_Comm_f2c(*comm), MPI_ALLW);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ineighbor_alltoallw()\n", debug_rank);
#endif
}

static void FMPI_Op_commutative(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Op_commutative()\n", debug_rank);
#endif
	call_start(__MPI_OP_COMMUTATIVE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_op_commutative_(op, commute, ierr);
    call_end(__MPI_OP_COMMUTATIVE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Op_commutative()\n", debug_rank);
#endif
}

static void FMPI_Op_create(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Op_create()\n", debug_rank);
#endif
	call_start(__MPI_OP_CREATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_op_create_(function, commute, op, ierr);
    call_end(__MPI_OP_CREATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Op_create()\n", debug_rank);
#endif
}

static void FMPI_Open_port(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Open_port()\n", debug_rank);
#endif
	call_start(__MPI_OPEN_PORT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_open_port_(info, port_name, ierr, port_name_len);
    call_end(__MPI_OPEN_PORT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Open_port()\n", debug_rank);
#endif
}

static void FMPI_Op_free(MPI_Fint *op, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Op_free()\n", debug_rank);
#endif
	call_start(__MPI_OP_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_op_free_(op, ierr);
    call_end(__MPI_OP_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Op_free()\n", debug_rank);
#endif
}

static void FMPI_Pack_external(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Pack_external()\n", debug_rank);
#endif
	call_start(__MPI_PACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_pack_external_(datarep, inbuf, incount, datatype, outbuf, outsize, position, ierr, datarep_len);
    call_end(__MPI_PACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Pack_external()\n", debug_rank);
#endif
}

static void FMPI_Pack_external_size(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Pack_external_size()\n", debug_rank);
#endif
	call_start(__MPI_PACK_EXTERNAL_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_pack_external_size_(datarep, incount, datatype, size, ierr, datarep_len);
    call_end(__MPI_PACK_EXTERNAL_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Pack_external_size()\n", debug_rank);
#endif
}

static void FMPI_Pack(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Pack()\n", debug_rank);
#endif
	call_start(__MPI_PACK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_pack_(inbuf, incount, datatype, outbuf, outsize, position, comm, ierr);
    call_end(__MPI_PACK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Pack()\n", debug_rank);
#endif
}

static void FMPI_Pack_size(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Pack_size()\n", debug_rank);
#endif
	call_start(__MPI_PACK_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_pack_size_(incount, datatype, comm, size, ierr);
    call_end(__MPI_PACK_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Pack_size()\n", debug_rank);
#endif
}

static void FMPI_Pcontrol(MPI_Fint *level, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Pcontrol()\n", debug_rank);
#endif
	call_start(__MPI_PCONTROL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_pcontrol_(level, ierr);
    call_end(__MPI_PCONTROL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Pcontrol()\n", debug_rank);
#endif
}

static void FMPI_Publish_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Publish_name()\n", debug_rank);
#endif
	call_start(__MPI_PUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_publish_name_(service_name, info, port_name, ierr, service_name_len, port_name_len);
    call_end(__MPI_PUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Publish_name()\n", debug_rank);
#endif
}

static void FMPI_Put(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Put()\n", debug_rank);
#endif
	call_start(__MPI_PUT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_put_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
    call_end(__MPI_PUT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Put()\n", debug_rank);
#endif
}

static void FMPI_Query_thread(MPI_Fint *provided, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Query_thread()\n", debug_rank);
#endif
	call_start(__MPI_QUERY_THREAD, MPI_COMM_WORLD, MPI_NONE);
	pmpi_query_thread_(provided, ierr);
    call_end(__MPI_QUERY_THREAD, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Query_thread()\n", debug_rank);
#endif
}

static void FMPI_Raccumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Raccumulate()\n", debug_rank);
#endif
	call_start(__MPI_RACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_raccumulate_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
    call_end(__MPI_RACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Raccumulate()\n", debug_rank);
#endif
}

static void FMPI_Recv_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Recv_init()\n", debug_rank);
#endif
    call_start(__MPI_RECV_INIT, MPI_Comm_f2c(*comm), *source);
	pmpi_recv_init_(buf, count, datatype, source, tag, comm, request, ierr);
    call_end(__MPI_RECV_INIT, MPI_Comm_f2c(*comm), *source);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Recv_init()\n", debug_rank);
#endif
}

static void FMPI_Ireduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ireduce()\n", debug_rank);
#endif
	call_start(__MPI_IREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == *root)
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE, NULL, NULL, MPI_NONE, count, &datatype_f2c, MPI_ALL);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE, count, &datatype_f2c, *root, NULL, NULL, MPI_NONE);
	pmpi_ireduce_(sendbuf, recvbuf, count, datatype, op, root, comm, request, ierr);
    call_end(__MPI_IREDUCE, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ireduce()\n", debug_rank);
#endif
}

static void FMPI_Ireduce_scatter(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ireduce_scatter()\n", debug_rank);
#endif
	call_start(__MPI_IREDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == 0)
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE_SCATTER, recvcounts, &datatype_f2c, MPI_ALLV, recvcounts, &datatype_f2c, MPI_ALLV);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE_SCATTER, &recvcounts[my_rank], &datatype_f2c, 0, &recvcounts[my_rank], &datatype_f2c, 0);
	pmpi_ireduce_scatter_(sendbuf, recvbuf, recvcounts, datatype, op, comm, request, ierr);
    call_end(__MPI_IREDUCE_SCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ireduce_scatter()\n", debug_rank);
#endif
}

static void FMPI_Reduce_scatter_block(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Reduce_scatter_block()\n", debug_rank);
#endif
	call_start(__MPI_REDUCE_SCATTER_BLOCK, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == 0)
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE_SCATTER_BLOCK, recvcount, &datatype_f2c, MPI_ALL, recvcount, &datatype_f2c, MPI_ALLV);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_REDUCE_SCATTER_BLOCK, recvcount, &datatype_f2c, 0, recvcount, &datatype_f2c, 0);
	pmpi_reduce_scatter_block_(sendbuf, recvbuf, recvcount, datatype, op, comm, ierr);
    call_end(__MPI_REDUCE_SCATTER_BLOCK, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Reduce_scatter_block()\n", debug_rank);
#endif
}

static void FMPI_Ireduce_scatter_block(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ireduce_scatter_block()\n", debug_rank);
#endif
	call_start(__MPI_IREDUCE_SCATTER_BLOCK, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	MPI_Datatype datatype_f2c = MPI_Type_f2c(*datatype);
	if(my_rank == 0)
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE_SCATTER_BLOCK, recvcount, &datatype_f2c, MPI_ALL, recvcount, &datatype_f2c, MPI_ALLV);
	else
		add_network(MPI_Comm_f2c(*comm), __MPI_IREDUCE_SCATTER_BLOCK, recvcount, &datatype_f2c, 0, recvcount, &datatype_f2c, 0);
	pmpi_ireduce_scatter_block_(sendbuf, recvbuf, recvcount, datatype, op, comm, request, ierr);
    call_end(__MPI_IREDUCE_SCATTER_BLOCK, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Ireduce_scatter_block()\n", debug_rank);
#endif
}

static void FMPI_Register_datarep(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Register_datarep()\n", debug_rank);
#endif
	call_start(__MPI_REGISTER_DATAREP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_register_datarep_(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr, datarep_len);
    call_end(__MPI_REGISTER_DATAREP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Register_datarep()\n", debug_rank);
#endif
}

static void FMPI_Request_free(MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Request_free()\n", debug_rank);
#endif
	call_start(__MPI_REQUEST_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_request_free_(request, ierr);
    call_end(__MPI_REQUEST_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Request_free()\n", debug_rank);
#endif
}

static void FMPI_Request_get_status(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Request_get_status()\n", debug_rank);
#endif
	call_start(__MPI_REQUEST_GET_STATUS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_request_get_status_(request, flag, status, ierr);
    call_end(__MPI_REQUEST_GET_STATUS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Request_get_status()\n", debug_rank);
#endif
}

static void FMPI_Rget(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Rget()\n", debug_rank);
#endif
	call_start(__MPI_RGET, MPI_COMM_WORLD, MPI_NONE);
	pmpi_rget_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
    call_end(__MPI_RGET, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Rget()\n", debug_rank);
#endif
}

static void FMPI_Rget_accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Rget_accumulate()\n", debug_rank);
#endif
	call_start(__MPI_RGET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_rget_accumulate_(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
    call_end(__MPI_RGET_ACCUMULATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Rget_accumulate()\n", debug_rank);
#endif
}

static void FMPI_Rput(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Rput()\n", debug_rank);
#endif
	call_start(__MPI_RPUT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_rput_(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
    call_end(__MPI_RPUT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Rput()\n", debug_rank);
#endif
}

static void FMPI_Rsend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Rsend_init()\n", debug_rank);
#endif
    call_start(__MPI_RSEND_INIT, MPI_Comm_f2c(*comm), *dest);
	pmpi_rsend_init_(buf, count, datatype, dest, tag, comm, request, ierr);
    call_end(__MPI_RSEND_INIT, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Rsend_init()\n", debug_rank);
#endif
}

static void FMPI_Iscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iscan()\n", debug_rank);
#endif
	call_start(__MPI_ISCAN, MPI_Comm_f2c(*comm), MPI_NONE);
	pmpi_iscan_(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
    call_end(__MPI_ISCAN, MPI_Comm_f2c(*comm), MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iscan()\n", debug_rank);
#endif
}

static void FMPI_Iscatter(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iscatter()\n", debug_rank);
#endif
	call_start(__MPI_ISCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_ISCATTER, sendcount, &sendtype_f2c, MPI_ALL, NULL, NULL, MPI_NONE);
	}
	else
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_ISCATTER, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, *root);
	}
	pmpi_iscatter_(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
    call_end(__MPI_ISCATTER, MPI_Comm_f2c(*comm), MPI_ALL);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iscatter()\n", debug_rank);
#endif
}

static void FMPI_Iscatterv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Iscatterv()\n", debug_rank);
#endif
	call_start(__MPI_ISCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
	int my_rank;
	PMPI_Comm_rank(MPI_Comm_f2c(*comm), &my_rank);
	if(my_rank == *root)
	{
		MPI_Datatype sendtype_f2c = MPI_Type_f2c(*sendtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_ISCATTERV, sendcounts, &sendtype_f2c, MPI_ALLV, NULL, NULL, MPI_NONE);
	}
	else
	{
		MPI_Datatype recvtype_f2c = MPI_Type_f2c(*recvtype);
		add_network(MPI_Comm_f2c(*comm), __MPI_ISCATTERV, NULL, NULL, MPI_NONE, recvcount, &recvtype_f2c, *root);
	}
	pmpi_iscatterv_(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
    call_end(__MPI_ISCATTERV, MPI_Comm_f2c(*comm), MPI_ALLV);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Iscatterv()\n", debug_rank);
#endif
}

static void FMPI_Send_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Send_init()\n", debug_rank);
#endif
    call_start(__MPI_SEND_INIT, MPI_Comm_f2c(*comm), *dest);
	pmpi_send_init_(buf, count, datatype, dest, tag, comm, request, ierr);
    call_end(__MPI_SEND_INIT, MPI_Comm_f2c(*comm), *dest);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Send_init()\n", debug_rank);
#endif
}

static void FMPI_Ssend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Ssend_init()\n", debug_rank);
#endif
    call_start(__MPI_SSEND_INIT, MPI_Comm_f2c(*comm), *dest);
	pmpi_ssend_init_(buf, count, datatype, dest, tag, comm, request, ierr);
       call_end(__MPI_SSEND_INIT, MPI_Comm_f2c(*comm), *dest);
	#ifdef DEBUG_MPI
		printf("[DEBUG][RANK:%d] End MPI_Ssend_init()\n", debug_rank);
	#endif
}

static void FMPI_Start(MPI_Fint *request, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Start()\n", debug_rank);
#endif
	call_start(__MPI_START, MPI_COMM_WORLD, MPI_NONE);
	pmpi_start_(request, ierr);
    call_end(__MPI_START, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Start()\n", debug_rank);
#endif
}

static void FMPI_Startall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Startall()\n", debug_rank);
#endif
	call_start(__MPI_STARTALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_startall_(count, array_of_requests, ierr);
    call_end(__MPI_STARTALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Startall()\n", debug_rank);
#endif
}

static void FMPI_Status_set_cancelled(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Status_set_cancelled()\n", debug_rank);
#endif
	call_start(__MPI_STATUS_SET_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_status_set_cancelled_(status, flag, ierr);
    call_end(__MPI_STATUS_SET_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Status_set_cancelled()\n", debug_rank);
#endif
}

static void FMPI_Status_set_elements(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Status_set_elements()\n", debug_rank);
#endif
	call_start(__MPI_STATUS_SET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_status_set_elements_(status, datatype, count, ierr);
    call_end(__MPI_STATUS_SET_ELEMENTS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Status_set_elements()\n", debug_rank);
#endif
}

static void FMPI_Status_set_elements_x(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Status_set_elements_x()\n", debug_rank);
#endif
	call_start(__MPI_STATUS_SET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
	pmpi_status_set_elements_x_(status, datatype, count, ierr);
    call_end(__MPI_STATUS_SET_ELEMENTS_X, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Status_set_elements_x()\n", debug_rank);
#endif
}

static void FMPI_Testall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Testall()\n", debug_rank);
#endif
	call_start(__MPI_TESTALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_testall_(count, array_of_requests, flag, array_of_statuses, ierr);
    call_end(__MPI_TESTALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Testall()\n", debug_rank);
#endif
}

static void FMPI_Testany(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Testany()\n", debug_rank);
#endif
	call_start(__MPI_TESTANY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_testany_(count, array_of_requests, index, flag, status, ierr);
    call_end(__MPI_TESTANY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Testany()\n", debug_rank);
#endif
}

static void FMPI_Test(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Test()\n", debug_rank);
#endif
	call_start(__MPI_TEST, MPI_COMM_WORLD, MPI_NONE);
	pmpi_test_(request, flag, status, ierr);
    call_end(__MPI_TEST, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Test()\n", debug_rank);
#endif
}

static void FMPI_Test_cancelled(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Test_cancelled()\n", debug_rank);
#endif
	call_start(__MPI_TEST_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_test_cancelled_(status, flag, ierr);
    call_end(__MPI_TEST_CANCELLED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Test_cancelled()\n", debug_rank);
#endif
}

static void FMPI_Testsome(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Testsome()\n", debug_rank);
#endif
	call_start(__MPI_TESTSOME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_testsome_(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
    call_end(__MPI_TESTSOME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Testsome()\n", debug_rank);
#endif
}

static void FMPI_Topo_test(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Topo_test()\n", debug_rank);
#endif
	call_start(__MPI_TOPO_TEST, MPI_COMM_WORLD, MPI_NONE);
	pmpi_topo_test_(comm, status, ierr);
    call_end(__MPI_TOPO_TEST, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Topo_test()\n", debug_rank);
#endif
}

static void FMPI_Type_commit(MPI_Fint *type, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_commit()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_COMMIT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_commit_(type, ierr);
    call_end(__MPI_TYPE_COMMIT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_commit()\n", debug_rank);
#endif
}

static void FMPI_Type_contiguous(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_contiguous()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CONTIGUOUS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_contiguous_(count, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CONTIGUOUS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_contiguous()\n", debug_rank);
#endif
}

static void FMPI_Type_create_darray(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_darray()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_DARRAY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_darray_(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_DARRAY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_darray()\n", debug_rank);
#endif
}

static void FMPI_Type_create_f90_complex(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_f90_complex()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_F90_COMPLEX, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_f90_complex_(p, r, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_F90_COMPLEX, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_f90_complex()\n", debug_rank);
#endif
}

static void FMPI_Type_create_f90_integer(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_f90_integer()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_F90_INTEGER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_f90_integer_(r, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_F90_INTEGER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_f90_integer()\n", debug_rank);
#endif
}

static void FMPI_Type_create_f90_real(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_f90_real()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_F90_REAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_f90_real_(p, r, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_F90_REAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_f90_real()\n", debug_rank);
#endif
}

static void FMPI_Type_create_hindexed_block(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_hindexed_block()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_HINDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_hindexed_block_(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_HINDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_hindexed_block()\n", debug_rank);
#endif
}

static void FMPI_Type_create_hindexed(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_hindexed()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_HINDEXED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_hindexed_(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_HINDEXED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_hindexed()\n", debug_rank);
#endif
}

static void FMPI_Type_create_hvector(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_hvector()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_HVECTOR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_hvector_(count, blocklength, stride, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_HVECTOR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_hvector()\n", debug_rank);
#endif
}

static void FMPI_Type_create_keyval(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_keyval()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_keyval_(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr);
    call_end(__MPI_TYPE_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_keyval()\n", debug_rank);
#endif
}

static void FMPI_Type_create_indexed_block(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_indexed_block()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_INDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_indexed_block_(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_INDEXED_BLOCK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_indexed_block()\n", debug_rank);
#endif
}

static void FMPI_Type_create_struct(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_struct()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_STRUCT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_struct_(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_STRUCT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_struct()\n", debug_rank);
#endif
}

static void FMPI_Type_create_subarray(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_subarray()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_SUBARRAY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_subarray_(ndims, size_array, subsize_array, start_array, order, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_SUBARRAY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_subarray()\n", debug_rank);
#endif
}

static void FMPI_Type_create_resized(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_create_resized()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_CREATE_RESIZED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_create_resized_(oldtype, lb, extent, newtype, ierr);
    call_end(__MPI_TYPE_CREATE_RESIZED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_create_resized()\n", debug_rank);
#endif
}

static void FMPI_Type_delete_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_delete_attr()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_delete_attr_(type, type_keyval, ierr);
    call_end(__MPI_TYPE_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_delete_attr()\n", debug_rank);
#endif
}

static void FMPI_Type_dup(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_dup()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_DUP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_dup_(type, newtype, ierr);
    call_end(__MPI_TYPE_DUP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_dup()\n", debug_rank);
#endif
}

static void FMPI_Type_free(MPI_Fint *type, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_free()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_free_(type, ierr);
    call_end(__MPI_TYPE_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_free()\n", debug_rank);
#endif
}

static void FMPI_Type_free_keyval(MPI_Fint *type_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_free_keyval()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_free_keyval_(type_keyval, ierr);
    call_end(__MPI_TYPE_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_free_keyval()\n", debug_rank);
#endif
}

static void FMPI_Type_get_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_attr()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_attr_(type, type_keyval, attribute_val, flag, ierr);
    call_end(__MPI_TYPE_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_attr()\n", debug_rank);
#endif
}

static void FMPI_Type_get_contents(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_contents()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_CONTENTS, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_contents_(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes, ierr);
    call_end(__MPI_TYPE_GET_CONTENTS, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_contents()\n", debug_rank);
#endif
}

static void FMPI_Type_get_envelope(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_envelope()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_ENVELOPE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_envelope_(type, num_integers, num_addresses, num_datatypes, combiner, ierr);
    call_end(__MPI_TYPE_GET_ENVELOPE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_envelope()\n", debug_rank);
#endif
}

static void FMPI_Type_get_extent(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_extent()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_extent_(type, lb, extent, ierr);
    call_end(__MPI_TYPE_GET_EXTENT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_extent()\n", debug_rank);
#endif
}

static void FMPI_Type_get_extent_x(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_extent_x()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_extent_x_(type, lb, extent, ierr);
    call_end(__MPI_TYPE_GET_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_extent_x()\n", debug_rank);
#endif
}

static void FMPI_Type_get_name(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_name()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_name_(type, type_name, resultlen, ierr, type_name_len);
    call_end(__MPI_TYPE_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_name()\n", debug_rank);
#endif
}

static void FMPI_Type_get_true_extent(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_true_extent()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_TRUE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_true_extent_(datatype, true_lb, true_extent, ierr);
    call_end(__MPI_TYPE_GET_TRUE_EXTENT, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_true_extent()\n", debug_rank);
#endif
}

static void FMPI_Type_get_true_extent_x(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_get_true_extent_x()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_GET_TRUE_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_get_true_extent_x_(datatype, true_lb, true_extent, ierr);
    call_end(__MPI_TYPE_GET_TRUE_EXTENT_X, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_get_true_extent_x()\n", debug_rank);
#endif
}

static void FMPI_Type_indexed(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_indexed()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_INDEXED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_indexed_(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_INDEXED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_indexed()\n", debug_rank);
#endif
}

static void FMPI_Type_match_size(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_match_size()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_MATCH_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_match_size_(typeclass, size, type, ierr);
    call_end(__MPI_TYPE_MATCH_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_match_size()\n", debug_rank);
#endif
}

static void FMPI_Type_set_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_set_attr()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_set_attr_(type, type_keyval, attr_val, ierr);
    call_end(__MPI_TYPE_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_set_attr()\n", debug_rank);
#endif
}

static void FMPI_Type_set_name(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_set_name()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_set_name_(type, type_name, ierr, type_name_len);
    call_end(__MPI_TYPE_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_set_name()\n", debug_rank);
#endif
}

static void FMPI_Type_size(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_size()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_SIZE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_size_(type, size, ierr);
    call_end(__MPI_TYPE_SIZE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_size()\n", debug_rank);
#endif
}

static void FMPI_Type_size_x(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_size_x()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_SIZE_X, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_size_x_(type, size, ierr);
    call_end(__MPI_TYPE_SIZE_X, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_size_x()\n", debug_rank);
#endif
}

static void FMPI_Type_vector(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Type_vector()\n", debug_rank);
#endif
	call_start(__MPI_TYPE_VECTOR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_type_vector_(count, blocklength, stride, oldtype, newtype, ierr);
    call_end(__MPI_TYPE_VECTOR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Type_vector()\n", debug_rank);
#endif
}

static void FMPI_Unpack(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Unpack()\n", debug_rank);
#endif
	call_start(__MPI_UNPACK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_unpack_(inbuf, insize, position, outbuf, outcount, datatype, comm, ierr);
    call_end(__MPI_UNPACK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Unpack()\n", debug_rank);
#endif
}

static void FMPI_Unpublish_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Unpublish_name()\n", debug_rank);
#endif
	call_start(__MPI_UNPUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_unpublish_name_(service_name, info, port_name, ierr, service_name_len, port_name_len);
    call_end(__MPI_UNPUBLISH_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Unpublish_name()\n", debug_rank);
#endif
}

static void FMPI_Unpack_external(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Unpack_external()\n", debug_rank);
#endif
	call_start(__MPI_UNPACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_unpack_external_(datarep, inbuf, insize, position, outbuf, outcount, datatype, ierr, datarep_len);
    call_end(__MPI_UNPACK_EXTERNAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Unpack_external()\n", debug_rank);
#endif
}

static void FMPI_Win_allocate(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_allocate()\n", debug_rank);
#endif
	call_start(__MPI_WIN_ALLOCATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_allocate_(size, disp_unit, info, comm, baseptr, win, ierr);
    call_end(__MPI_WIN_ALLOCATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_allocate()\n", debug_rank);
#endif
}

static void FMPI_Win_allocate_shared(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_allocate_shared()\n", debug_rank);
#endif
	call_start(__MPI_WIN_ALLOCATE_SHARED, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_allocate_shared_(size, disp_unit, info, comm, baseptr, win, ierr);
    call_end(__MPI_WIN_ALLOCATE_SHARED, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_allocate_shared()\n", debug_rank);
#endif
}

static void FMPI_Win_attach(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_attach()\n", debug_rank);
#endif
	call_start(__MPI_WIN_ATTACH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_attach_(win, base, size, ierr);
    call_end(__MPI_WIN_ATTACH, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_attach()\n", debug_rank);
#endif
}

static void FMPI_Win_call_errhandler(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_call_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_WIN_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_call_errhandler_(win, errorcode, ierr);
    call_end(__MPI_WIN_CALL_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_call_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Win_complete(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_complete()\n", debug_rank);
#endif
	call_start(__MPI_WIN_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_complete_(win, ierr);
    call_end(__MPI_WIN_COMPLETE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_complete()\n", debug_rank);
#endif
}

static void FMPI_Win_create(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_create()\n", debug_rank);
#endif
	call_start(__MPI_WIN_CREATE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_create_(base, size, disp_unit, info, comm, win, ierr);
    call_end(__MPI_WIN_CREATE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_create()\n", debug_rank);
#endif
}

static void FMPI_Win_create_dynamic(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_create_dynamic()\n", debug_rank);
#endif
	call_start(__MPI_WIN_CREATE_DYNAMIC, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_create_dynamic_(info, comm, win, ierr);
    call_end(__MPI_WIN_CREATE_DYNAMIC, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_create_dynamic()\n", debug_rank);
#endif
}

static void FMPI_Win_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_create_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_WIN_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_create_errhandler_(function, errhandler, ierr);
    call_end(__MPI_WIN_CREATE_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_create_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Win_create_keyval(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_create_keyval()\n", debug_rank);
#endif
	call_start(__MPI_WIN_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_create_keyval_(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr);
    call_end(__MPI_WIN_CREATE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_create_keyval()\n", debug_rank);
#endif
}

static void FMPI_Win_delete_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_delete_attr()\n", debug_rank);
#endif
	call_start(__MPI_WIN_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_delete_attr_(win, win_keyval, ierr);
    call_end(__MPI_WIN_DELETE_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_delete_attr()\n", debug_rank);
#endif
}

static void FMPI_Win_detach(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_detach()\n", debug_rank);
#endif
	call_start(__MPI_WIN_DETACH, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_detach_(win, base, ierr);
    call_end(__MPI_WIN_DETACH, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_detach()\n", debug_rank);
#endif
}

static void FMPI_Win_fence(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_fence()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FENCE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_fence_(assert, win, ierr);
    call_end(__MPI_WIN_FENCE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_fence()\n", debug_rank);
#endif
}

static void FMPI_Win_free(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_free()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FREE, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_free_(win, ierr);
    call_end(__MPI_WIN_FREE, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_free()\n", debug_rank);
#endif
}

static void FMPI_Win_free_keyval(MPI_Fint *win_keyval, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_free_keyval()\n", debug_rank);
#endif
	call_start(__MPI_WIN_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_free_keyval_(win_keyval, ierr);
    call_end(__MPI_WIN_FREE_KEYVAL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_free_keyval()\n", debug_rank);
#endif
}

static void FMPI_Win_get_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_get_attr()\n", debug_rank);
#endif
	call_start(__MPI_WIN_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_get_attr_(win, win_keyval, attribute_val, flag, ierr);
    call_end(__MPI_WIN_GET_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_get_attr()\n", debug_rank);
#endif
}

static void FMPI_Win_get_errhandler(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_get_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_WIN_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_get_errhandler_(win, errhandler, ierr);
    call_end(__MPI_WIN_GET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_get_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Win_get_group(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_get_group()\n", debug_rank);
#endif
	call_start(__MPI_WIN_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_get_group_(win, group, ierr);
    call_end(__MPI_WIN_GET_GROUP, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_get_group()\n", debug_rank);
#endif
}

static void FMPI_Win_get_info(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_get_info()\n", debug_rank);
#endif
	call_start(__MPI_WIN_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_get_info_(win, info_used, ierr);
    call_end(__MPI_WIN_GET_INFO, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_get_info()\n", debug_rank);
#endif
}

static void FMPI_Win_get_name(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_get_name()\n", debug_rank);
#endif
	call_start(__MPI_WIN_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_get_name_(win, win_name, resultlen, ierr, win_name_len);
    call_end(__MPI_WIN_GET_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_get_name()\n", debug_rank);
#endif
}

static void FMPI_Win_post(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_post()\n", debug_rank);
#endif
	call_start(__MPI_WIN_POST, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_post_(group, assert, win, ierr);
    call_end(__MPI_WIN_POST, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_post()\n", debug_rank);
#endif
}

static void FMPI_Win_set_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_set_attr()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_set_attr_(win, win_keyval, attribute_val, ierr);
    call_end(__MPI_WIN_SET_ATTR, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_set_attr()\n", debug_rank);
#endif
}

static void FMPI_Win_set_errhandler(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_set_errhandler()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_set_errhandler_(win, errhandler, ierr);
    call_end(__MPI_WIN_SET_ERRHANDLER, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_set_errhandler()\n", debug_rank);
#endif
}

static void FMPI_Win_set_info(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_set_info()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_set_info_(win, info, ierr);
    call_end(__MPI_WIN_SET_INFO, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_set_info()\n", debug_rank);
#endif
}

static void FMPI_Win_set_name(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_set_name()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_set_name_(win, win_name, ierr, win_name_len);
    call_end(__MPI_WIN_SET_NAME, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_set_name()\n", debug_rank);
#endif
}

static void FMPI_Win_shared_query(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_shared_query()\n", debug_rank);
#endif
	call_start(__MPI_WIN_SHARED_QUERY, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_shared_query_(win, rank, size, disp_unit, baseptr, ierr);
    call_end(__MPI_WIN_SHARED_QUERY, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_shared_query()\n", debug_rank);
#endif
}

static void FMPI_Win_start(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_start()\n", debug_rank);
#endif
	call_start(__MPI_WIN_START, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_start_(group, assert, win, ierr);
    call_end(__MPI_WIN_START, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_start()\n", debug_rank);
#endif
}

static void FMPI_Win_test(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_test()\n", debug_rank);
#endif
	call_start(__MPI_WIN_TEST, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_test_(win, flag, ierr);
    call_end(__MPI_WIN_TEST, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_test()\n", debug_rank);
#endif
}

static void FMPI_Win_unlock(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_unlock()\n", debug_rank);
#endif
	call_start(__MPI_WIN_UNLOCK, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_unlock_(rank, win, ierr);
    call_end(__MPI_WIN_UNLOCK, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_unlock()\n", debug_rank);
#endif
}

static void FMPI_Win_unlock_all(MPI_Fint *win, MPI_Fint *ierr)
{
#ifdef DEBUG_MPI
	int debug_rank;
	PMPI_Comm_rank(MPI_COMM_WORLD, &debug_rank);
	printf("[DEBUG][RANK:%d] Start MPI_Win_unlock_all()\n", debug_rank);
#endif
	call_start(__MPI_WIN_UNLOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
	pmpi_win_unlock_all_(win, ierr);
    call_end(__MPI_WIN_UNLOCK_ALL, MPI_COMM_WORLD, MPI_NONE);
#ifdef DEBUG_MPI
	printf("[DEBUG][RANK:%d] End MPI_Win_unlock_all()\n", debug_rank);
#endif
}

void mpi_accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_add_error_class(MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Add_error_class(errorclass, ierr);
}

void mpi_add_error_code(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Add_error_code(errorclass, errorcode, ierr);
}

void mpi_add_error_string(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Add_error_string(errorcode, string, ierr, string_len);
}

void mpi_iallgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_iallgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_alloc_mem(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Alloc_mem(size, info, baseptr, ierr);
}

void mpi_iallreduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_ialltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ialltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ialltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_ibarrier(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibarrier(comm, request, ierr);
}

void mpi_ibcast(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibcast(buffer, count, datatype, root, comm, request, ierr);
}

void mpi_bsend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_buffer_attach(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_attach(buffer, size, ierr);
}

void mpi_buffer_detach(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_detach(buffer, size, ierr);
}

void mpi_cancel(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Cancel(request, ierr);
}

void mpi_cart_coords(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_coords(comm, rank, maxdims, coords, ierr);
}

void mpi_cart_create(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr)
{
	FMPI_Cart_create(old_comm, ndims, dims, periods, reorder, comm_cart, ierr);
}

void mpi_cart_get(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_get(comm, maxdims, dims, periods, coords, ierr);
}

void mpi_cart_map(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Cart_map(comm, ndims, dims, periods, newrank, ierr);
}

void mpi_cart_rank(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Cart_rank(comm, coords, rank, ierr);
}

void mpi_cart_shift(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr)
{
	FMPI_Cart_shift(comm, direction, disp, rank_source, rank_dest, ierr);
}

void mpi_cart_sub(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr)
{
	FMPI_Cart_sub(comm, remain_dims, new_comm, ierr);
}

void mpi_cartdim_get(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr)
{
	FMPI_Cartdim_get(comm, ndims, ierr);
}

void mpi_close_port(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Close_port(port_name, ierr, port_name_len);
}

void mpi_comm_accept(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_accept(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_call_errhandler(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Comm_call_errhandler(comm, errorcode, ierr);
}

void mpi_comm_compare(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Comm_compare(comm1, comm2, result, ierr);
}

void mpi_comm_connect(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_connect(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_create_errhandler(function, errhandler, ierr);
}

void mpi_comm_create_keyval(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr);
}

void mpi_comm_create_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create_group(comm, group, tag, newcomm, ierr);
}

void mpi_comm_create(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create(comm, group, newcomm, ierr);
}

void mpi_comm_delete_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_delete_attr(comm, comm_keyval, ierr);
}

void mpi_comm_disconnect(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_disconnect(comm, ierr);
}

void mpi_comm_dup(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup(comm, newcomm, ierr);
}

void mpi_comm_idup(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Comm_idup(comm, newcomm, request, ierr);
}

void mpi_comm_dup_with_info(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup_with_info(comm, info, newcomm, ierr);
}

void mpi_comm_free_keyval(MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_free_keyval(comm_keyval, ierr);
}

void mpi_comm_free(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_free(comm, ierr);
}

void mpi_comm_get_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag, ierr);
}

void mpi_dist_graph_create(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint * newcomm, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm, ierr);
}

void mpi_dist_graph_create_adjacent(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create_adjacent(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph, ierr);
}

void mpi_dist_graph_neighbors(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights, ierr);
}

void mpi_dist_graph_neighbors_count(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors_count(comm, inneighbors, outneighbors, weighted, ierr);
}

void mpi_comm_get_errhandler(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr)
{
	FMPI_Comm_get_errhandler(comm, erhandler, ierr);
}

void mpi_comm_get_info(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Comm_get_info(comm, info_used, ierr);
}

void mpi_comm_get_name(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_get_name(comm, comm_name, resultlen, ierr, name_len);
}

void mpi_comm_get_parent(MPI_Fint *parent, MPI_Fint *ierr)
{
	FMPI_Comm_get_parent(parent, ierr);
}

void mpi_comm_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_group(comm, group, ierr);
}

void mpi_comm_join(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr)
{
	FMPI_Comm_join(fd, intercomm, ierr);
}

void mpi_comm_rank(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Comm_rank(comm, rank, ierr);
}

void mpi_comm_remote_group(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_remote_group(comm, group, ierr);
}

void mpi_comm_remote_size(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_remote_size(comm, size, ierr);
}

void mpi_comm_set_attr(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Comm_set_attr(comm, comm_keyval, attribute_val, ierr);
}

void mpi_comm_set_errhandler(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_set_errhandler(comm, errhandler, ierr);
}

void mpi_comm_set_info(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Comm_set_info(comm, info, ierr);
}

void mpi_comm_set_name(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_set_name(comm, comm_name, ierr, name_len);
}

void mpi_comm_size(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_size(comm, size, ierr);
}

void mpi_comm_spawn(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len)
{
	FMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes, ierr, cmd_len, string_len);
}

void mpi_comm_spawn_multiple(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len)
{
	FMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes, ierr, cmd_string_len, argv_string_len);
}

void mpi_comm_test_inter(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_test_inter(comm, flag, ierr);
}

void mpi_compare_and_swap(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Compare_and_swap(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win, ierr);
}

void mpi_dims_create(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr)
{
	FMPI_Dims_create(nnodes, ndims, dims, ierr);
}

void mpi_errhandler_free(MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Errhandler_free(errhandler, ierr);
}

void mpi_error_class(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Error_class(errorcode, errorclass, ierr);
}

void mpi_error_string(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Error_string(errorcode, string, resultlen, ierr, string_len);
}

void mpi_fetch_and_op(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, ierr);
}

void mpi_iexscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iexscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_file_call_errhandler(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_File_call_errhandler(fh, errorcode, ierr);
}

void mpi_file_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_create_errhandler(function, errhandler, ierr);
}

void mpi_file_set_errhandler( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_set_errhandler( file, errhandler, ierr);
}

void mpi_file_get_errhandler( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_get_errhandler( file, errhandler, ierr);
}

void mpi_file_open(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_File_open(comm, filename, amode, info, fh, ierr, name_len);
}

void mpi_file_close(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_close(fh, ierr);
}

void mpi_file_delete(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len)
{
	FMPI_File_delete(filename, info, ierr, filename_len);
}

void mpi_file_set_size(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_set_size(fh, size, ierr);
}

void mpi_file_preallocate(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_preallocate(fh, size, ierr);
}

void mpi_file_get_size(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_get_size(fh, size, ierr);
}

void mpi_file_get_group(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_File_get_group(fh, group, ierr);
}

void mpi_file_get_amode(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr)
{
	FMPI_File_get_amode(fh, amode, ierr);
}

void mpi_file_set_info(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_File_set_info(fh, info, ierr);
}

void mpi_file_get_info(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_File_get_info(fh, info_used, ierr);
}

void mpi_file_set_view(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_set_view(fh, disp, etype, filetype, datarep, info, ierr, datarep_len);
}

void mpi_file_get_view(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_get_view(fh, disp, etype, filetype, datarep, ierr, datarep_len);
}

void mpi_file_read_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_read_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_iread_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iread_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at_all(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_read(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read(fh, buf, count, datatype, status, ierr);
}

void mpi_file_read_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iread_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_all(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_seek(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek(fh, offset, whence, ierr);
}

void mpi_file_get_position(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position(fh, offset, ierr);
}

void mpi_file_get_byte_offset(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr)
{
	FMPI_File_get_byte_offset(fh, offset, disp, ierr);
}

void mpi_file_read_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_shared(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_read_ordered(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_ordered(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_seek_shared(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek_shared(fh, offset, whence, ierr);
}

void mpi_file_get_position_shared(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position_shared(fh, offset, ierr);
}

void mpi_file_read_at_all_begin(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_read_at_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_end(fh, buf, status, ierr);
}

void mpi_file_write_at_all_begin(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_write_at_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_end(fh, buf, status, ierr);
}

void mpi_file_read_all_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all_end(fh, buf, status, ierr);
}

void mpi_file_write_all_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_all_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all_end(fh, buf, status, ierr);
}

void mpi_file_read_ordered_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_ordered_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_end(fh, buf, status, ierr);
}

void mpi_file_write_ordered_begin(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_ordered_end(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_end(fh, buf, status, ierr);
}

void mpi_file_get_type_extent(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_File_get_type_extent(fh, datatype, extent, ierr);
}

void mpi_file_set_atomicity(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_set_atomicity(fh, flag, ierr);
}

void mpi_file_get_atomicity(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_get_atomicity(fh, flag, ierr);
}

void mpi_finalized(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Finalized(flag, ierr);
}

void mpi_free_mem(MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Free_mem(base, ierr);
}

void mpi_igather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_igatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request, ierr);
}

void mpi_get_address(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr)
{
	FMPI_Get_address(location, address, ierr);
}

void mpi_get_count(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_count(status, datatype, count, ierr);
}

void mpi_get_elements(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements(status, datatype, count, ierr);
}

void mpi_get_elements_x(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements_x(status, datatype, count, ierr);
}

void mpi_get(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_get_accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_get_library_version(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len)
{
	FMPI_Get_library_version(version, resultlen, ierr, version_len);
}

void mpi_get_processor_name(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Get_processor_name(name, resultlen, ierr, name_len);
}

void mpi_get_version(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
	FMPI_Get_version(version, subversion, ierr);
}

void mpi_graph_create(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr)
{
	FMPI_Graph_create(comm_old, nnodes, index, edges, reorder, comm_graph, ierr);
}

void mpi_graph_get(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
	FMPI_Graph_get(comm, maxindex, maxedges, index, edges, ierr);
}

void mpi_graph_map(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Graph_map(comm, nnodes, index, edges, newrank, ierr);
}

void mpi_graph_neighbors_count(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors_count(comm, rank, nneighbors, ierr);
}

void mpi_graph_neighbors(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors, ierr);
}

void mpi_graphdims_get(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr)
{
	FMPI_Graphdims_get(comm, nnodes, nedges, ierr);
}

void mpi_grequest_complete(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_complete(request, ierr);
}

void mpi_grequest_start(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request, ierr);
}

void mpi_group_compare(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Group_compare(group1, group2, result, ierr);
}

void mpi_group_difference(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_difference(group1, group2, newgroup, ierr);
}

void mpi_group_excl(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_excl(group, n, ranks, newgroup, ierr);
}

void mpi_group_free(MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Group_free(group, ierr);
}

void mpi_group_incl(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_incl(group, n, ranks, newgroup, ierr);
}

void mpi_group_intersection(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_intersection(group1, group2, newgroup, ierr);
}

void mpi_group_range_excl(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_excl(group, n, ranges, newgroup, ierr);
}

void mpi_group_range_incl(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_incl(group, n, ranges, newgroup, ierr);
}

void mpi_group_rank(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Group_rank(group, rank, ierr);
}

void mpi_group_size(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Group_size(group, size, ierr);
}

void mpi_group_translate_ranks(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
	FMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2, ierr);
}

void mpi_group_union(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_union(group1, group2, newgroup, ierr);
}

void mpi_improbe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Improbe(source, tag, comm, flag, message, status, ierr);
}

void mpi_imrecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Imrecv(buf, count, type, message, request, ierr);
}

void mpi_info_create(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_create(info, ierr);
}

void mpi_info_delete(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_delete(info, key, ierr, key_len);
}

void mpi_info_dup(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr)
{
	FMPI_Info_dup(info, newinfo, ierr);
}

void mpi_info_free(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_free(info, ierr);
}

void mpi_info_get(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_get(info, key, valuelen, value, flag, ierr, key_len, value_len);
}

void mpi_info_get_nkeys(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
	FMPI_Info_get_nkeys(info, nkeys, ierr);
}

void mpi_info_get_nthkey(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_nthkey(info, n, key, ierr, key_len);
}

void mpi_info_get_valuelen(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_valuelen(info, key, valuelen, flag, ierr, key_len);
}

void mpi_info_set(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_set(info, key, value, ierr, key_len, value_len);
}

void mpi_initialized(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Initialized(flag, ierr);
}

void mpi_intercomm_create(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_create(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr);
}

void mpi_intercomm_merge(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_merge(intercomm, high, newintercomm, ierr);
}

void mpi_is_thread_main(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Is_thread_main(flag, ierr);
}

void mpi_lookup_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Lookup_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_mprobe(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mprobe(source, tag, comm, message, status, ierr);
}

void mpi_mrecv(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mrecv(buf, count, type, message, status, ierr);
}

void mpi_ineighbor_allgather(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_allgatherv(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoall(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallw(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_op_commutative(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr)
{
	FMPI_Op_commutative(op, commute, ierr);
}

void mpi_op_create(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_create(function, commute, op, ierr);
}

void mpi_open_port(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Open_port(info, port_name, ierr, port_name_len);
}

void mpi_op_free(MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_free(op, ierr);
}

void mpi_pack_external(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outsize, position, ierr, datarep_len);
}

void mpi_pack_external_size(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external_size(datarep, incount, datatype, size, ierr, datarep_len);
}

void mpi_pack(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierr);
}

void mpi_pack_size(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Pack_size(incount, datatype, comm, size, ierr);
}

void mpi_pcontrol(MPI_Fint *level, MPI_Fint *ierr)
{
	FMPI_Pcontrol(level, ierr);
}

void mpi_publish_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Publish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_put(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_query_thread(MPI_Fint *provided, MPI_Fint *ierr)
{
	FMPI_Query_thread(provided, ierr);
}

void mpi_raccumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_recv_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Recv_init(buf, count, datatype, source, tag, comm, request, ierr);
}

void mpi_ireduce(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request, ierr);
}

void mpi_ireduce_scatter(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request, ierr);
}

void mpi_reduce_scatter_block(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, ierr);
}

void mpi_ireduce_scatter_block(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request, ierr);
}

void mpi_register_datarep(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Register_datarep(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr, datarep_len);
}

void mpi_request_free(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Request_free(request, ierr);
}

void mpi_request_get_status(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Request_get_status(request, flag, status, ierr);
}

void mpi_rget(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
}

void mpi_rget_accumulate(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_rput(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_cout, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_cout, target_datatype, win, request, ierr);
}

void mpi_rsend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_iscan(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_iscatter(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_iscatterv(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_send_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Send_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ssend_init(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_start(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Start(request, ierr);
}

void mpi_startall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr)
{
	FMPI_Startall(count, array_of_requests, ierr);
}

void mpi_status_set_cancelled(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Status_set_cancelled(status, flag, ierr);
}

void mpi_status_set_elements(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements(status, datatype, count, ierr);
}

void mpi_status_set_elements_x(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements_x(status, datatype, count, ierr);
}

void mpi_testall(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testall(count, array_of_requests, flag, array_of_statuses, ierr);
}

void mpi_testany(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Testany(count, array_of_requests, index, flag, status, ierr);
}

void mpi_test(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Test(request, flag, status, ierr);
}

void mpi_test_cancelled(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Test_cancelled(status, flag, ierr);
}

void mpi_testsome(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_topo_test(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Topo_test(comm, status, ierr);
}

void mpi_type_commit(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_commit(type, ierr);
}

void mpi_type_contiguous(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_contiguous(count, oldtype, newtype, ierr);
}

void mpi_type_create_darray(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_darray(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_f90_complex(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_complex(p, r, newtype, ierr);
}

void mpi_type_create_f90_integer(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_integer(r, newtype, ierr);
}

void mpi_type_create_f90_real(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_real(p, r, newtype, ierr);
}

void mpi_type_create_hindexed_block(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hindexed(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hvector(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_type_create_keyval(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr);
}

void mpi_type_create_indexed_block(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_struct(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_struct(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype, ierr);
}

void mpi_type_create_subarray(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_subarray(ndims, size_array, subsize_array, start_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_resized(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_resized(oldtype, lb, extent, newtype, ierr);
}

void mpi_type_delete_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_delete_attr(type, type_keyval, ierr);
}

void mpi_type_dup(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_dup(type, newtype, ierr);
}

void mpi_type_free(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_free(type, ierr);
}

void mpi_type_free_keyval(MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_free_keyval(type_keyval, ierr);
}

void mpi_type_get_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Type_get_attr(type, type_keyval, attribute_val, flag, ierr);
}

void mpi_type_get_contents(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr)
{
	FMPI_Type_get_contents(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes, ierr);
}

void mpi_type_get_envelope(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
	FMPI_Type_get_envelope(type, num_integers, num_addresses, num_datatypes, combiner, ierr);
}

void mpi_type_get_extent(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent(type, lb, extent, ierr);
}

void mpi_type_get_extent_x(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent_x(type, lb, extent, ierr);
}

void mpi_type_get_name(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_get_name(type, type_name, resultlen, ierr, type_name_len);
}

void mpi_type_get_true_extent(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent(datatype, true_lb, true_extent, ierr);
}

void mpi_type_get_true_extent_x(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent_x(datatype, true_lb, true_extent, ierr);
}

void mpi_type_indexed(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_indexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_match_size(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_match_size(typeclass, size, type, ierr);
}

void mpi_type_set_attr(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr)
{
	FMPI_Type_set_attr(type, type_keyval, attr_val, ierr);
}

void mpi_type_set_name(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_set_name(type, type_name, ierr, type_name_len);
}

void mpi_type_size(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size(type, size, ierr);
}

void mpi_type_size_x(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size_x(type, size, ierr);
}

void mpi_type_vector(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_vector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_unpack(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierr);
}

void mpi_unpublish_name(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Unpublish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_unpack_external(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype, ierr, datarep_len);
}

void mpi_win_allocate(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_allocate_shared(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_attach(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Win_attach(win, base, size, ierr);
}

void mpi_win_call_errhandler(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Win_call_errhandler(win, errorcode, ierr);
}

void mpi_win_complete(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_complete(win, ierr);
}

void mpi_win_create(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create(base, size, disp_unit, info, comm, win, ierr);
}

void mpi_win_create_dynamic(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create_dynamic(info, comm, win, ierr);
}

void mpi_win_create_errhandler(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_create_errhandler(function, errhandler, ierr);
}

void mpi_win_create_keyval(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr);
}

void mpi_win_delete_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_delete_attr(win, win_keyval, ierr);
}

void mpi_win_detach(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Win_detach(win, base, ierr);
}

void mpi_win_fence(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_fence(assert, win, ierr);
}

void mpi_win_free(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_free(win, ierr);
}

void mpi_win_free_keyval(MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_free_keyval(win_keyval, ierr);
}

void mpi_win_get_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_get_attr(win, win_keyval, attribute_val, flag, ierr);
}

void mpi_win_get_errhandler(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_get_errhandler(win, errhandler, ierr);
}

void mpi_win_get_group(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Win_get_group(win, group, ierr);
}

void mpi_win_get_info(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Win_get_info(win, info_used, ierr);
}

void mpi_win_get_name(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_get_name(win, win_name, resultlen, ierr, win_name_len);
}

void mpi_win_post(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_post(group, assert, win, ierr);
}

void mpi_win_set_attr(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Win_set_attr(win, win_keyval, attribute_val, ierr);
}

void mpi_win_set_errhandler(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_set_errhandler(win, errhandler, ierr);
}

void mpi_win_set_info(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Win_set_info(win, info, ierr);
}

void mpi_win_set_name(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_set_name(win, win_name, ierr, win_name_len);
}

void mpi_win_shared_query(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Win_shared_query(win, rank, size, disp_unit, baseptr, ierr);
}

void mpi_win_start(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_start(group, assert, win, ierr);
}

void mpi_win_test(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_test(win, flag, ierr);
}

void mpi_win_unlock(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock(rank, win, ierr);
}

void mpi_win_unlock_all(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock_all(win, ierr);
}

void mpi_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_add_error_class_(MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Add_error_class(errorclass, ierr);
}

void mpi_add_error_code_(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Add_error_code(errorclass, errorcode, ierr);
}

void mpi_add_error_string_(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Add_error_string(errorcode, string, ierr, string_len);
}

void mpi_iallgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_iallgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_alloc_mem_(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Alloc_mem(size, info, baseptr, ierr);
}

void mpi_iallreduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_ialltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ialltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ialltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_ibarrier_(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibarrier(comm, request, ierr);
}

void mpi_ibcast_(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibcast(buffer, count, datatype, root, comm, request, ierr);
}

void mpi_bsend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_buffer_attach_(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_attach(buffer, size, ierr);
}

void mpi_buffer_detach_(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_detach(buffer, size, ierr);
}

void mpi_cancel_(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Cancel(request, ierr);
}

void mpi_cart_coords_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_coords(comm, rank, maxdims, coords, ierr);
}

void mpi_cart_create_(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr)
{
	FMPI_Cart_create(old_comm, ndims, dims, periods, reorder, comm_cart, ierr);
}

void mpi_cart_get_(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_get(comm, maxdims, dims, periods, coords, ierr);
}

void mpi_cart_map_(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Cart_map(comm, ndims, dims, periods, newrank, ierr);
}

void mpi_cart_rank_(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Cart_rank(comm, coords, rank, ierr);
}

void mpi_cart_shift_(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr)
{
	FMPI_Cart_shift(comm, direction, disp, rank_source, rank_dest, ierr);
}

void mpi_cart_sub_(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr)
{
	FMPI_Cart_sub(comm, remain_dims, new_comm, ierr);
}

void mpi_cartdim_get_(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr)
{
	FMPI_Cartdim_get(comm, ndims, ierr);
}

void mpi_close_port_(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Close_port(port_name, ierr, port_name_len);
}

void mpi_comm_accept_(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_accept(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_call_errhandler_(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Comm_call_errhandler(comm, errorcode, ierr);
}

void mpi_comm_compare_(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Comm_compare(comm1, comm2, result, ierr);
}

void mpi_comm_connect_(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_connect(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_create_errhandler(function, errhandler, ierr);
}

void mpi_comm_create_keyval_(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr);
}

void mpi_comm_create_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create_group(comm, group, tag, newcomm, ierr);
}

void mpi_comm_create_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create(comm, group, newcomm, ierr);
}

void mpi_comm_delete_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_delete_attr(comm, comm_keyval, ierr);
}

void mpi_comm_disconnect_(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_disconnect(comm, ierr);
}

void mpi_comm_dup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup(comm, newcomm, ierr);
}

void mpi_comm_idup_(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Comm_idup(comm, newcomm, request, ierr);
}

void mpi_comm_dup_with_info_(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup_with_info(comm, info, newcomm, ierr);
}

void mpi_comm_free_keyval_(MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_free_keyval(comm_keyval, ierr);
}

void mpi_comm_free_(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_free(comm, ierr);
}

void mpi_comm_get_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag, ierr);
}

void mpi_dist_graph_create_(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint * newcomm, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm, ierr);
}

void mpi_dist_graph_create_adjacent_(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create_adjacent(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph, ierr);
}

void mpi_dist_graph_neighbors_(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights, ierr);
}

void mpi_dist_graph_neighbors_count_(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors_count(comm, inneighbors, outneighbors, weighted, ierr);
}

void mpi_comm_get_errhandler_(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr)
{
	FMPI_Comm_get_errhandler(comm, erhandler, ierr);
}

void mpi_comm_get_info_(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Comm_get_info(comm, info_used, ierr);
}

void mpi_comm_get_name_(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_get_name(comm, comm_name, resultlen, ierr, name_len);
}

void mpi_comm_get_parent_(MPI_Fint *parent, MPI_Fint *ierr)
{
	FMPI_Comm_get_parent(parent, ierr);
}

void mpi_comm_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_group(comm, group, ierr);
}

void mpi_comm_join_(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr)
{
	FMPI_Comm_join(fd, intercomm, ierr);
}

void mpi_comm_rank_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Comm_rank(comm, rank, ierr);
}

void mpi_comm_remote_group_(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_remote_group(comm, group, ierr);
}

void mpi_comm_remote_size_(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_remote_size(comm, size, ierr);
}

void mpi_comm_set_attr_(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Comm_set_attr(comm, comm_keyval, attribute_val, ierr);
}

void mpi_comm_set_errhandler_(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_set_errhandler(comm, errhandler, ierr);
}

void mpi_comm_set_info_(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Comm_set_info(comm, info, ierr);
}

void mpi_comm_set_name_(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_set_name(comm, comm_name, ierr, name_len);
}

void mpi_comm_size_(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_size(comm, size, ierr);
}

void mpi_comm_spawn_(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len)
{
	FMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes, ierr, cmd_len, string_len);
}

void mpi_comm_spawn_multiple_(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len)
{
	FMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes, ierr, cmd_string_len, argv_string_len);
}

void mpi_comm_test_inter_(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_test_inter(comm, flag, ierr);
}

void mpi_compare_and_swap_(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Compare_and_swap(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win, ierr);
}

void mpi_dims_create_(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr)
{
	FMPI_Dims_create(nnodes, ndims, dims, ierr);
}

void mpi_errhandler_free_(MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Errhandler_free(errhandler, ierr);
}

void mpi_error_class_(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Error_class(errorcode, errorclass, ierr);
}

void mpi_error_string_(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Error_string(errorcode, string, resultlen, ierr, string_len);
}

void mpi_fetch_and_op_(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, ierr);
}

void mpi_iexscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iexscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_file_call_errhandler_(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_File_call_errhandler(fh, errorcode, ierr);
}

void mpi_file_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_create_errhandler(function, errhandler, ierr);
}

void mpi_file_set_errhandler_( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_set_errhandler( file, errhandler, ierr);
}

void mpi_file_get_errhandler_( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_get_errhandler( file, errhandler, ierr);
}

void mpi_file_open_(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_File_open(comm, filename, amode, info, fh, ierr, name_len);
}

void mpi_file_close_(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_close(fh, ierr);
}

void mpi_file_delete_(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len)
{
	FMPI_File_delete(filename, info, ierr, filename_len);
}

void mpi_file_set_size_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_set_size(fh, size, ierr);
}

void mpi_file_preallocate_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_preallocate(fh, size, ierr);
}

void mpi_file_get_size_(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_get_size(fh, size, ierr);
}

void mpi_file_get_group_(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_File_get_group(fh, group, ierr);
}

void mpi_file_get_amode_(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr)
{
	FMPI_File_get_amode(fh, amode, ierr);
}

void mpi_file_set_info_(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_File_set_info(fh, info, ierr);
}

void mpi_file_get_info_(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_File_get_info(fh, info_used, ierr);
}

void mpi_file_set_view_(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_set_view(fh, disp, etype, filetype, datarep, info, ierr, datarep_len);
}

void mpi_file_get_view_(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_get_view(fh, disp, etype, filetype, datarep, ierr, datarep_len);
}

void mpi_file_read_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_read_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_iread_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iread_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at_all_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_read_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read(fh, buf, count, datatype, status, ierr);
}

void mpi_file_read_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iread_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_all_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_seek_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek(fh, offset, whence, ierr);
}

void mpi_file_get_position_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position(fh, offset, ierr);
}

void mpi_file_get_byte_offset_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr)
{
	FMPI_File_get_byte_offset(fh, offset, disp, ierr);
}

void mpi_file_read_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_shared_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_read_ordered_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_ordered_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_seek_shared_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek_shared(fh, offset, whence, ierr);
}

void mpi_file_get_position_shared_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position_shared(fh, offset, ierr);
}

void mpi_file_read_at_all_begin_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_read_at_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_end(fh, buf, status, ierr);
}

void mpi_file_write_at_all_begin_(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_write_at_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_end(fh, buf, status, ierr);
}

void mpi_file_read_all_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all_end(fh, buf, status, ierr);
}

void mpi_file_write_all_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_all_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all_end(fh, buf, status, ierr);
}

void mpi_file_read_ordered_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_ordered_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_end(fh, buf, status, ierr);
}

void mpi_file_write_ordered_begin_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_ordered_end_(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_end(fh, buf, status, ierr);
}

void mpi_file_get_type_extent_(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_File_get_type_extent(fh, datatype, extent, ierr);
}

void mpi_file_set_atomicity_(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_set_atomicity(fh, flag, ierr);
}

void mpi_file_get_atomicity_(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_get_atomicity(fh, flag, ierr);
}

void mpi_finalized_(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Finalized(flag, ierr);
}

void mpi_free_mem_(MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Free_mem(base, ierr);
}

void mpi_igather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_igatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request, ierr);
}

void mpi_get_address_(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr)
{
	FMPI_Get_address(location, address, ierr);
}

void mpi_get_count_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_count(status, datatype, count, ierr);
}

void mpi_get_elements_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements(status, datatype, count, ierr);
}

void mpi_get_elements_x_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements_x(status, datatype, count, ierr);
}

void mpi_get_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_get_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_get_library_version_(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len)
{
	FMPI_Get_library_version(version, resultlen, ierr, version_len);
}

void mpi_get_processor_name_(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Get_processor_name(name, resultlen, ierr, name_len);
}

void mpi_get_version_(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
	FMPI_Get_version(version, subversion, ierr);
}

void mpi_graph_create_(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr)
{
	FMPI_Graph_create(comm_old, nnodes, index, edges, reorder, comm_graph, ierr);
}

void mpi_graph_get_(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
	FMPI_Graph_get(comm, maxindex, maxedges, index, edges, ierr);
}

void mpi_graph_map_(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Graph_map(comm, nnodes, index, edges, newrank, ierr);
}

void mpi_graph_neighbors_count_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors_count(comm, rank, nneighbors, ierr);
}

void mpi_graph_neighbors_(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors, ierr);
}

void mpi_graphdims_get_(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr)
{
	FMPI_Graphdims_get(comm, nnodes, nedges, ierr);
}

void mpi_grequest_complete_(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_complete(request, ierr);
}

void mpi_grequest_start_(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request, ierr);
}

void mpi_group_compare_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Group_compare(group1, group2, result, ierr);
}

void mpi_group_difference_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_difference(group1, group2, newgroup, ierr);
}

void mpi_group_excl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_excl(group, n, ranks, newgroup, ierr);
}

void mpi_group_free_(MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Group_free(group, ierr);
}

void mpi_group_incl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_incl(group, n, ranks, newgroup, ierr);
}

void mpi_group_intersection_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_intersection(group1, group2, newgroup, ierr);
}

void mpi_group_range_excl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_excl(group, n, ranges, newgroup, ierr);
}

void mpi_group_range_incl_(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_incl(group, n, ranges, newgroup, ierr);
}

void mpi_group_rank_(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Group_rank(group, rank, ierr);
}

void mpi_group_size_(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Group_size(group, size, ierr);
}

void mpi_group_translate_ranks_(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
	FMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2, ierr);
}

void mpi_group_union_(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_union(group1, group2, newgroup, ierr);
}

void mpi_improbe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Improbe(source, tag, comm, flag, message, status, ierr);
}

void mpi_imrecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Imrecv(buf, count, type, message, request, ierr);
}

void mpi_info_create_(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_create(info, ierr);
}

void mpi_info_delete_(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_delete(info, key, ierr, key_len);
}

void mpi_info_dup_(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr)
{
	FMPI_Info_dup(info, newinfo, ierr);
}

void mpi_info_free_(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_free(info, ierr);
}

void mpi_info_get_(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_get(info, key, valuelen, value, flag, ierr, key_len, value_len);
}

void mpi_info_get_nkeys_(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
	FMPI_Info_get_nkeys(info, nkeys, ierr);
}

void mpi_info_get_nthkey_(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_nthkey(info, n, key, ierr, key_len);
}

void mpi_info_get_valuelen_(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_valuelen(info, key, valuelen, flag, ierr, key_len);
}

void mpi_info_set_(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_set(info, key, value, ierr, key_len, value_len);
}

void mpi_initialized_(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Initialized(flag, ierr);
}

void mpi_intercomm_create_(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_create(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr);
}

void mpi_intercomm_merge_(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_merge(intercomm, high, newintercomm, ierr);
}

void mpi_is_thread_main_(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Is_thread_main(flag, ierr);
}

void mpi_lookup_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Lookup_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_mprobe_(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mprobe(source, tag, comm, message, status, ierr);
}

void mpi_mrecv_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mrecv(buf, count, type, message, status, ierr);
}

void mpi_ineighbor_allgather_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_allgatherv_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoall_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallw_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_op_commutative_(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr)
{
	FMPI_Op_commutative(op, commute, ierr);
}

void mpi_op_create_(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_create(function, commute, op, ierr);
}

void mpi_open_port_(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Open_port(info, port_name, ierr, port_name_len);
}

void mpi_op_free_(MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_free(op, ierr);
}

void mpi_pack_external_(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outsize, position, ierr, datarep_len);
}

void mpi_pack_external_size_(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external_size(datarep, incount, datatype, size, ierr, datarep_len);
}

void mpi_pack_(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierr);
}

void mpi_pack_size_(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Pack_size(incount, datatype, comm, size, ierr);
}

void mpi_pcontrol_(MPI_Fint *level, MPI_Fint *ierr)
{
	FMPI_Pcontrol(level, ierr);
}

void mpi_publish_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Publish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_put_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_query_thread_(MPI_Fint *provided, MPI_Fint *ierr)
{
	FMPI_Query_thread(provided, ierr);
}

void mpi_raccumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_recv_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Recv_init(buf, count, datatype, source, tag, comm, request, ierr);
}

void mpi_ireduce_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request, ierr);
}

void mpi_ireduce_scatter_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request, ierr);
}

void mpi_reduce_scatter_block_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, ierr);
}

void mpi_ireduce_scatter_block_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request, ierr);
}

void mpi_register_datarep_(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Register_datarep(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr, datarep_len);
}

void mpi_request_free_(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Request_free(request, ierr);
}

void mpi_request_get_status_(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Request_get_status(request, flag, status, ierr);
}

void mpi_rget_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
}

void mpi_rget_accumulate_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_rput_(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_cout, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_cout, target_datatype, win, request, ierr);
}

void mpi_rsend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_iscan_(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_iscatter_(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_iscatterv_(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_send_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Send_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ssend_init_(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_start_(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Start(request, ierr);
}

void mpi_startall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr)
{
	FMPI_Startall(count, array_of_requests, ierr);
}

void mpi_status_set_cancelled_(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Status_set_cancelled(status, flag, ierr);
}

void mpi_status_set_elements_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements(status, datatype, count, ierr);
}

void mpi_status_set_elements_x_(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements_x(status, datatype, count, ierr);
}

void mpi_testall_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testall(count, array_of_requests, flag, array_of_statuses, ierr);
}

void mpi_testany_(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Testany(count, array_of_requests, index, flag, status, ierr);
}

void mpi_test_(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Test(request, flag, status, ierr);
}

void mpi_test_cancelled_(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Test_cancelled(status, flag, ierr);
}

void mpi_testsome_(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_topo_test_(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Topo_test(comm, status, ierr);
}

void mpi_type_commit_(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_commit(type, ierr);
}

void mpi_type_contiguous_(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_contiguous(count, oldtype, newtype, ierr);
}

void mpi_type_create_darray_(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_darray(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_f90_complex_(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_complex(p, r, newtype, ierr);
}

void mpi_type_create_f90_integer_(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_integer(r, newtype, ierr);
}

void mpi_type_create_f90_real_(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_real(p, r, newtype, ierr);
}

void mpi_type_create_hindexed_block_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hindexed_(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hvector_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_type_create_keyval_(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr);
}

void mpi_type_create_indexed_block_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_struct_(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_struct(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype, ierr);
}

void mpi_type_create_subarray_(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_subarray(ndims, size_array, subsize_array, start_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_resized_(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_resized(oldtype, lb, extent, newtype, ierr);
}

void mpi_type_delete_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_delete_attr(type, type_keyval, ierr);
}

void mpi_type_dup_(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_dup(type, newtype, ierr);
}

void mpi_type_free_(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_free(type, ierr);
}

void mpi_type_free_keyval_(MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_free_keyval(type_keyval, ierr);
}

void mpi_type_get_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Type_get_attr(type, type_keyval, attribute_val, flag, ierr);
}

void mpi_type_get_contents_(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr)
{
	FMPI_Type_get_contents(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes, ierr);
}

void mpi_type_get_envelope_(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
	FMPI_Type_get_envelope(type, num_integers, num_addresses, num_datatypes, combiner, ierr);
}

void mpi_type_get_extent_(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent(type, lb, extent, ierr);
}

void mpi_type_get_extent_x_(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent_x(type, lb, extent, ierr);
}

void mpi_type_get_name_(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_get_name(type, type_name, resultlen, ierr, type_name_len);
}

void mpi_type_get_true_extent_(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent(datatype, true_lb, true_extent, ierr);
}

void mpi_type_get_true_extent_x_(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent_x(datatype, true_lb, true_extent, ierr);
}

void mpi_type_indexed_(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_indexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_match_size_(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_match_size(typeclass, size, type, ierr);
}

void mpi_type_set_attr_(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr)
{
	FMPI_Type_set_attr(type, type_keyval, attr_val, ierr);
}

void mpi_type_set_name_(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_set_name(type, type_name, ierr, type_name_len);
}

void mpi_type_size_(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size(type, size, ierr);
}

void mpi_type_size_x_(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size_x(type, size, ierr);
}

void mpi_type_vector_(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_vector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_unpack_(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierr);
}

void mpi_unpublish_name_(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Unpublish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_unpack_external_(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype, ierr, datarep_len);
}

void mpi_win_allocate_(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_allocate_shared_(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_attach_(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Win_attach(win, base, size, ierr);
}

void mpi_win_call_errhandler_(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Win_call_errhandler(win, errorcode, ierr);
}

void mpi_win_complete_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_complete(win, ierr);
}

void mpi_win_create_(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create(base, size, disp_unit, info, comm, win, ierr);
}

void mpi_win_create_dynamic_(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create_dynamic(info, comm, win, ierr);
}

void mpi_win_create_errhandler_(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_create_errhandler(function, errhandler, ierr);
}

void mpi_win_create_keyval_(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr);
}

void mpi_win_delete_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_delete_attr(win, win_keyval, ierr);
}

void mpi_win_detach_(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Win_detach(win, base, ierr);
}

void mpi_win_fence_(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_fence(assert, win, ierr);
}

void mpi_win_free_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_free(win, ierr);
}

void mpi_win_free_keyval_(MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_free_keyval(win_keyval, ierr);
}

void mpi_win_get_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_get_attr(win, win_keyval, attribute_val, flag, ierr);
}

void mpi_win_get_errhandler_(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_get_errhandler(win, errhandler, ierr);
}

void mpi_win_get_group_(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Win_get_group(win, group, ierr);
}

void mpi_win_get_info_(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Win_get_info(win, info_used, ierr);
}

void mpi_win_get_name_(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_get_name(win, win_name, resultlen, ierr, win_name_len);
}

void mpi_win_post_(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_post(group, assert, win, ierr);
}

void mpi_win_set_attr_(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Win_set_attr(win, win_keyval, attribute_val, ierr);
}

void mpi_win_set_errhandler_(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_set_errhandler(win, errhandler, ierr);
}

void mpi_win_set_info_(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Win_set_info(win, info, ierr);
}

void mpi_win_set_name_(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_set_name(win, win_name, ierr, win_name_len);
}

void mpi_win_shared_query_(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Win_shared_query(win, rank, size, disp_unit, baseptr, ierr);
}

void mpi_win_start_(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_start(group, assert, win, ierr);
}

void mpi_win_test_(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_test(win, flag, ierr);
}

void mpi_win_unlock_(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock(rank, win, ierr);
}

void mpi_win_unlock_all_(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock_all(win, ierr);
}

void mpi_accumulate__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_add_error_class__(MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Add_error_class(errorclass, ierr);
}

void mpi_add_error_code__(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Add_error_code(errorclass, errorcode, ierr);
}

void mpi_add_error_string__(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Add_error_string(errorcode, string, ierr, string_len);
}

void mpi_iallgather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_iallgatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_alloc_mem__(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Alloc_mem(size, info, baseptr, ierr);
}

void mpi_iallreduce__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_ialltoall__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ialltoallv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ialltoallw__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_ibarrier__(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibarrier(comm, request, ierr);
}

void mpi_ibcast__(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibcast(buffer, count, datatype, root, comm, request, ierr);
}

void mpi_bsend_init__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_buffer_attach__(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_attach(buffer, size, ierr);
}

void mpi_buffer_detach__(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_detach(buffer, size, ierr);
}

void mpi_cancel__(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Cancel(request, ierr);
}

void mpi_cart_coords__(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_coords(comm, rank, maxdims, coords, ierr);
}

void mpi_cart_create__(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr)
{
	FMPI_Cart_create(old_comm, ndims, dims, periods, reorder, comm_cart, ierr);
}

void mpi_cart_get__(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_get(comm, maxdims, dims, periods, coords, ierr);
}

void mpi_cart_map__(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Cart_map(comm, ndims, dims, periods, newrank, ierr);
}

void mpi_cart_rank__(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Cart_rank(comm, coords, rank, ierr);
}

void mpi_cart_shift__(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr)
{
	FMPI_Cart_shift(comm, direction, disp, rank_source, rank_dest, ierr);
}

void mpi_cart_sub__(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr)
{
	FMPI_Cart_sub(comm, remain_dims, new_comm, ierr);
}

void mpi_cartdim_get__(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr)
{
	FMPI_Cartdim_get(comm, ndims, ierr);
}

void mpi_close_port__(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Close_port(port_name, ierr, port_name_len);
}

void mpi_comm_accept__(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_accept(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_call_errhandler__(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Comm_call_errhandler(comm, errorcode, ierr);
}

void mpi_comm_compare__(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Comm_compare(comm1, comm2, result, ierr);
}

void mpi_comm_connect__(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_connect(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void mpi_comm_create_errhandler__(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_create_errhandler(function, errhandler, ierr);
}

void mpi_comm_create_keyval__(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr);
}

void mpi_comm_create_group__(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create_group(comm, group, tag, newcomm, ierr);
}

void mpi_comm_create__(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create(comm, group, newcomm, ierr);
}

void mpi_comm_delete_attr__(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_delete_attr(comm, comm_keyval, ierr);
}

void mpi_comm_disconnect__(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_disconnect(comm, ierr);
}

void mpi_comm_dup__(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup(comm, newcomm, ierr);
}

void mpi_comm_idup__(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Comm_idup(comm, newcomm, request, ierr);
}

void mpi_comm_dup_with_info__(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup_with_info(comm, info, newcomm, ierr);
}

void mpi_comm_free_keyval__(MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_free_keyval(comm_keyval, ierr);
}

void mpi_comm_free__(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_free(comm, ierr);
}

void mpi_comm_get_attr__(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag, ierr);
}

void mpi_dist_graph_create__(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint * newcomm, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm, ierr);
}

void mpi_dist_graph_create_adjacent__(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create_adjacent(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph, ierr);
}

void mpi_dist_graph_neighbors__(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights, ierr);
}

void mpi_dist_graph_neighbors_count__(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors_count(comm, inneighbors, outneighbors, weighted, ierr);
}

void mpi_comm_get_errhandler__(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr)
{
	FMPI_Comm_get_errhandler(comm, erhandler, ierr);
}

void mpi_comm_get_info__(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Comm_get_info(comm, info_used, ierr);
}

void mpi_comm_get_name__(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_get_name(comm, comm_name, resultlen, ierr, name_len);
}

void mpi_comm_get_parent__(MPI_Fint *parent, MPI_Fint *ierr)
{
	FMPI_Comm_get_parent(parent, ierr);
}

void mpi_comm_group__(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_group(comm, group, ierr);
}

void mpi_comm_join__(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr)
{
	FMPI_Comm_join(fd, intercomm, ierr);
}

void mpi_comm_rank__(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Comm_rank(comm, rank, ierr);
}

void mpi_comm_remote_group__(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_remote_group(comm, group, ierr);
}

void mpi_comm_remote_size__(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_remote_size(comm, size, ierr);
}

void mpi_comm_set_attr__(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Comm_set_attr(comm, comm_keyval, attribute_val, ierr);
}

void mpi_comm_set_errhandler__(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_set_errhandler(comm, errhandler, ierr);
}

void mpi_comm_set_info__(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Comm_set_info(comm, info, ierr);
}

void mpi_comm_set_name__(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_set_name(comm, comm_name, ierr, name_len);
}

void mpi_comm_size__(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_size(comm, size, ierr);
}

void mpi_comm_spawn__(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len)
{
	FMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes, ierr, cmd_len, string_len);
}

void mpi_comm_spawn_multiple__(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len)
{
	FMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes, ierr, cmd_string_len, argv_string_len);
}

void mpi_comm_test_inter__(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_test_inter(comm, flag, ierr);
}

void mpi_compare_and_swap__(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Compare_and_swap(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win, ierr);
}

void mpi_dims_create__(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr)
{
	FMPI_Dims_create(nnodes, ndims, dims, ierr);
}

void mpi_errhandler_free__(MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Errhandler_free(errhandler, ierr);
}

void mpi_error_class__(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Error_class(errorcode, errorclass, ierr);
}

void mpi_error_string__(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Error_string(errorcode, string, resultlen, ierr, string_len);
}

void mpi_fetch_and_op__(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, ierr);
}

void mpi_iexscan__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iexscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_file_call_errhandler__(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_File_call_errhandler(fh, errorcode, ierr);
}

void mpi_file_create_errhandler__(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_create_errhandler(function, errhandler, ierr);
}

void mpi_file_set_errhandler__( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_set_errhandler( file, errhandler, ierr);
}

void mpi_file_get_errhandler__( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_get_errhandler( file, errhandler, ierr);
}

void mpi_file_open__(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_File_open(comm, filename, amode, info, fh, ierr, name_len);
}

void mpi_file_close__(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_close(fh, ierr);
}

void mpi_file_delete__(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len)
{
	FMPI_File_delete(filename, info, ierr, filename_len);
}

void mpi_file_set_size__(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_set_size(fh, size, ierr);
}

void mpi_file_preallocate__(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_preallocate(fh, size, ierr);
}

void mpi_file_get_size__(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_get_size(fh, size, ierr);
}

void mpi_file_get_group__(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_File_get_group(fh, group, ierr);
}

void mpi_file_get_amode__(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr)
{
	FMPI_File_get_amode(fh, amode, ierr);
}

void mpi_file_set_info__(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_File_set_info(fh, info, ierr);
}

void mpi_file_get_info__(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_File_get_info(fh, info_used, ierr);
}

void mpi_file_set_view__(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_set_view(fh, disp, etype, filetype, datarep, info, ierr, datarep_len);
}

void mpi_file_get_view__(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_get_view(fh, disp, etype, filetype, datarep, ierr, datarep_len);
}

void mpi_file_read_at__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_read_at_all__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_write_at_all__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void mpi_file_iread_at__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iread_at_all__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_at_all__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void mpi_file_read__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read(fh, buf, count, datatype, status, ierr);
}

void mpi_file_read_all__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_all__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iread_all__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_all__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_all(fh, buf, count, datatype, request, ierr);
}

void mpi_file_seek__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek(fh, offset, whence, ierr);
}

void mpi_file_get_position__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position(fh, offset, ierr);
}

void mpi_file_get_byte_offset__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr)
{
	FMPI_File_get_byte_offset(fh, offset, disp, ierr);
}

void mpi_file_read_shared__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_shared__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_shared(fh, buf, count, datatype, status, ierr);
}

void mpi_file_iread_shared__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_iwrite_shared__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_shared(fh, buf, count, datatype, request, ierr);
}

void mpi_file_read_ordered__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_write_ordered__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered(fh, buf, count, datatype, status, ierr);
}

void mpi_file_seek_shared__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek_shared(fh, offset, whence, ierr);
}

void mpi_file_get_position_shared__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position_shared(fh, offset, ierr);
}

void mpi_file_read_at_all_begin__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_read_at_all_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_end(fh, buf, status, ierr);
}

void mpi_file_write_at_all_begin__(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void mpi_file_write_at_all_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_end(fh, buf, status, ierr);
}

void mpi_file_read_all_begin__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_all_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all_end(fh, buf, status, ierr);
}

void mpi_file_write_all_begin__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_all_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_all_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all_end(fh, buf, status, ierr);
}

void mpi_file_read_ordered_begin__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_read_ordered_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_end(fh, buf, status, ierr);
}

void mpi_file_write_ordered_begin__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_begin(fh, buf, count, datatype, ierr);
}

void mpi_file_write_ordered_end__(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_end(fh, buf, status, ierr);
}

void mpi_file_get_type_extent__(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_File_get_type_extent(fh, datatype, extent, ierr);
}

void mpi_file_set_atomicity__(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_set_atomicity(fh, flag, ierr);
}

void mpi_file_get_atomicity__(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_get_atomicity(fh, flag, ierr);
}

void mpi_finalized__(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Finalized(flag, ierr);
}

void mpi_free_mem__(MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Free_mem(base, ierr);
}

void mpi_igather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_igatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request, ierr);
}

void mpi_get_address__(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr)
{
	FMPI_Get_address(location, address, ierr);
}

void mpi_get_count__(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_count(status, datatype, count, ierr);
}

void mpi_get_elements__(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements(status, datatype, count, ierr);
}

void mpi_get_elements_x__(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements_x(status, datatype, count, ierr);
}

void mpi_get__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_get_accumulate__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void mpi_get_library_version__(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len)
{
	FMPI_Get_library_version(version, resultlen, ierr, version_len);
}

void mpi_get_processor_name__(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Get_processor_name(name, resultlen, ierr, name_len);
}

void mpi_get_version__(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
	FMPI_Get_version(version, subversion, ierr);
}

void mpi_graph_create__(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr)
{
	FMPI_Graph_create(comm_old, nnodes, index, edges, reorder, comm_graph, ierr);
}

void mpi_graph_get__(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
	FMPI_Graph_get(comm, maxindex, maxedges, index, edges, ierr);
}

void mpi_graph_map__(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Graph_map(comm, nnodes, index, edges, newrank, ierr);
}

void mpi_graph_neighbors_count__(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors_count(comm, rank, nneighbors, ierr);
}

void mpi_graph_neighbors__(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors, ierr);
}

void mpi_graphdims_get__(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr)
{
	FMPI_Graphdims_get(comm, nnodes, nedges, ierr);
}

void mpi_grequest_complete__(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_complete(request, ierr);
}

void mpi_grequest_start__(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request, ierr);
}

void mpi_group_compare__(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Group_compare(group1, group2, result, ierr);
}

void mpi_group_difference__(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_difference(group1, group2, newgroup, ierr);
}

void mpi_group_excl__(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_excl(group, n, ranks, newgroup, ierr);
}

void mpi_group_free__(MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Group_free(group, ierr);
}

void mpi_group_incl__(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_incl(group, n, ranks, newgroup, ierr);
}

void mpi_group_intersection__(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_intersection(group1, group2, newgroup, ierr);
}

void mpi_group_range_excl__(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_excl(group, n, ranges, newgroup, ierr);
}

void mpi_group_range_incl__(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_incl(group, n, ranges, newgroup, ierr);
}

void mpi_group_rank__(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Group_rank(group, rank, ierr);
}

void mpi_group_size__(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Group_size(group, size, ierr);
}

void mpi_group_translate_ranks__(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
	FMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2, ierr);
}

void mpi_group_union__(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_union(group1, group2, newgroup, ierr);
}

void mpi_improbe__(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Improbe(source, tag, comm, flag, message, status, ierr);
}

void mpi_imrecv__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Imrecv(buf, count, type, message, request, ierr);
}

void mpi_info_create__(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_create(info, ierr);
}

void mpi_info_delete__(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_delete(info, key, ierr, key_len);
}

void mpi_info_dup__(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr)
{
	FMPI_Info_dup(info, newinfo, ierr);
}

void mpi_info_free__(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_free(info, ierr);
}

void mpi_info_get__(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_get(info, key, valuelen, value, flag, ierr, key_len, value_len);
}

void mpi_info_get_nkeys__(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
	FMPI_Info_get_nkeys(info, nkeys, ierr);
}

void mpi_info_get_nthkey__(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_nthkey(info, n, key, ierr, key_len);
}

void mpi_info_get_valuelen__(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_valuelen(info, key, valuelen, flag, ierr, key_len);
}

void mpi_info_set__(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_set(info, key, value, ierr, key_len, value_len);
}

void mpi_initialized__(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Initialized(flag, ierr);
}

void mpi_intercomm_create__(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_create(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr);
}

void mpi_intercomm_merge__(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_merge(intercomm, high, newintercomm, ierr);
}

void mpi_is_thread_main__(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Is_thread_main(flag, ierr);
}

void mpi_lookup_name__(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Lookup_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_mprobe__(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mprobe(source, tag, comm, message, status, ierr);
}

void mpi_mrecv__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mrecv(buf, count, type, message, status, ierr);
}

void mpi_ineighbor_allgather__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_allgatherv__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoall__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void mpi_ineighbor_alltoallw__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void mpi_op_commutative__(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr)
{
	FMPI_Op_commutative(op, commute, ierr);
}

void mpi_op_create__(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_create(function, commute, op, ierr);
}

void mpi_open_port__(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Open_port(info, port_name, ierr, port_name_len);
}

void mpi_op_free__(MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_free(op, ierr);
}

void mpi_pack_external__(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outsize, position, ierr, datarep_len);
}

void mpi_pack_external_size__(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external_size(datarep, incount, datatype, size, ierr, datarep_len);
}

void mpi_pack__(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierr);
}

void mpi_pack_size__(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Pack_size(incount, datatype, comm, size, ierr);
}

void mpi_pcontrol__(MPI_Fint *level, MPI_Fint *ierr)
{
	FMPI_Pcontrol(level, ierr);
}

void mpi_publish_name__(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Publish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_put__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void mpi_query_thread__(MPI_Fint *provided, MPI_Fint *ierr)
{
	FMPI_Query_thread(provided, ierr);
}

void mpi_raccumulate__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_recv_init__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Recv_init(buf, count, datatype, source, tag, comm, request, ierr);
}

void mpi_ireduce__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request, ierr);
}

void mpi_ireduce_scatter__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request, ierr);
}

void mpi_reduce_scatter_block__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, ierr);
}

void mpi_ireduce_scatter_block__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request, ierr);
}

void mpi_register_datarep__(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Register_datarep(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr, datarep_len);
}

void mpi_request_free__(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Request_free(request, ierr);
}

void mpi_request_get_status__(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Request_get_status(request, flag, status, ierr);
}

void mpi_rget__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
}

void mpi_rget_accumulate__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void mpi_rput__(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_cout, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_cout, target_datatype, win, request, ierr);
}

void mpi_rsend_init__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_iscan__(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void mpi_iscatter__(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_iscatterv__(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void mpi_send_init__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Send_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_ssend_init__(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void mpi_start__(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Start(request, ierr);
}

void mpi_startall__(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr)
{
	FMPI_Startall(count, array_of_requests, ierr);
}

void mpi_status_set_cancelled__(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Status_set_cancelled(status, flag, ierr);
}

void mpi_status_set_elements__(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements(status, datatype, count, ierr);
}

void mpi_status_set_elements_x__(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements_x(status, datatype, count, ierr);
}

void mpi_testall__(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testall(count, array_of_requests, flag, array_of_statuses, ierr);
}

void mpi_testany__(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Testany(count, array_of_requests, index, flag, status, ierr);
}

void mpi_test__(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Test(request, flag, status, ierr);
}

void mpi_test_cancelled__(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Test_cancelled(status, flag, ierr);
}

void mpi_testsome__(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void mpi_topo_test__(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Topo_test(comm, status, ierr);
}

void mpi_type_commit__(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_commit(type, ierr);
}

void mpi_type_contiguous__(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_contiguous(count, oldtype, newtype, ierr);
}

void mpi_type_create_darray__(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_darray(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_f90_complex__(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_complex(p, r, newtype, ierr);
}

void mpi_type_create_f90_integer__(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_integer(r, newtype, ierr);
}

void mpi_type_create_f90_real__(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_real(p, r, newtype, ierr);
}

void mpi_type_create_hindexed_block__(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hindexed__(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_hvector__(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_type_create_keyval__(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr);
}

void mpi_type_create_indexed_block__(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_create_struct__(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_struct(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype, ierr);
}

void mpi_type_create_subarray__(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_subarray(ndims, size_array, subsize_array, start_array, order, oldtype, newtype, ierr);
}

void mpi_type_create_resized__(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_resized(oldtype, lb, extent, newtype, ierr);
}

void mpi_type_delete_attr__(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_delete_attr(type, type_keyval, ierr);
}

void mpi_type_dup__(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_dup(type, newtype, ierr);
}

void mpi_type_free__(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_free(type, ierr);
}

void mpi_type_free_keyval__(MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_free_keyval(type_keyval, ierr);
}

void mpi_type_get_attr__(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Type_get_attr(type, type_keyval, attribute_val, flag, ierr);
}

void mpi_type_get_contents__(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr)
{
	FMPI_Type_get_contents(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes, ierr);
}

void mpi_type_get_envelope__(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
	FMPI_Type_get_envelope(type, num_integers, num_addresses, num_datatypes, combiner, ierr);
}

void mpi_type_get_extent__(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent(type, lb, extent, ierr);
}

void mpi_type_get_extent_x__(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent_x(type, lb, extent, ierr);
}

void mpi_type_get_name__(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_get_name(type, type_name, resultlen, ierr, type_name_len);
}

void mpi_type_get_true_extent__(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent(datatype, true_lb, true_extent, ierr);
}

void mpi_type_get_true_extent_x__(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent_x(datatype, true_lb, true_extent, ierr);
}

void mpi_type_indexed__(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_indexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void mpi_type_match_size__(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_match_size(typeclass, size, type, ierr);
}

void mpi_type_set_attr__(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr)
{
	FMPI_Type_set_attr(type, type_keyval, attr_val, ierr);
}

void mpi_type_set_name__(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_set_name(type, type_name, ierr, type_name_len);
}

void mpi_type_size__(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size(type, size, ierr);
}

void mpi_type_size_x__(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size_x(type, size, ierr);
}

void mpi_type_vector__(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_vector(count, blocklength, stride, oldtype, newtype, ierr);
}

void mpi_unpack__(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierr);
}

void mpi_unpublish_name__(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Unpublish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void mpi_unpack_external__(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype, ierr, datarep_len);
}

void mpi_win_allocate__(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_allocate_shared__(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win, ierr);
}

void mpi_win_attach__(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Win_attach(win, base, size, ierr);
}

void mpi_win_call_errhandler__(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Win_call_errhandler(win, errorcode, ierr);
}

void mpi_win_complete__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_complete(win, ierr);
}

void mpi_win_create__(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create(base, size, disp_unit, info, comm, win, ierr);
}

void mpi_win_create_dynamic__(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create_dynamic(info, comm, win, ierr);
}

void mpi_win_create_errhandler__(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_create_errhandler(function, errhandler, ierr);
}

void mpi_win_create_keyval__(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr);
}

void mpi_win_delete_attr__(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_delete_attr(win, win_keyval, ierr);
}

void mpi_win_detach__(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Win_detach(win, base, ierr);
}

void mpi_win_fence__(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_fence(assert, win, ierr);
}

void mpi_win_free__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_free(win, ierr);
}

void mpi_win_free_keyval__(MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_free_keyval(win_keyval, ierr);
}

void mpi_win_get_attr__(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_get_attr(win, win_keyval, attribute_val, flag, ierr);
}

void mpi_win_get_errhandler__(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_get_errhandler(win, errhandler, ierr);
}

void mpi_win_get_group__(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Win_get_group(win, group, ierr);
}

void mpi_win_get_info__(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Win_get_info(win, info_used, ierr);
}

void mpi_win_get_name__(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_get_name(win, win_name, resultlen, ierr, win_name_len);
}

void mpi_win_post__(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_post(group, assert, win, ierr);
}

void mpi_win_set_attr__(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Win_set_attr(win, win_keyval, attribute_val, ierr);
}

void mpi_win_set_errhandler__(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_set_errhandler(win, errhandler, ierr);
}

void mpi_win_set_info__(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Win_set_info(win, info, ierr);
}

void mpi_win_set_name__(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_set_name(win, win_name, ierr, win_name_len);
}

void mpi_win_shared_query__(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Win_shared_query(win, rank, size, disp_unit, baseptr, ierr);
}

void mpi_win_start__(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_start(group, assert, win, ierr);
}

void mpi_win_test__(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_test(win, flag, ierr);
}

void mpi_win_unlock__(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock(rank, win, ierr);
}

void mpi_win_unlock_all__(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock_all(win, ierr);
}

void MPI_ACCUMULATE(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void MPI_ADD_ERROR_CLASS(MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Add_error_class(errorclass, ierr);
}

void MPI_ADD_ERROR_CODE(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Add_error_code(errorclass, errorcode, ierr);
}

void MPI_ADD_ERROR_STRING(MPI_Fint *errorcode, char *string, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Add_error_string(errorcode, string, ierr, string_len);
}

void MPI_IALLGATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void MPI_IALLGATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void MPI_ALLOC_MEM(MPI_Fint *size, MPI_Fint *info, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Alloc_mem(size, info, baseptr, ierr);
}

void MPI_IALLREDUCE(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void MPI_IALLTOALL(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void MPI_IALLTOALLV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void MPI_IALLTOALLW(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ialltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void MPI_IBARRIER(MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibarrier(comm, request, ierr);
}

void MPI_IBCAST(MPI_Fint *buffer, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ibcast(buffer, count, datatype, root, comm, request, ierr);
}

void MPI_BSEND_INIT(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_BUFFER_ATTACH(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_attach(buffer, size, ierr);
}

void MPI_BUFFER_DETACH(MPI_Fint *buffer, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Buffer_detach(buffer, size, ierr);
}

void MPI_CANCEL(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Cancel(request, ierr);
}

void MPI_CART_COORDS(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_coords(comm, rank, maxdims, coords, ierr);
}

void MPI_CART_CREATE(MPI_Fint *old_comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *comm_cart, MPI_Fint *ierr)
{
	FMPI_Cart_create(old_comm, ndims, dims, periods, reorder, comm_cart, ierr);
}

void MPI_CART_GET(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
	FMPI_Cart_get(comm, maxdims, dims, periods, coords, ierr);
}

void MPI_CART_MAP(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Cart_map(comm, ndims, dims, periods, newrank, ierr);
}

void MPI_CART_RANK(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Cart_rank(comm, coords, rank, ierr);
}

void MPI_CART_SHIFT(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *disp, MPI_Fint *rank_source, MPI_Fint *rank_dest, MPI_Fint *ierr)
{
	FMPI_Cart_shift(comm, direction, disp, rank_source, rank_dest, ierr);
}

void MPI_CART_SUB(MPI_Fint *comm, MPI_Fint *remain_dims, MPI_Fint *new_comm, MPI_Fint *ierr)
{
	FMPI_Cart_sub(comm, remain_dims, new_comm, ierr);
}

void MPI_CARTDIM_GET(MPI_Fint *comm, MPI_Fint *ndims, MPI_Fint *ierr)
{
	FMPI_Cartdim_get(comm, ndims, ierr);
}

void MPI_CLOSE_PORT(char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Close_port(port_name, ierr, port_name_len);
}

void MPI_COMM_ACCEPT(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_accept(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void MPI_COMM_CALL_ERRHANDLER(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Comm_call_errhandler(comm, errorcode, ierr);
}

void MPI_COMM_COMPARE(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Comm_compare(comm1, comm2, result, ierr);
}

void MPI_COMM_CONNECT(char *port_name, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Comm_connect(port_name, info, root, comm, newcomm, ierr, port_name_len);
}

void MPI_COMM_CREATE_ERRHANDLER(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_create_errhandler(function, errhandler, ierr);
}

void MPI_COMM_CREATE_KEYVAL(MPI_Fint *comm_copy_attr_fn, MPI_Fint *comm_delete_attr_fn, MPI_Fint *comm_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Comm_create_keyval(comm_copy_attr_fn, comm_delete_attr_fn, comm_keyval, extra_state, ierr);
}

void MPI_COMM_CREATE_GROUP(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create_group(comm, group, tag, newcomm, ierr);
}

void MPI_COMM_CREATE(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_create(comm, group, newcomm, ierr);
}

void MPI_COMM_DELETE_ATTR(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_delete_attr(comm, comm_keyval, ierr);
}

void MPI_COMM_DISCONNECT(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_disconnect(comm, ierr);
}

void MPI_COMM_DUP(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup(comm, newcomm, ierr);
}

void MPI_COMM_IDUP(MPI_Fint *comm, MPI_Fint *newcomm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Comm_idup(comm, newcomm, request, ierr);
}

void MPI_COMM_DUP_WITH_INFO(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *newcomm, MPI_Fint *ierr)
{
	FMPI_Comm_dup_with_info(comm, info, newcomm, ierr);
}

void MPI_COMM_FREE_KEYVAL(MPI_Fint *comm_keyval, MPI_Fint *ierr)
{
	FMPI_Comm_free_keyval(comm_keyval, ierr);
}

void MPI_COMM_FREE(MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Comm_free(comm, ierr);
}

void MPI_COMM_GET_ATTR(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_get_attr(comm, comm_keyval, attribute_val, flag, ierr);
}

void MPI_DIST_GRAPH_CREATE(MPI_Fint *comm_old, MPI_Fint *n, MPI_Fint *nodes, MPI_Fint *degrees, MPI_Fint *targets, MPI_Fint *weights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint * newcomm, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create(comm_old, n, nodes, degrees, targets, weights, info, reorder,  newcomm, ierr);
}

void MPI_DIST_GRAPH_CREATE_ADJACENT(MPI_Fint *comm_old, MPI_Fint *indegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *outdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *info, MPI_Fint *reorder, MPI_Fint *comm_dist_graph, MPI_Fint *ierr)
{
	FMPI_Dist_graph_create_adjacent(comm_old, indegree, sources, sourceweights, outdegree, destinations, destweights, info, reorder, comm_dist_graph, ierr);
}

void MPI_DIST_GRAPH_NEIGHBORS(MPI_Fint *comm, MPI_Fint *maxindegree, MPI_Fint *sources, MPI_Fint *sourceweights, MPI_Fint *maxoutdegree, MPI_Fint *destinations, MPI_Fint *destweights, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors(comm, maxindegree, sources, sourceweights, maxoutdegree, destinations, destweights, ierr);
}

void MPI_DIST_GRAPH_NEIGHBORS_COUNT(MPI_Fint *comm, MPI_Fint *inneighbors, MPI_Fint *outneighbors, MPI_Fint *weighted, MPI_Fint *ierr)
{
	FMPI_Dist_graph_neighbors_count(comm, inneighbors, outneighbors, weighted, ierr);
}

void MPI_COMM_GET_ERRHANDLER(MPI_Fint *comm, MPI_Fint *erhandler, MPI_Fint *ierr)
{
	FMPI_Comm_get_errhandler(comm, erhandler, ierr);
}

void MPI_COMM_GET_INFO(MPI_Fint *comm, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Comm_get_info(comm, info_used, ierr);
}

void MPI_COMM_GET_NAME(MPI_Fint *comm, char *comm_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_get_name(comm, comm_name, resultlen, ierr, name_len);
}

void MPI_COMM_GET_PARENT(MPI_Fint *parent, MPI_Fint *ierr)
{
	FMPI_Comm_get_parent(parent, ierr);
}

void MPI_COMM_GROUP(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_group(comm, group, ierr);
}

void MPI_COMM_JOIN(MPI_Fint *fd, MPI_Fint *intercomm, MPI_Fint *ierr)
{
	FMPI_Comm_join(fd, intercomm, ierr);
}

void MPI_COMM_RANK(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Comm_rank(comm, rank, ierr);
}

void MPI_COMM_REMOTE_GROUP(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Comm_remote_group(comm, group, ierr);
}

void MPI_COMM_REMOTE_SIZE(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_remote_size(comm, size, ierr);
}

void MPI_COMM_SET_ATTR(MPI_Fint *comm, MPI_Fint *comm_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Comm_set_attr(comm, comm_keyval, attribute_val, ierr);
}

void MPI_COMM_SET_ERRHANDLER(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Comm_set_errhandler(comm, errhandler, ierr);
}

void MPI_COMM_SET_INFO(MPI_Fint *comm, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Comm_set_info(comm, info, ierr);
}

void MPI_COMM_SET_NAME(MPI_Fint *comm, char *comm_name, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Comm_set_name(comm, comm_name, ierr, name_len);
}

void MPI_COMM_SIZE(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Comm_size(comm, size, ierr);
}

void MPI_COMM_SPAWN(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_len, MPI_Fint string_len)
{
	FMPI_Comm_spawn(command, argv, maxprocs, info, root, comm, intercomm, array_of_errcodes, ierr, cmd_len, string_len);
}

void MPI_COMM_SPAWN_MULTIPLE(MPI_Fint *count, char *array_of_commands, char *array_of_argv, MPI_Fint *array_of_maxprocs, MPI_Fint *array_of_info, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *intercomm, MPI_Fint *array_of_errcodes, MPI_Fint *ierr, MPI_Fint cmd_string_len, MPI_Fint argv_string_len)
{
	FMPI_Comm_spawn_multiple(count, array_of_commands, array_of_argv, array_of_maxprocs, array_of_info, root, comm, intercomm, array_of_errcodes, ierr, cmd_string_len, argv_string_len);
}

void MPI_COMM_TEST_INTER(MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Comm_test_inter(comm, flag, ierr);
}

void MPI_COMPARE_AND_SWAP(MPI_Fint *origin_addr, MPI_Fint *compare_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Compare_and_swap(origin_addr, compare_addr, result_addr, datatype, target_rank, target_disp, win, ierr);
}

void MPI_DIMS_CREATE(MPI_Fint *nnodes, MPI_Fint *ndims, MPI_Fint *dims, MPI_Fint *ierr)
{
	FMPI_Dims_create(nnodes, ndims, dims, ierr);
}

void MPI_ERRHANDLER_FREE(MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Errhandler_free(errhandler, ierr);
}

void MPI_ERROR_CLASS(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
	FMPI_Error_class(errorcode, errorclass, ierr);
}

void MPI_ERROR_STRING(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint string_len)
{
	FMPI_Error_string(errorcode, string, resultlen, ierr, string_len);
}

void MPI_FETCH_AND_OP(MPI_Fint *origin_addr, MPI_Fint *result_addr, MPI_Fint *datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, ierr);
}

void MPI_IEXSCAN(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iexscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void MPI_FILE_CALL_ERRHANDLER(MPI_Fint *fh, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_File_call_errhandler(fh, errorcode, ierr);
}

void MPI_FILE_CREATE_ERRHANDLER(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_create_errhandler(function, errhandler, ierr);
}

void MPI_FILE_SET_ERRHANDLER( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_set_errhandler( file, errhandler, ierr);
}

void MPI_FILE_GET_ERRHANDLER( MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_File_get_errhandler( file, errhandler, ierr);
}

void MPI_FILE_OPEN(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *fh, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_File_open(comm, filename, amode, info, fh, ierr, name_len);
}

void MPI_FILE_CLOSE(MPI_Fint *fh, MPI_Fint *ierr)
{
	FMPI_File_close(fh, ierr);
}

void MPI_FILE_DELETE(char *filename, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint filename_len)
{
	FMPI_File_delete(filename, info, ierr, filename_len);
}

void MPI_FILE_SET_SIZE(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_set_size(fh, size, ierr);
}

void MPI_FILE_PREALLOCATE(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_preallocate(fh, size, ierr);
}

void MPI_FILE_GET_SIZE(MPI_Fint *fh, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_File_get_size(fh, size, ierr);
}

void MPI_FILE_GET_GROUP(MPI_Fint *fh, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_File_get_group(fh, group, ierr);
}

void MPI_FILE_GET_AMODE(MPI_Fint *fh, MPI_Fint *amode, MPI_Fint *ierr)
{
	FMPI_File_get_amode(fh, amode, ierr);
}

void MPI_FILE_SET_INFO(MPI_Fint *fh, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_File_set_info(fh, info, ierr);
}

void MPI_FILE_GET_INFO(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_File_get_info(fh, info_used, ierr);
}

void MPI_FILE_SET_VIEW(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_set_view(fh, disp, etype, filetype, datarep, info, ierr, datarep_len);
}

void MPI_FILE_GET_VIEW(MPI_Fint *fh, MPI_Fint *disp, MPI_Fint *etype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_File_get_view(fh, disp, etype, filetype, datarep, ierr, datarep_len);
}

void MPI_FILE_READ_AT(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at(fh, offset, buf, count, datatype, status, ierr);
}

void MPI_FILE_READ_AT_ALL(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE_AT(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at(fh, offset, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE_AT_ALL(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all(fh, offset, buf, count, datatype, status, ierr);
}

void MPI_FILE_IREAD_AT(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at(fh, offset, buf, count, datatype, request, ierr);
}

void MPI_FILE_IWRITE_AT(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at(fh, offset, buf, count, datatype, request, ierr);
}

void MPI_FILE_IREAD_AT_ALL(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void MPI_FILE_IWRITE_AT_ALL(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_at_all(fh, offset, buf, count, datatype, request, ierr);
}

void MPI_FILE_READ(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_READ_ALL(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE_ALL(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_IREAD(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_IWRITE(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_IREAD_ALL(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_all(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_IWRITE_ALL(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_all(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_SEEK(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek(fh, offset, whence, ierr);
}

void MPI_FILE_GET_POSITION(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position(fh, offset, ierr);
}

void MPI_FILE_GET_BYTE_OFFSET(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *disp, MPI_Fint *ierr)
{
	FMPI_File_get_byte_offset(fh, offset, disp, ierr);
}

void MPI_FILE_READ_SHARED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_shared(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE_SHARED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_shared(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_IREAD_SHARED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iread_shared(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_IWRITE_SHARED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_File_iwrite_shared(fh, buf, count, datatype, request, ierr);
}

void MPI_FILE_READ_ORDERED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_WRITE_ORDERED(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered(fh, buf, count, datatype, status, ierr);
}

void MPI_FILE_SEEK_SHARED(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
	FMPI_File_seek_shared(fh, offset, whence, ierr);
}

void MPI_FILE_GET_POSITION_SHARED(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *ierr)
{
	FMPI_File_get_position_shared(fh, offset, ierr);
}

void MPI_FILE_READ_AT_ALL_BEGIN(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void MPI_FILE_READ_AT_ALL_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_at_all_end(fh, buf, status, ierr);
}

void MPI_FILE_WRITE_AT_ALL_BEGIN(MPI_Fint *fh, MPI_Fint *offset, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_begin(fh, offset, buf, count, datatype, ierr);
}

void MPI_FILE_WRITE_AT_ALL_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_at_all_end(fh, buf, status, ierr);
}

void MPI_FILE_READ_ALL_BEGIN(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_all_begin(fh, buf, count, datatype, ierr);
}

void MPI_FILE_READ_ALL_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_all_end(fh, buf, status, ierr);
}

void MPI_FILE_WRITE_ALL_BEGIN(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_all_begin(fh, buf, count, datatype, ierr);
}

void MPI_FILE_WRITE_ALL_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_all_end(fh, buf, status, ierr);
}

void MPI_FILE_READ_ORDERED_BEGIN(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_begin(fh, buf, count, datatype, ierr);
}

void MPI_FILE_READ_ORDERED_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_read_ordered_end(fh, buf, status, ierr);
}

void MPI_FILE_WRITE_ORDERED_BEGIN(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_begin(fh, buf, count, datatype, ierr);
}

void MPI_FILE_WRITE_ORDERED_END(MPI_Fint *fh, MPI_Fint *buf, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_File_write_ordered_end(fh, buf, status, ierr);
}

void MPI_FILE_GET_TYPE_EXTENT(MPI_Fint *fh, MPI_Fint *datatype, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_File_get_type_extent(fh, datatype, extent, ierr);
}

void MPI_FILE_SET_ATOMICITY(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_set_atomicity(fh, flag, ierr);
}

void MPI_FILE_GET_ATOMICITY(MPI_Fint *fh, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_File_get_atomicity(fh, flag, ierr);
}

void MPI_FINALIZED(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Finalized(flag, ierr);
}

void MPI_FREE_MEM(MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Free_mem(base, ierr);
}

void MPI_IGATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void MPI_IGATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Igatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm, request, ierr);
}

void MPI_GET_ADDRESS(MPI_Fint *location, MPI_Fint *address, MPI_Fint *ierr)
{
	FMPI_Get_address(location, address, ierr);
}

void MPI_GET_COUNT(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_count(status, datatype, count, ierr);
}

void MPI_GET_ELEMENTS(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements(status, datatype, count, ierr);
}

void MPI_GET_ELEMENTS_X(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Get_elements_x(status, datatype, count, ierr);
}

void MPI_GET(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void MPI_GET_ACCUMULATE(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, ierr);
}

void MPI_GET_LIBRARY_VERSION(char *version, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint version_len)
{
	FMPI_Get_library_version(version, resultlen, ierr, version_len);
}

void MPI_GET_PROCESSOR_NAME(char *name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint name_len)
{
	FMPI_Get_processor_name(name, resultlen, ierr, name_len);
}

void MPI_GET_VERSION(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
	FMPI_Get_version(version, subversion, ierr);
}

void MPI_GRAPH_CREATE(MPI_Fint *comm_old, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *comm_graph, MPI_Fint *ierr)
{
	FMPI_Graph_create(comm_old, nnodes, index, edges, reorder, comm_graph, ierr);
}

void MPI_GRAPH_GET(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
	FMPI_Graph_get(comm, maxindex, maxedges, index, edges, ierr);
}

void MPI_GRAPH_MAP(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
	FMPI_Graph_map(comm, nnodes, index, edges, newrank, ierr);
}

void MPI_GRAPH_NEIGHBORS_COUNT(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors_count(comm, rank, nneighbors, ierr);
}

void MPI_GRAPH_NEIGHBORS(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
	FMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors, ierr);
}

void MPI_GRAPHDIMS_GET(MPI_Fint *comm, MPI_Fint *nnodes, MPI_Fint *nedges, MPI_Fint *ierr)
{
	FMPI_Graphdims_get(comm, nnodes, nedges, ierr);
}

void MPI_GREQUEST_COMPLETE(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_complete(request, ierr);
}

void MPI_GREQUEST_START(MPI_Fint *query_fn, MPI_Fint *free_fn, MPI_Fint *cancel_fn, MPI_Fint *extra_state, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Grequest_start(query_fn, free_fn, cancel_fn, extra_state, request, ierr);
}

void MPI_GROUP_COMPARE(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
	FMPI_Group_compare(group1, group2, result, ierr);
}

void MPI_GROUP_DIFFERENCE(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_difference(group1, group2, newgroup, ierr);
}

void MPI_GROUP_EXCL(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_excl(group, n, ranks, newgroup, ierr);
}

void MPI_GROUP_FREE(MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Group_free(group, ierr);
}

void MPI_GROUP_INCL(MPI_Fint *group, MPI_Fint *n, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_incl(group, n, ranks, newgroup, ierr);
}

void MPI_GROUP_INTERSECTION(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_intersection(group1, group2, newgroup, ierr);
}

void MPI_GROUP_RANGE_EXCL(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_excl(group, n, ranges, newgroup, ierr);
}

void MPI_GROUP_RANGE_INCL(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_range_incl(group, n, ranges, newgroup, ierr);
}

void MPI_GROUP_RANK(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
	FMPI_Group_rank(group, rank, ierr);
}

void MPI_GROUP_SIZE(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Group_size(group, size, ierr);
}

void MPI_GROUP_TRANSLATE_RANKS(MPI_Fint *group1, MPI_Fint *n, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
	FMPI_Group_translate_ranks(group1, n, ranks1, group2, ranks2, ierr);
}

void MPI_GROUP_UNION(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
	FMPI_Group_union(group1, group2, newgroup, ierr);
}

void MPI_IMPROBE(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Improbe(source, tag, comm, flag, message, status, ierr);
}

void MPI_IMRECV(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Imrecv(buf, count, type, message, request, ierr);
}

void MPI_INFO_CREATE(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_create(info, ierr);
}

void MPI_INFO_DELETE(MPI_Fint *info, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_delete(info, key, ierr, key_len);
}

void MPI_INFO_DUP(MPI_Fint *info, MPI_Fint *newinfo, MPI_Fint *ierr)
{
	FMPI_Info_dup(info, newinfo, ierr);
}

void MPI_INFO_FREE(MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Info_free(info, ierr);
}

void MPI_INFO_GET(MPI_Fint *info, char *key, MPI_Fint *valuelen, char *value, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_get(info, key, valuelen, value, flag, ierr, key_len, value_len);
}

void MPI_INFO_GET_NKEYS(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
	FMPI_Info_get_nkeys(info, nkeys, ierr);
}

void MPI_INFO_GET_NTHKEY(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_nthkey(info, n, key, ierr, key_len);
}

void MPI_INFO_GET_VALUELEN(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr, MPI_Fint key_len)
{
	FMPI_Info_get_valuelen(info, key, valuelen, flag, ierr, key_len);
}

void MPI_INFO_SET(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr, MPI_Fint key_len, MPI_Fint value_len)
{
	FMPI_Info_set(info, key, value, ierr, key_len, value_len);
}

void MPI_INITIALIZED(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Initialized(flag, ierr);
}

void MPI_INTERCOMM_CREATE(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_create(local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr);
}

void MPI_INTERCOMM_MERGE(MPI_Fint *intercomm, MPI_Fint *high, MPI_Fint *newintercomm, MPI_Fint *ierr)
{
	FMPI_Intercomm_merge(intercomm, high, newintercomm, ierr);
}

void MPI_IS_THREAD_MAIN(MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Is_thread_main(flag, ierr);
}

void MPI_LOOKUP_NAME(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Lookup_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void MPI_MPROBE(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mprobe(source, tag, comm, message, status, ierr);
}

void MPI_MRECV(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *type, MPI_Fint *message, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Mrecv(buf, count, type, message, status, ierr);
}

void MPI_INEIGHBOR_ALLGATHER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void MPI_INEIGHBOR_ALLGATHERV(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm, request, ierr);
}

void MPI_INEIGHBOR_ALLTOALL(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, request, ierr);
}

void MPI_INEIGHBOR_ALLTOALLV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, recvtype, comm, request, ierr);
}

void MPI_INEIGHBOR_ALLTOALLW(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *sdispls, MPI_Fint *sendtypes, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *rdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ineighbor_alltoallw(sendbuf, sendcounts, sdispls, sendtypes, recvbuf, recvcounts, rdispls, recvtypes, comm, request, ierr);
}

void MPI_OP_COMMUTATIVE(MPI_Fint *op, MPI_Fint *commute, MPI_Fint *ierr)
{
	FMPI_Op_commutative(op, commute, ierr);
}

void MPI_OP_CREATE(MPI_Fint *function, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_create(function, commute, op, ierr);
}

void MPI_OPEN_PORT(MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint port_name_len)
{
	FMPI_Open_port(info, port_name, ierr, port_name_len);
}

void MPI_OP_FREE(MPI_Fint *op, MPI_Fint *ierr)
{
	FMPI_Op_free(op, ierr);
}

void MPI_PACK_EXTERNAL(char *datarep, MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external(datarep, inbuf, incount, datatype, outbuf, outsize, position, ierr, datarep_len);
}

void MPI_PACK_EXTERNAL_SIZE(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Pack_external_size(datarep, incount, datatype, size, ierr, datarep_len);
}

void MPI_PACK(MPI_Fint *inbuf, MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *outbuf, MPI_Fint *outsize, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierr);
}

void MPI_PACK_SIZE(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Pack_size(incount, datatype, comm, size, ierr);
}

void MPI_PCONTROL(MPI_Fint *level, MPI_Fint *ierr)
{
	FMPI_Pcontrol(level, ierr);
}

void MPI_PUBLISH_NAME(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Publish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void MPI_PUT(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, ierr);
}

void MPI_QUERY_THREAD(MPI_Fint *provided, MPI_Fint *ierr)
{
	FMPI_Query_thread(provided, ierr);
}

void MPI_RACCUMULATE(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void MPI_RECV_INIT(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Recv_init(buf, count, datatype, source, tag, comm, request, ierr);
}

void MPI_IREDUCE(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request, ierr);
}

void MPI_IREDUCE_SCATTER(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request, ierr);
}

void MPI_REDUCE_SCATTER_BLOCK(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, ierr);
}

void MPI_IREDUCE_SCATTER_BLOCK(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request, ierr);
}

void MPI_REGISTER_DATAREP(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, MPI_Fint *extra_state, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Register_datarep(datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr, datarep_len);
}

void MPI_REQUEST_FREE(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Request_free(request, ierr);
}

void MPI_REQUEST_GET_STATUS(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Request_get_status(request, flag, status, ierr);
}

void MPI_RGET(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_count, target_datatype, win, request, ierr);
}

void MPI_RGET_ACCUMULATE(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *result_addr, MPI_Fint *result_count, MPI_Fint *result_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_count, MPI_Fint *target_datatype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count, result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, request, ierr);
}

void MPI_RPUT(MPI_Fint *origin_addr, MPI_Fint *origin_count, MPI_Fint *origin_datatype, MPI_Fint *target_rank, MPI_Fint *target_disp, MPI_Fint *target_cout, MPI_Fint *target_datatype, MPI_Fint *win, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp, target_cout, target_datatype, win, request, ierr);
}

void MPI_RSEND_INIT(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_ISCAN(MPI_Fint *sendbuf, MPI_Fint *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscan(sendbuf, recvbuf, count, datatype, op, comm, request, ierr);
}

void MPI_ISCATTER(MPI_Fint *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void MPI_ISCATTERV(MPI_Fint *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, MPI_Fint *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Iscatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm, request, ierr);
}

void MPI_SEND_INIT(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Send_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_SSEND_INIT(MPI_Fint *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request, ierr);
}

void MPI_START(MPI_Fint *request, MPI_Fint *ierr)
{
	FMPI_Start(request, ierr);
}

void MPI_STARTALL(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *ierr)
{
	FMPI_Startall(count, array_of_requests, ierr);
}

void MPI_STATUS_SET_CANCELLED(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Status_set_cancelled(status, flag, ierr);
}

void MPI_STATUS_SET_ELEMENTS(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements(status, datatype, count, ierr);
}

void MPI_STATUS_SET_ELEMENTS_X(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
	FMPI_Status_set_elements_x(status, datatype, count, ierr);
}

void MPI_TESTALL(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *flag, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testall(count, array_of_requests, flag, array_of_statuses, ierr);
}

void MPI_TESTANY(MPI_Fint *count, MPI_Fint *array_of_requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Testany(count, array_of_requests, index, flag, status, ierr);
}

void MPI_TEST(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Test(request, flag, status, ierr);
}

void MPI_TEST_CANCELLED(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Test_cancelled(status, flag, ierr);
}

void MPI_TESTSOME(MPI_Fint *incount, MPI_Fint *array_of_requests, MPI_Fint *outcount, MPI_Fint *array_of_indices, MPI_Fint *array_of_statuses, MPI_Fint *ierr)
{
	FMPI_Testsome(incount, array_of_requests, outcount, array_of_indices, array_of_statuses, ierr);
}

void MPI_TOPO_TEST(MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
	FMPI_Topo_test(comm, status, ierr);
}

void MPI_TYPE_COMMIT(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_commit(type, ierr);
}

void MPI_TYPE_CONTIGUOUS(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_contiguous(count, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_DARRAY(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsize_array, MPI_Fint *distrib_array, MPI_Fint *darg_array, MPI_Fint *psize_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_darray(size, rank, ndims, gsize_array, distrib_array, darg_array, psize_array, order, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_F90_COMPLEX(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_complex(p, r, newtype, ierr);
}

void MPI_TYPE_CREATE_F90_INTEGER(MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_integer(r, newtype, ierr);
}

void MPI_TYPE_CREATE_F90_REAL(MPI_Fint *p, MPI_Fint *r, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_f90_real(p, r, newtype, ierr);
}

void MPI_TYPE_CREATE_HINDEXED_BLOCK(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_HINDEXED(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hindexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_HVECTOR(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_KEYVAL(MPI_Fint *type_copy_attr_fn, MPI_Fint *type_delete_attr_fn, MPI_Fint *type_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Type_create_keyval(type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state, ierr);
}

void MPI_TYPE_CREATE_INDEXED_BLOCK(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_indexed_block(count, blocklength, array_of_displacements, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_STRUCT(MPI_Fint *count, MPI_Fint *array_of_block_lengths, MPI_Fint *array_of_displacements, MPI_Fint *array_of_types, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_struct(count, array_of_block_lengths, array_of_displacements, array_of_types, newtype, ierr);
}

void MPI_TYPE_CREATE_SUBARRAY(MPI_Fint *ndims, MPI_Fint *size_array, MPI_Fint *subsize_array, MPI_Fint *start_array, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_subarray(ndims, size_array, subsize_array, start_array, order, oldtype, newtype, ierr);
}

void MPI_TYPE_CREATE_RESIZED(MPI_Fint *oldtype, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_create_resized(oldtype, lb, extent, newtype, ierr);
}

void MPI_TYPE_DELETE_ATTR(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_delete_attr(type, type_keyval, ierr);
}

void MPI_TYPE_DUP(MPI_Fint *type, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_dup(type, newtype, ierr);
}

void MPI_TYPE_FREE(MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_free(type, ierr);
}

void MPI_TYPE_FREE_KEYVAL(MPI_Fint *type_keyval, MPI_Fint *ierr)
{
	FMPI_Type_free_keyval(type_keyval, ierr);
}

void MPI_TYPE_GET_ATTR(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Type_get_attr(type, type_keyval, attribute_val, flag, ierr);
}

void MPI_TYPE_GET_CONTENTS(MPI_Fint *mtype, MPI_Fint *max_integers, MPI_Fint *max_addresses, MPI_Fint *max_datatypes, MPI_Fint *array_of_integers, MPI_Fint *array_of_addresses, MPI_Fint *array_of_datatypes, MPI_Fint *ierr)
{
	FMPI_Type_get_contents(mtype, max_integers, max_addresses, max_datatypes, array_of_integers, array_of_addresses, array_of_datatypes, ierr);
}

void MPI_TYPE_GET_ENVELOPE(MPI_Fint *type, MPI_Fint *num_integers, MPI_Fint *num_addresses, MPI_Fint *num_datatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
	FMPI_Type_get_envelope(type, num_integers, num_addresses, num_datatypes, combiner, ierr);
}

void MPI_TYPE_GET_EXTENT(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent(type, lb, extent, ierr);
}

void MPI_TYPE_GET_EXTENT_X(MPI_Fint *type, MPI_Fint *lb, MPI_Fint *extent, MPI_Fint *ierr)
{
	FMPI_Type_get_extent_x(type, lb, extent, ierr);
}

void MPI_TYPE_GET_NAME(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_get_name(type, type_name, resultlen, ierr, type_name_len);
}

void MPI_TYPE_GET_TRUE_EXTENT(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent(datatype, true_lb, true_extent, ierr);
}

void MPI_TYPE_GET_TRUE_EXTENT_X(MPI_Fint *datatype, MPI_Fint *true_lb, MPI_Fint *true_extent, MPI_Fint *ierr)
{
	FMPI_Type_get_true_extent_x(datatype, true_lb, true_extent, ierr);
}

void MPI_TYPE_INDEXED(MPI_Fint *count, MPI_Fint *array_of_blocklengths, MPI_Fint *array_of_displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_indexed(count, array_of_blocklengths, array_of_displacements, oldtype, newtype, ierr);
}

void MPI_TYPE_MATCH_SIZE(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *type, MPI_Fint *ierr)
{
	FMPI_Type_match_size(typeclass, size, type, ierr);
}

void MPI_TYPE_SET_ATTR(MPI_Fint *type, MPI_Fint *type_keyval, MPI_Fint *attr_val, MPI_Fint *ierr)
{
	FMPI_Type_set_attr(type, type_keyval, attr_val, ierr);
}

void MPI_TYPE_SET_NAME(MPI_Fint *type, char *type_name, MPI_Fint *ierr, MPI_Fint type_name_len)
{
	FMPI_Type_set_name(type, type_name, ierr, type_name_len);
}

void MPI_TYPE_SIZE(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size(type, size, ierr);
}

void MPI_TYPE_SIZE_X(MPI_Fint *type, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Type_size_x(type, size, ierr);
}

void MPI_TYPE_VECTOR(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
	FMPI_Type_vector(count, blocklength, stride, oldtype, newtype, ierr);
}

void MPI_UNPACK(MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
	FMPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierr);
}

void MPI_UNPUBLISH_NAME(char *service_name, MPI_Fint *info, char *port_name, MPI_Fint *ierr, MPI_Fint service_name_len, MPI_Fint port_name_len)
{
	FMPI_Unpublish_name(service_name, info, port_name, ierr, service_name_len, port_name_len);
}

void MPI_UNPACK_EXTERNAL(char *datarep, MPI_Fint *inbuf, MPI_Fint *insize, MPI_Fint *position, MPI_Fint *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr, MPI_Fint datarep_len)
{
	FMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype, ierr, datarep_len);
}

void MPI_WIN_ALLOCATE(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win, ierr);
}

void MPI_WIN_ALLOCATE_SHARED(MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *baseptr, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win, ierr);
}

void MPI_WIN_ATTACH(MPI_Fint *win, MPI_Fint *base, MPI_Fint *size, MPI_Fint *ierr)
{
	FMPI_Win_attach(win, base, size, ierr);
}

void MPI_WIN_CALL_ERRHANDLER(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
	FMPI_Win_call_errhandler(win, errorcode, ierr);
}

void MPI_WIN_COMPLETE(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_complete(win, ierr);
}

void MPI_WIN_CREATE(MPI_Fint *base, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create(base, size, disp_unit, info, comm, win, ierr);
}

void MPI_WIN_CREATE_DYNAMIC(MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_create_dynamic(info, comm, win, ierr);
}

void MPI_WIN_CREATE_ERRHANDLER(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_create_errhandler(function, errhandler, ierr);
}

void MPI_WIN_CREATE_KEYVAL(MPI_Fint *win_copy_attr_fn, MPI_Fint *win_delete_attr_fn, MPI_Fint *win_keyval, MPI_Fint *extra_state, MPI_Fint *ierr)
{
	FMPI_Win_create_keyval(win_copy_attr_fn, win_delete_attr_fn, win_keyval, extra_state, ierr);
}

void MPI_WIN_DELETE_ATTR(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_delete_attr(win, win_keyval, ierr);
}

void MPI_WIN_DETACH(MPI_Fint *win, MPI_Fint *base, MPI_Fint *ierr)
{
	FMPI_Win_detach(win, base, ierr);
}

void MPI_WIN_FENCE(MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_fence(assert, win, ierr);
}

void MPI_WIN_FREE(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_free(win, ierr);
}

void MPI_WIN_FREE_KEYVAL(MPI_Fint *win_keyval, MPI_Fint *ierr)
{
	FMPI_Win_free_keyval(win_keyval, ierr);
}

void MPI_WIN_GET_ATTR(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_get_attr(win, win_keyval, attribute_val, flag, ierr);
}

void MPI_WIN_GET_ERRHANDLER(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_get_errhandler(win, errhandler, ierr);
}

void MPI_WIN_GET_GROUP(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
	FMPI_Win_get_group(win, group, ierr);
}

void MPI_WIN_GET_INFO(MPI_Fint *win, MPI_Fint *info_used, MPI_Fint *ierr)
{
	FMPI_Win_get_info(win, info_used, ierr);
}

void MPI_WIN_GET_NAME(MPI_Fint *win, char *win_name, MPI_Fint *resultlen, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_get_name(win, win_name, resultlen, ierr, win_name_len);
}

void MPI_WIN_POST(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_post(group, assert, win, ierr);
}

void MPI_WIN_SET_ATTR(MPI_Fint *win, MPI_Fint *win_keyval, MPI_Fint *attribute_val, MPI_Fint *ierr)
{
	FMPI_Win_set_attr(win, win_keyval, attribute_val, ierr);
}

void MPI_WIN_SET_ERRHANDLER(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
	FMPI_Win_set_errhandler(win, errhandler, ierr);
}

void MPI_WIN_SET_INFO(MPI_Fint *win, MPI_Fint *info, MPI_Fint *ierr)
{
	FMPI_Win_set_info(win, info, ierr);
}

void MPI_WIN_SET_NAME(MPI_Fint *win, char *win_name, MPI_Fint *ierr, MPI_Fint win_name_len)
{
	FMPI_Win_set_name(win, win_name, ierr, win_name_len);
}

void MPI_WIN_SHARED_QUERY(MPI_Fint *win, MPI_Fint *rank, MPI_Fint *size, MPI_Fint *disp_unit, MPI_Fint *baseptr, MPI_Fint *ierr)
{
	FMPI_Win_shared_query(win, rank, size, disp_unit, baseptr, ierr);
}

void MPI_WIN_START(MPI_Fint *group, MPI_Fint *assert, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_start(group, assert, win, ierr);
}

void MPI_WIN_TEST(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
	FMPI_Win_test(win, flag, ierr);
}

void MPI_WIN_UNLOCK(MPI_Fint *rank, MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock(rank, win, ierr);
}

void MPI_WIN_UNLOCK_ALL(MPI_Fint *win, MPI_Fint *ierr)
{
	FMPI_Win_unlock_all(win, ierr);
}

#endif // DISABLE_ACCESSORY_MPI

#endif // DISABLE_PROFILING_MPI

#endif // OMPI_MPI_H
