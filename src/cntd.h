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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>
#include <mpi.h>

#include "cpuid.h"
#include "memhdlr.h"
#include "msr_core.h"
#include "msr_rapl.h"
#include "msr_thermal.h"
#include "msr_counters.h"
#include "msr_clocks.h"
#include "msr_misc.h"
#include "msr_turbo.h"
#include "csr_core.h"
#include "csr_imc.h"
#include "libmsr_error.h"


#ifndef _CNTD_H_
#define	_CNTD_H_

#ifdef __INTEL_COMPILER
#pragma warning disable 1786
#else   /// GNU build
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif


// Default timing
#define DEFAULT_EAM_TIMEOUT 500		// microseconds
#define DEFAULT_SAMPLING_TIME 1		// seconds

#define CLOCK_TYPE CLOCK_REALTIME
//#define CLOCK_TYPE CLOCK_MONOTONIC

//#define READ_TSC RDTSC
#define READ_TSC RDTSCP

#define BUFF_SIZE_LOG_CALL 1048576

#define MEM_SIZE 128
#define STRING_SIZE 128


// Constants
#define MAX_48_BIT_UNSIGN 281474976710655

#define FALSE 0
#define TRUE 1

#define CURR 0
#define PREV 1

#define START 0
#define END 1
#define DURATION 2

#define APP 0
#define MPI 1
#define TOT 2
#define GT_TIMEOUT 3

#define SEND 0
#define RECV 1

#define READ 0
#define WRITE 1

#define MPI_TYPE_TOT 0
#define MPI_TYPE_EAM 1

#define MPI_NONE -1
#define MPI_ALL -2
#define MPI_ALLV -3
#define MPI_ALLW -4

#define ROOT_MPI 0
#define CNTD_GROUP_WORLD_IDX 0
#define CNTD_COMM_WORLD_IDX 0

// Batch
#define CPU_READ_BATCH USR_BATCH0
#define SOCKET_READ_BATCH USR_BATCH1
#define ENABLE_UNC_FREQ_BATCH USR_BATCH2
#define DISABLE_UNC_FREQ_BATCH USR_BATCH3

// Uncore frequency stuffs
#define SMSR_PLATFORM_INFO          (0xCE)
#define MSR_U_PMON_UCLK_FIXED_CTL	(0x703)
#define MSR_U_PMON_UCLK_FIXED_CTR	(0x704)
#define CONF_UNCORE_FREQ_ENABLE		(0x400000)


// Enumerator
#define FOREACH_MPI(MPI) \
	MPI(ENUM_MPI_ABORT) \
	MPI(ENUM_MPI_ACCUMULATE) \
	MPI(ENUM_MPI_ADD_ERROR_CLASS) \
	MPI(ENUM_MPI_ADD_ERROR_CODE) \
	MPI(ENUM_MPI_ADD_ERROR_STRING) \
	MPI(ENUM_MPI_ADDRESS) \
	MPI(ENUM_MPI_ALLGATHER) \
	MPI(ENUM_MPI_IALLGATHER) \
	MPI(ENUM_MPI_ALLGATHERV) \
	MPI(ENUM_MPI_IALLGATHERV) \
	MPI(ENUM_MPI_ALLOC_MEM) \
	MPI(ENUM_MPI_ALLREDUCE) \
	MPI(ENUM_MPI_IALLREDUCE) \
	MPI(ENUM_MPI_ALLTOALL) \
	MPI(ENUM_MPI_IALLTOALL) \
	MPI(ENUM_MPI_ALLTOALLV) \
	MPI(ENUM_MPI_IALLTOALLV) \
	MPI(ENUM_MPI_ALLTOALLW) \
	MPI(ENUM_MPI_IALLTOALLW) \
	MPI(ENUM_MPI_ATTR_DELETE) \
	MPI(ENUM_MPI_ATTR_GET) \
	MPI(ENUM_MPI_ATTR_PUT) \
	MPI(ENUM_MPI_BARRIER) \
	MPI(ENUM_MPI_IBARRIER) \
	MPI(ENUM_MPI_BCAST) \
	MPI(ENUM_MPI_BSEND) \
	MPI(ENUM_MPI_IBCAST) \
	MPI(ENUM_MPI_BSEND_INIT) \
	MPI(ENUM_MPI_BUFFER_ATTACH) \
	MPI(ENUM_MPI_BUFFER_DETACH) \
	MPI(ENUM_MPI_CANCEL) \
	MPI(ENUM_MPI_CART_COORDS) \
	MPI(ENUM_MPI_CART_CREATE) \
	MPI(ENUM_MPI_CART_GET) \
	MPI(ENUM_MPI_CART_MAP) \
	MPI(ENUM_MPI_CART_RANK) \
	MPI(ENUM_MPI_CART_SHIFT) \
	MPI(ENUM_MPI_CART_SUB) \
	MPI(ENUM_MPI_CARTDIM_GET) \
	MPI(ENUM_MPI_CLOSE_PORT) \
	MPI(ENUM_MPI_COMM_ACCEPT) \
	MPI(ENUM_MPI_FMPI_COMM_C2F) \
	MPI(ENUM_MPI_COMM_CALL_ERRHANDLER) \
	MPI(ENUM_MPI_COMM_COMPARE) \
	MPI(ENUM_MPI_COMM_CONNECT) \
	MPI(ENUM_MPI_COMM_CREATE_ERRHANDLER) \
	MPI(ENUM_MPI_COMM_CREATE_KEYVAL) \
	MPI(ENUM_MPI_COMM_CREATE_GROUP) \
	MPI(ENUM_MPI_COMM_CREATE) \
	MPI(ENUM_MPI_COMM_DELETE_ATTR) \
	MPI(ENUM_MPI_COMM_DISCONNECT) \
	MPI(ENUM_MPI_COMM_DUP) \
	MPI(ENUM_MPI_COMM_IDUP) \
	MPI(ENUM_MPI_COMM_DUP_WITH_INFO) \
	MPI(ENUM_MPI_COMM_F2C) \
	MPI(ENUM_MPI_COMM_FREE_KEYVAL) \
	MPI(ENUM_MPI_COMM_FREE) \
	MPI(ENUM_MPI_COMM_GET_ATTR) \
	MPI(ENUM_MPI_DIST_GRAPH_CREATE) \
	MPI(ENUM_MPI_DIST_GRAPH_CREATE_ADJACENT) \
	MPI(ENUM_MPI_DIST_GRAPH_NEIGHBORS) \
	MPI(ENUM_MPI_DIST_GRAPH_NEIGHBORS_COUNT) \
	MPI(ENUM_MPI_COMM_GET_ERRHANDLER) \
	MPI(ENUM_MPI_COMM_GET_INFO) \
	MPI(ENUM_MPI_COMM_GET_NAME) \
	MPI(ENUM_MPI_COMM_GET_PARENT) \
	MPI(ENUM_MPI_COMM_GROUP) \
	MPI(ENUM_MPI_COMM_JOIN) \
	MPI(ENUM_MPI_COMM_RANK) \
	MPI(ENUM_MPI_COMM_REMOTE_GROUP) \
	MPI(ENUM_MPI_COMM_REMOTE_SIZE) \
	MPI(ENUM_MPI_COMM_SET_ATTR) \
	MPI(ENUM_MPI_COMM_SET_ERRHANDLER) \
	MPI(ENUM_MPI_COMM_SET_INFO) \
	MPI(ENUM_MPI_COMM_SET_NAME) \
	MPI(ENUM_MPI_COMM_SIZE) \
	MPI(ENUM_MPI_COMM_SPAWN) \
	MPI(ENUM_MPI_COMM_SPAWN_MULTIPLE) \
	MPI(ENUM_MPI_COMM_SPLIT) \
	MPI(ENUM_MPI_COMM_SPLIT_TYPE) \
	MPI(ENUM_MPI_COMM_TEST_INTER) \
	MPI(ENUM_MPI_COMPARE_AND_SWAP) \
	MPI(ENUM_MPI_DIMS_CREATE) \
	MPI(ENUM_MPI_FMPI_ERRHANDLER_C2F) \
	MPI(ENUM_MPI_ERRHANDLER_CREATE) \
	MPI(ENUM_MPI_ERRHANDLER_F2C) \
	MPI(ENUM_MPI_ERRHANDLER_FREE) \
	MPI(ENUM_MPI_ERRHANDLER_GET) \
	MPI(ENUM_MPI_ERRHANDLER_SET) \
	MPI(ENUM_MPI_ERROR_CLASS) \
	MPI(ENUM_MPI_ERROR_STRING) \
	MPI(ENUM_MPI_EXSCAN) \
	MPI(ENUM_MPI_FETCH_AND_OP) \
	MPI(ENUM_MPI_IEXSCAN) \
	MPI(ENUM_MPI_FILE_C2F) \
	MPI(ENUM_MPI_FILE_F2C) \
	MPI(ENUM_MPI_FILE_CALL_ERRHANDLER) \
	MPI(ENUM_MPI_FILE_CREATE_ERRHANDLER) \
	MPI(ENUM_MPI_FILE_SET_ERRHANDLER) \
	MPI(ENUM_MPI_FILE_GET_ERRHANDLER) \
	MPI(ENUM_MPI_FILE_OPEN) \
	MPI(ENUM_MPI_FILE_CLOSE) \
	MPI(ENUM_MPI_FILE_DELETE) \
	MPI(ENUM_MPI_FILE_SET_SIZE) \
	MPI(ENUM_MPI_FILE_PREALLOCATE) \
	MPI(ENUM_MPI_FILE_GET_SIZE) \
	MPI(ENUM_MPI_FILE_GET_GROUP) \
	MPI(ENUM_MPI_FILE_GET_AMODE) \
	MPI(ENUM_MPI_FILE_SET_INFO) \
	MPI(ENUM_MPI_FILE_GET_INFO) \
	MPI(ENUM_MPI_FILE_SET_VIEW) \
	MPI(ENUM_MPI_FILE_GET_VIEW) \
	MPI(ENUM_MPI_FILE_READ_AT) \
	MPI(ENUM_MPI_FILE_READ_AT_ALL) \
	MPI(ENUM_MPI_FILE_WRITE_AT) \
	MPI(ENUM_MPI_FILE_WRITE_AT_ALL) \
	MPI(ENUM_MPI_FILE_IREAD_AT) \
	MPI(ENUM_MPI_FILE_IWRITE_AT) \
	MPI(ENUM_MPI_FILE_IREAD_AT_ALL) \
	MPI(ENUM_MPI_FILE_IWRITE_AT_ALL) \
	MPI(ENUM_MPI_FILE_READ) \
	MPI(ENUM_MPI_FILE_READ_ALL) \
	MPI(ENUM_MPI_FILE_WRITE) \
	MPI(ENUM_MPI_FILE_WRITE_ALL) \
	MPI(ENUM_MPI_FILE_IREAD) \
	MPI(ENUM_MPI_FILE_IWRITE) \
	MPI(ENUM_MPI_FILE_IREAD_ALL) \
	MPI(ENUM_MPI_FILE_IWRITE_ALL) \
	MPI(ENUM_MPI_FILE_SEEK) \
	MPI(ENUM_MPI_FILE_GET_POSITION) \
	MPI(ENUM_MPI_FILE_GET_BYTE_OFFSET) \
	MPI(ENUM_MPI_FILE_READ_SHARED) \
	MPI(ENUM_MPI_FILE_WRITE_SHARED) \
	MPI(ENUM_MPI_FILE_IREAD_SHARED) \
	MPI(ENUM_MPI_FILE_IWRITE_SHARED) \
	MPI(ENUM_MPI_FILE_READ_ORDERED) \
	MPI(ENUM_MPI_FILE_WRITE_ORDERED) \
	MPI(ENUM_MPI_FILE_SEEK_SHARED) \
	MPI(ENUM_MPI_FILE_GET_POSITION_SHARED) \
	MPI(ENUM_MPI_FILE_READ_AT_ALL_BEGIN) \
	MPI(ENUM_MPI_FILE_READ_AT_ALL_END) \
	MPI(ENUM_MPI_FILE_WRITE_AT_ALL_BEGIN) \
	MPI(ENUM_MPI_FILE_WRITE_AT_ALL_END) \
	MPI(ENUM_MPI_FILE_READ_ALL_BEGIN) \
	MPI(ENUM_MPI_FILE_READ_ALL_END) \
	MPI(ENUM_MPI_FILE_WRITE_ALL_BEGIN) \
	MPI(ENUM_MPI_FILE_WRITE_ALL_END) \
	MPI(ENUM_MPI_FILE_READ_ORDERED_BEGIN) \
	MPI(ENUM_MPI_FILE_READ_ORDERED_END) \
	MPI(ENUM_MPI_FILE_WRITE_ORDERED_BEGIN) \
	MPI(ENUM_MPI_FILE_WRITE_ORDERED_END) \
	MPI(ENUM_MPI_FILE_GET_TYPE_EXTENT) \
	MPI(ENUM_MPI_FILE_SET_ATOMICITY) \
	MPI(ENUM_MPI_FILE_GET_ATOMICITY) \
	MPI(ENUM_MPI_FILE_SYNC) \
	MPI(ENUM_MPI_FINALIZE) \
	MPI(ENUM_MPI_FINALIZED) \
	MPI(ENUM_MPI_FREE_MEM) \
	MPI(ENUM_MPI_GATHER) \
	MPI(ENUM_MPI_IGATHER) \
	MPI(ENUM_MPI_GATHERV) \
	MPI(ENUM_MPI_IGATHERV) \
	MPI(ENUM_MPI_GET_ADDRESS) \
	MPI(ENUM_MPI_GET_COUNT) \
	MPI(ENUM_MPI_GET_ELEMENTS) \
	MPI(ENUM_MPI_GET_ELEMENTS_X) \
	MPI(ENUM_MPI_GET) \
	MPI(ENUM_MPI_GET_ACCUMULATE) \
	MPI(ENUM_MPI_GET_LIBRARY_VERSION) \
	MPI(ENUM_MPI_GET_PROCESSOR_NAME) \
	MPI(ENUM_MPI_GET_VERSION) \
	MPI(ENUM_MPI_GRAPH_CREATE) \
	MPI(ENUM_MPI_GRAPH_GET) \
	MPI(ENUM_MPI_GRAPH_MAP) \
	MPI(ENUM_MPI_GRAPH_NEIGHBORS_COUNT) \
	MPI(ENUM_MPI_GRAPH_NEIGHBORS) \
	MPI(ENUM_MPI_GRAPHDIMS_GET) \
	MPI(ENUM_MPI_GREQUEST_COMPLETE) \
	MPI(ENUM_MPI_GREQUEST_START) \
	MPI(ENUM_MPI_FMPI_GROUP_C2F) \
	MPI(ENUM_MPI_GROUP_COMPARE) \
	MPI(ENUM_MPI_GROUP_DIFFERENCE) \
	MPI(ENUM_MPI_GROUP_EXCL) \
	MPI(ENUM_MPI_GROUP_F2C) \
	MPI(ENUM_MPI_GROUP_FREE) \
	MPI(ENUM_MPI_GROUP_INCL) \
	MPI(ENUM_MPI_GROUP_INTERSECTION) \
	MPI(ENUM_MPI_GROUP_RANGE_EXCL) \
	MPI(ENUM_MPI_GROUP_RANGE_INCL) \
	MPI(ENUM_MPI_GROUP_RANK) \
	MPI(ENUM_MPI_GROUP_SIZE) \
	MPI(ENUM_MPI_GROUP_TRANSLATE_RANKS) \
	MPI(ENUM_MPI_GROUP_UNION) \
	MPI(ENUM_MPI_IBSEND) \
	MPI(ENUM_MPI_IMPROBE) \
	MPI(ENUM_MPI_IMRECV) \
	MPI(ENUM_MPI_FMPI_INFO_C2F) \
	MPI(ENUM_MPI_INFO_CREATE) \
	MPI(ENUM_MPI_INFO_DELETE) \
	MPI(ENUM_MPI_INFO_DUP) \
	MPI(ENUM_MPI_INFO_F2C) \
	MPI(ENUM_MPI_INFO_FREE) \
	MPI(ENUM_MPI_INFO_GET) \
	MPI(ENUM_MPI_INFO_GET_NKEYS) \
	MPI(ENUM_MPI_INFO_GET_NTHKEY) \
	MPI(ENUM_MPI_INFO_GET_VALUELEN) \
	MPI(ENUM_MPI_INFO_SET) \
	MPI(ENUM_MPI_INIT) \
	MPI(ENUM_MPI_INITIALIZED) \
	MPI(ENUM_MPI_INIT_THREAD) \
	MPI(ENUM_MPI_INTERCOMM_CREATE) \
	MPI(ENUM_MPI_INTERCOMM_MERGE) \
	MPI(ENUM_MPI_IPROBE) \
	MPI(ENUM_MPI_IRECV) \
	MPI(ENUM_MPI_IRSEND) \
	MPI(ENUM_MPI_ISEND) \
	MPI(ENUM_MPI_ISSEND) \
	MPI(ENUM_MPI_IS_THREAD_MAIN) \
	MPI(ENUM_MPI_KEYVAL_CREATE) \
	MPI(ENUM_MPI_KEYVAL_FREE) \
	MPI(ENUM_MPI_LOOKUP_NAME) \
	MPI(ENUM_MPI_FMPI_MESSAGE_C2F) \
	MPI(ENUM_MPI_MESSAGE_F2C) \
	MPI(ENUM_MPI_MPROBE) \
	MPI(ENUM_MPI_MRECV) \
	MPI(ENUM_MPI_NEIGHBOR_ALLGATHER) \
	MPI(ENUM_MPI_INEIGHBOR_ALLGATHER) \
	MPI(ENUM_MPI_NEIGHBOR_ALLGATHERV) \
	MPI(ENUM_MPI_INEIGHBOR_ALLGATHERV) \
	MPI(ENUM_MPI_NEIGHBOR_ALLTOALL) \
	MPI(ENUM_MPI_INEIGHBOR_ALLTOALL) \
	MPI(ENUM_MPI_NEIGHBOR_ALLTOALLV) \
	MPI(ENUM_MPI_INEIGHBOR_ALLTOALLV) \
	MPI(ENUM_MPI_NEIGHBOR_ALLTOALLW) \
	MPI(ENUM_MPI_INEIGHBOR_ALLTOALLW) \
	MPI(ENUM_MPI_FMPI_OP_C2F) \
	MPI(ENUM_MPI_OP_COMMUTATIVE) \
	MPI(ENUM_MPI_OP_CREATE) \
	MPI(ENUM_MPI_OPEN_PORT) \
	MPI(ENUM_MPI_OP_F2C) \
	MPI(ENUM_MPI_OP_FREE) \
	MPI(ENUM_MPI_PACK_EXTERNAL) \
	MPI(ENUM_MPI_PACK_EXTERNAL_SIZE) \
	MPI(ENUM_MPI_PACK) \
	MPI(ENUM_MPI_PACK_SIZE) \
	MPI(ENUM_MPI_PCONTROL) \
	MPI(ENUM_MPI_PROBE) \
	MPI(ENUM_MPI_PUBLISH_NAME) \
	MPI(ENUM_MPI_PUT) \
	MPI(ENUM_MPI_QUERY_THREAD) \
	MPI(ENUM_MPI_RACCUMULATE) \
	MPI(ENUM_MPI_RECV_INIT) \
	MPI(ENUM_MPI_RECV) \
	MPI(ENUM_MPI_REDUCE) \
	MPI(ENUM_MPI_IREDUCE) \
	MPI(ENUM_MPI_REDUCE_LOCAL) \
	MPI(ENUM_MPI_REDUCE_SCATTER) \
	MPI(ENUM_MPI_IREDUCE_SCATTER) \
	MPI(ENUM_MPI_REDUCE_SCATTER_BLOCK) \
	MPI(ENUM_MPI_IREDUCE_SCATTER_BLOCK) \
	MPI(ENUM_MPI_REGISTER_DATAREP) \
	MPI(ENUM_MPI_FMPI_REQUEST_C2F) \
	MPI(ENUM_MPI_REQUEST_F2C) \
	MPI(ENUM_MPI_REQUEST_FREE) \
	MPI(ENUM_MPI_REQUEST_GET_STATUS) \
	MPI(ENUM_MPI_RGET) \
	MPI(ENUM_MPI_RGET_ACCUMULATE) \
	MPI(ENUM_MPI_RPUT) \
	MPI(ENUM_MPI_RSEND) \
	MPI(ENUM_MPI_RSEND_INIT) \
	MPI(ENUM_MPI_SCAN) \
	MPI(ENUM_MPI_ISCAN) \
	MPI(ENUM_MPI_SCATTER) \
	MPI(ENUM_MPI_ISCATTER) \
	MPI(ENUM_MPI_SCATTERV) \
	MPI(ENUM_MPI_ISCATTERV) \
	MPI(ENUM_MPI_SEND_INIT) \
	MPI(ENUM_MPI_SEND) \
	MPI(ENUM_MPI_SENDRECV) \
	MPI(ENUM_MPI_SENDRECV_REPLACE) \
	MPI(ENUM_MPI_SSEND_INIT) \
	MPI(ENUM_MPI_SSEND) \
	MPI(ENUM_MPI_START) \
	MPI(ENUM_MPI_STARTALL) \
	MPI(ENUM_MPI_STATUS_C2F) \
	MPI(ENUM_MPI_STATUS_F2C) \
	MPI(ENUM_MPI_STATUS_SET_CANCELLED) \
	MPI(ENUM_MPI_STATUS_SET_ELEMENTS) \
	MPI(ENUM_MPI_STATUS_SET_ELEMENTS_X) \
	MPI(ENUM_MPI_TESTALL) \
	MPI(ENUM_MPI_TESTANY) \
	MPI(ENUM_MPI_TEST) \
	MPI(ENUM_MPI_TEST_CANCELLED) \
	MPI(ENUM_MPI_TESTSOME) \
	MPI(ENUM_MPI_TOPO_TEST) \
	MPI(ENUM_MPI_FMPI_TYPE_C2F) \
	MPI(ENUM_MPI_TYPE_COMMIT) \
	MPI(ENUM_MPI_TYPE_CONTIGUOUS) \
	MPI(ENUM_MPI_TYPE_CREATE_DARRAY) \
	MPI(ENUM_MPI_TYPE_CREATE_F90_COMPLEX) \
	MPI(ENUM_MPI_TYPE_CREATE_F90_INTEGER) \
	MPI(ENUM_MPI_TYPE_CREATE_F90_REAL) \
	MPI(ENUM_MPI_TYPE_CREATE_HINDEXED_BLOCK) \
	MPI(ENUM_MPI_TYPE_CREATE_HINDEXED) \
	MPI(ENUM_MPI_TYPE_CREATE_HVECTOR) \
	MPI(ENUM_MPI_TYPE_CREATE_KEYVAL) \
	MPI(ENUM_MPI_TYPE_CREATE_INDEXED_BLOCK) \
	MPI(ENUM_MPI_TYPE_CREATE_STRUCT) \
	MPI(ENUM_MPI_TYPE_CREATE_SUBARRAY) \
	MPI(ENUM_MPI_TYPE_CREATE_RESIZED) \
	MPI(ENUM_MPI_TYPE_DELETE_ATTR) \
	MPI(ENUM_MPI_TYPE_DUP) \
	MPI(ENUM_MPI_TYPE_EXTENT) \
	MPI(ENUM_MPI_TYPE_FREE) \
	MPI(ENUM_MPI_TYPE_FREE_KEYVAL) \
	MPI(ENUM_MPI_TYPE_F2C) \
	MPI(ENUM_MPI_TYPE_GET_ATTR) \
	MPI(ENUM_MPI_TYPE_GET_CONTENTS) \
	MPI(ENUM_MPI_TYPE_GET_ENVELOPE) \
	MPI(ENUM_MPI_TYPE_GET_EXTENT) \
	MPI(ENUM_MPI_TYPE_GET_EXTENT_X) \
	MPI(ENUM_MPI_TYPE_GET_NAME) \
	MPI(ENUM_MPI_TYPE_GET_TRUE_EXTENT) \
	MPI(ENUM_MPI_TYPE_GET_TRUE_EXTENT_X) \
	MPI(ENUM_MPI_TYPE_HINDEXED) \
	MPI(ENUM_MPI_TYPE_HVECTOR) \
	MPI(ENUM_MPI_TYPE_INDEXED) \
	MPI(ENUM_MPI_TYPE_LB) \
	MPI(ENUM_MPI_TYPE_MATCH_SIZE) \
	MPI(ENUM_MPI_TYPE_SET_ATTR) \
	MPI(ENUM_MPI_TYPE_SET_NAME) \
	MPI(ENUM_MPI_TYPE_SIZE) \
	MPI(ENUM_MPI_TYPE_SIZE_X) \
	MPI(ENUM_MPI_TYPE_STRUCT) \
	MPI(ENUM_MPI_TYPE_UB) \
	MPI(ENUM_MPI_TYPE_VECTOR) \
	MPI(ENUM_MPI_UNPACK) \
	MPI(ENUM_MPI_UNPUBLISH_NAME) \
	MPI(ENUM_MPI_UNPACK_EXTERNAL ) \
	MPI(ENUM_MPI_WAITALL) \
	MPI(ENUM_MPI_WAITANY) \
	MPI(ENUM_MPI_WAIT) \
	MPI(ENUM_MPI_WAITSOME) \
	MPI(ENUM_MPI_WIN_ALLOCATE) \
	MPI(ENUM_MPI_WIN_ALLOCATE_SHARED) \
	MPI(ENUM_MPI_WIN_ATTACH) \
	MPI(ENUM_MPI_FMPI_WIN_C2F) \
	MPI(ENUM_MPI_WIN_CALL_ERRHANDLER) \
	MPI(ENUM_MPI_WIN_COMPLETE) \
	MPI(ENUM_MPI_WIN_CREATE) \
	MPI(ENUM_MPI_WIN_CREATE_DYNAMIC) \
	MPI(ENUM_MPI_WIN_CREATE_ERRHANDLER) \
	MPI(ENUM_MPI_WIN_CREATE_KEYVAL) \
	MPI(ENUM_MPI_WIN_DELETE_ATTR) \
	MPI(ENUM_MPI_WIN_DETACH) \
	MPI(ENUM_MPI_WIN_F2C) \
	MPI(ENUM_MPI_WIN_FENCE) \
	MPI(ENUM_MPI_WIN_FLUSH) \
	MPI(ENUM_MPI_WIN_FLUSH_ALL) \
	MPI(ENUM_MPI_WIN_FLUSH_LOCAL) \
	MPI(ENUM_MPI_WIN_FLUSH_LOCAL_ALL) \
	MPI(ENUM_MPI_WIN_FREE) \
	MPI(ENUM_MPI_WIN_FREE_KEYVAL) \
	MPI(ENUM_MPI_WIN_GET_ATTR) \
	MPI(ENUM_MPI_WIN_GET_ERRHANDLER) \
	MPI(ENUM_MPI_WIN_GET_GROUP) \
	MPI(ENUM_MPI_WIN_GET_INFO) \
	MPI(ENUM_MPI_WIN_GET_NAME) \
	MPI(ENUM_MPI_WIN_LOCK) \
	MPI(ENUM_MPI_WIN_LOCK_ALL) \
	MPI(ENUM_MPI_WIN_POST) \
	MPI(ENUM_MPI_WIN_SET_ATTR) \
	MPI(ENUM_MPI_WIN_SET_ERRHANDLER) \
	MPI(ENUM_MPI_WIN_SET_INFO) \
	MPI(ENUM_MPI_WIN_SET_NAME) \
	MPI(ENUM_MPI_WIN_SHARED_QUERY) \
	MPI(ENUM_MPI_WIN_START) \
	MPI(ENUM_MPI_WIN_SYNC) \
	MPI(ENUM_MPI_WIN_TEST) \
	MPI(ENUM_MPI_WIN_UNLOCK) \
	MPI(ENUM_MPI_WIN_UNLOCK_ALL) \
	MPI(ENUM_MPI_WIN_WAIT) \
	MPI(NUM_MPI_TYPE) \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

// Generate enumerator
typedef enum MPI_Type {
    FOREACH_MPI(GENERATE_ENUM)
} MPI_Type_t;

// Generate array string of enumerator
__attribute__((unused)) static const char *mpi_type_str[] = {
    FOREACH_MPI(GENERATE_STRING)
};


typedef struct
{
	uint64_t **tsc;
    uint64_t **aperf;
    uint64_t **mperf;
    uint64_t **inst_ret;
    uint64_t **clk_curr;
    uint64_t **clk_ref;
    uint64_t **temp;
    uint64_t **temp_target;
    uint64_t **pmc0;
    uint64_t **pmc1;
    uint64_t **pmc2;
    uint64_t **pmc3;
    uint64_t **pmc4;
    uint64_t **pmc5;
    uint64_t **pmc6;
    uint64_t **pmc7;
    uint64_t **C3;
    uint64_t **C6;
    uint64_t **C7;
} CNTD_Batch_Cpu_t;

typedef struct
{
	uint64_t **energy_pkg;
    uint64_t **energy_dram;
	uint64_t **pcu0;
	uint64_t **pcu1;
	uint64_t **pcu2;
	uint64_t **pcu3;
    uint64_t **C2;
    uint64_t **C3;
    uint64_t **C6;
    uint64_t **C7;
    uint64_t **temp;
    uint64_t **uclk;
} CNTD_Batch_Socket_t;


typedef struct
{
	// General
	int my_rank;
	int cpu_id;
	int socket_id;
	char hostname[STRING_SIZE];
	char cpu_model_name[STRING_SIZE];
	int process_id;
	int size;

	// Info architecture
	int cpus;
	int sockets;
	int cores;
	int ht_enable;
	int min_pstate;
	int max_pstate;
	int nominal_freq;

	// Timing
	double epoch[2];
	double epoch_sample[2];
	int phase;

	// MPI call
	uint64_t mpi_count[NUM_MPI_TYPE][2];
	uint64_t mpi_send_data[NUM_MPI_TYPE][2];
	uint64_t mpi_recv_data[NUM_MPI_TYPE][2];
	double mpi_timing[NUM_MPI_TYPE][2];
	uint64_t mpi_tsc[NUM_MPI_TYPE][2];
	uint64_t mpi_inst_ret[NUM_MPI_TYPE][2];
	uint64_t mpi_clk_curr[NUM_MPI_TYPE][2];
	uint64_t mpi_clk_ref[NUM_MPI_TYPE][2];

	// Network & File
	uint64_t net[2];
	uint64_t file[2];

	// Time
	double timing[4];

	// HW perf counters
	uint64_t tsc[3];
	uint64_t inst_ret[3];
	uint64_t clk_curr[3];
	uint64_t clk_ref[3];

	// PMC
	uint64_t pmc0[3];
    uint64_t pmc1[3];
    uint64_t pmc2[3];
    uint64_t pmc3[3];
    uint64_t pmc4[3];
    uint64_t pmc5[3];
    uint64_t pmc6[3];
    uint64_t pmc7[3];
} CNTD_Rank_t;

typedef struct
{
	int cpu_id;
	char hostname[STRING_SIZE];

	uint64_t num_samples;

    uint64_t tsc;
    uint64_t aperf;
    uint64_t mperf;
    uint64_t inst_ret;
    uint64_t clk_curr;
    uint64_t clk_ref;

    double temp;

    uint64_t C3;
    uint64_t C6;
    uint64_t C7;

    uint64_t pmc0;
    uint64_t pmc1;
    uint64_t pmc2;
    uint64_t pmc3;
    uint64_t pmc4;
    uint64_t pmc5;
    uint64_t pmc6;
    uint64_t pmc7;
} CNTD_Cpu_t;

typedef struct
{
	int socket_id;
	char hostname[STRING_SIZE];

	double rapl_joules;
    double rapl_watts;
    double rapl_seconds;

	uint64_t num_samples;

    uint64_t energy_pkg;
    uint64_t energy_dram;

    double temp;
    uint64_t uclk;

    uint64_t C2;
    uint64_t C3;
    uint64_t C6;
    uint64_t C7;

    uint64_t pcu0;
    uint64_t pcu1;
    uint64_t pcu2;
    uint64_t pcu3;
} CNTD_Socket_t;

typedef struct
{
	uint64_t next_call;
	uint64_t when;
	uint64_t next_pstate;
	uint64_t next_tstate;
} CNTD_EAMO_t;

typedef struct
{
	MPI_Group mpi_group;
	int idx;
	int size;
	int local_rank;
	int world_rank;
	int *world_ranks;
} CNTD_Group_t;

typedef struct
{
	MPI_Comm mpi_comm;
	CNTD_Group_t *cntd_group;
	int idx;
	char name_idx;
	int rank;
} CNTD_Comm_t;

typedef struct
{
	CNTD_Comm_t *cntd_comm;

	uint64_t idx;
	MPI_Type_t mpi_type;
	int flag_eam;

	// Timing
	double epoch[2];

	// Network & File
	uint64_t net[2];
	uint64_t file[2];

	// HW perf counters
	uint64_t tsc[2];
	uint64_t fix[3][2];
	uint64_t pmc[8][2];
} CNTD_Call_t;

typedef struct
{
    // Timing
	double epoch[2];

	// Shared data
	CNTD_Rank_t *rank;
	CNTD_Rank_t **shmem_local_rank;
	CNTD_Cpu_t *cpu;
	CNTD_Socket_t *socket;

	CNTD_Rank_t *last_batch_rank;
	CNTD_Cpu_t *last_batch_cpu;
	CNTD_Socket_t *last_batch_socket;
	
    // Batch
    CNTD_Batch_Cpu_t batch_cpu;
	CNTD_Batch_Socket_t batch_socket;

	// Groups
	CNTD_Group_t *group;
	uint64_t group_count;
	uint64_t group_mem_limit;

	// Communicators
	CNTD_Comm_t *comm;
	uint64_t comm_count;
	uint64_t comm_mem_limit;

	// Call
	CNTD_Call_t call[2];
	int curr_call;
	int prev_call;
	uint64_t call_count;
	int log_call;

	// EAMO
	int eamo;
	CNTD_EAMO_t *eamo_map;
	int eamo_curr_pstate;
	int eamo_curr_tstate;
	int eamo_count;

	// EAM
	int eam;
	int eam_timeout;
	timer_t sampling_timer;

	// Local master
	MPI_Comm comm_local_masters;
	MPI_Comm comm_local_procs;
	int my_local_rank;
	int local_size;

	// Flags
	int node_sampling;
	int adv_metrics;

	// RAPL
    struct rapl_units *ru;
    struct rapl_data *rd;
    uint64_t *rapl_flags;
    int ri_stat;

	// File descriptors
	char log_dir[STRING_SIZE];
	FILE *fd_mpicall;
	FILE *fd_sampling;
} CNTD_t;

CNTD_t *cntd;

// HEADERS
// init.c
void callback_batch(int sig, siginfo_t *si, void *uc);
void start_cntd();
void stop_cntd();
void call_start(CNTD_Call_t *call);
void call_end(CNTD_Call_t *call);

// memory.c
int world_rank_2_local_rank(int rank, CNTD_Group_t* group);
CNTD_Call_t* add_cntd_call(MPI_Type_t mpi_type, MPI_Comm mpi_comm);
void check_mem_cntd_comm();
void check_mem_cntd_group();
CNTD_Group_t* lookup_cntd_group(MPI_Group group);
CNTD_Comm_t* lookup_cntd_comm(MPI_Comm comm);

// monitor.c
void add_profiling(CNTD_Call_t *call, int when);
void add_network(CNTD_Call_t *call, 
	const int *send_count, MPI_Datatype *send_type, int dest, 
	const int *recv_count, MPI_Datatype *recv_type, int source);
void add_file(CNTD_Call_t *call, 
	int read_count, MPI_Datatype read_datatype,
	int write_count, MPI_Datatype write_datatype);
void update_call();
void switch_call_ptr();

// batch.c
void init_batch_cpu(int batch, CNTD_Batch_Cpu_t *cpu);
void init_batch_socket(int batch, CNTD_Batch_Socket_t *socket);
double sample_batch();
void update_last_batch(double epoch);
void update_batch(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket);

// log.c
void open_mpicall_file(int log_call);
void close_mpicall_file();
void open_sampling_file(const char mode[]);
void close_sampling_file();
void print_label_sampling_file();
void delete_sampling_file();
void print_mpicall();
void print_batch(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket);
void print_logs();

// pm.c
int read_current_pstate();
int read_target_pstate();
void write_pstate();
void reset_pstate();
int read_tstate();
void write_tstate();
void reset_tstate();

// arch.c
void enable_uncore_freq();
void disable_uncore_freq();
void get_brand_string(char *cpu_model_name);
void read_arch_info();

// eam.c
void eamo_load_maps(char *output_dir);
void eamo_sched_next_conf(CNTD_Call_t *call, int when);
void eam_call_back(int signum);
void eam(CNTD_Call_t *call, int when);

// tool.c
int str_to_bool(const char str[]);
uint32_t diff_32(uint32_t end, uint32_t start);
uint64_t diff_48(uint64_t end, uint64_t start);
uint64_t diff_64(uint64_t end, uint64_t start);
void remove_spaces(char* str);
void create_dir(const char output_dir[], char log_dir[]);
int mkpath(const char dir[], mode_t mode);
struct timespec time_sum(struct timespec par_1, struct timespec par_2);
struct timespec time_diff(struct timespec par_1, struct timespec par_2);
double timespec2double(struct timespec timing);

// calc.h
double get_load(uint64_t clk_ref, uint64_t tsc);
double get_core_freq(uint64_t clk_curr, uint64_t clk_ref, uint64_t nominal_freq);
double get_cpi(uint64_t clk_ref, uint64_t inst_ret);
double get_core_cstate(uint64_t cstate, uint64_t tsc);
double get_core_C0(uint64_t clk_ref, uint64_t tsc);
double get_core_C1(uint64_t clk_ref, uint64_t tsc, uint64_t C3, uint64_t C6, uint64_t C7);
double get_energy(uint64_t energy, uint64_t energy_unit);
double get_power(uint64_t energy, uint64_t energy_unit, double timing);
double get_pkg_cstate(uint64_t cstate, uint64_t tsc);
double get_pkg_C0(uint64_t C2, uint64_t C3, uint64_t C6, uint64_t C7, uint64_t tsc);
double get_uncore_freq(uint64_t uclk, double timing, uint64_t C2, uint64_t C3, uint64_t C6, uint64_t C7, uint64_t tsc);
double get_core_temp(uint64_t temp, uint64_t temp_target);
double get_pkg_temp(uint64_t temp, uint64_t temp_target);

// share.c
CNTD_Rank_t* create_shmem_rank(const char shmem_name[], int num_elem);
void destroy_shmem_rank(CNTD_Rank_t *shmem_ptr, int num_elem, const char shmem_name[]);
CNTD_Rank_t* get_shmem_rank(const char shmem_name[], int num_elem);

CNTD_Cpu_t* create_shmem_cpu(const char shmem_name[]);
void destroy_shmem_cpu(CNTD_Cpu_t *shmem_ptr, const char shmem_name[]);
CNTD_Cpu_t* get_shmem_cpu(const char shmem_name[]);

CNTD_Socket_t* create_shmem_socket(const char shmem_name[]);
void destroy_shmem_socket(CNTD_Socket_t *shmem_ptr, const char shmem_name[]);
CNTD_Socket_t* get_shmem_socket(const char shmem_name[]);

MPI_Datatype get_mpi_datatype_rank();
MPI_Datatype get_mpi_datatype_cpu();
MPI_Datatype get_mpi_datatype_socket();

#endif // _CNTD_H_
