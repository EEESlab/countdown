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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>

// MPI
#include <mpi.h>

// Libhwloc
#include "hwloc.h"

// Stacktrace + Libunwind
#include "stacktrace/stacktrace.h"

// Libmsr
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
#else	 /// GNU build
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

// EAM configurations
#define DEFAULT_TIMEOUT 0.0005				// 500us

// Time trace timer
#define DEFAULT_TIME_TRACE_TIMER 1		// seconds

#define CLOCK_TYPE CLOCK_REALTIME

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

#define NOT_DEFINED -1

#define APP 0
#define MPI 1
#define TOT 2
#define GT_TIMEOUT 3

#define SLACK 1
#define MISS_APP 2
#define MISS_SLACK 3

#define NO_EAM 0
#define EAM 1

#define SEND 0
#define RECV 1

#define SOURCE 0
#define DEST 1

#define READ 0
#define WRITE 1

#define IN 0
#define OUT 1

#define MIN 0
#define NOM 1
#define MAX 2

#define SUM 1

#define EAM_SLACK 0
#define EAM_CALL 1

#define MPI_TYPE_TOT 0
#define MPI_TYPE_EAM 1

#define MPI_NONE -1
#define MPI_ALL -2
#define MPI_ALLV -3
#define MPI_ALLW -4

#define CNTD_MPI_ROOT 0
#define CNTD_GROUP_WORLD_IDX 0
#define CNTD_COMM_WORLD_IDX 0

#define NUM_PSTATE 50

// Batch
#define CPU_READ_BATCH USR_BATCH0
#define SOCKET_READ_BATCH USR_BATCH1
#define ENABLE_UNC_FREQ_BATCH USR_BATCH2
#define DISABLE_UNC_FREQ_BATCH USR_BATCH3

// Uncore frequency stuffs
#define SMSR_PLATFORM_INFO				(0xCE)
#define MSR_U_PMON_UCLK_FIXED_CTL	(0x703)
#define MSR_U_PMON_UCLK_FIXED_CTR	(0x704)
#define CONF_UNCORE_FREQ_ENABLE		(0x400000)


// Enumerator
#define FOREACH_MPI(MPI) \
	MPI(__MPI_ABORT) \
	MPI(__MPI_ACCUMULATE) \
	MPI(__MPI_ADD_ERROR_CLASS) \
	MPI(__MPI_ADD_ERROR_CODE) \
	MPI(__MPI_ADD_ERROR_STRING) \
	MPI(__MPI_ADDRESS) \
	MPI(__MPI_ALLGATHER) \
	MPI(__MPI_IALLGATHER) \
	MPI(__MPI_ALLGATHERV) \
	MPI(__MPI_IALLGATHERV) \
	MPI(__MPI_ALLOC_MEM) \
	MPI(__MPI_ALLREDUCE) \
	MPI(__MPI_IALLREDUCE) \
	MPI(__MPI_ALLTOALL) \
	MPI(__MPI_IALLTOALL) \
	MPI(__MPI_ALLTOALLV) \
	MPI(__MPI_IALLTOALLV) \
	MPI(__MPI_ALLTOALLW) \
	MPI(__MPI_IALLTOALLW) \
	MPI(__MPI_ATTR_DELETE) \
	MPI(__MPI_ATTR_GET) \
	MPI(__MPI_ATTR_PUT) \
	MPI(__MPI_BARRIER) \
	MPI(__MPI_IBARRIER) \
	MPI(__MPI_BCAST) \
	MPI(__MPI_BSEND) \
	MPI(__MPI_IBCAST) \
	MPI(__MPI_BSEND_INIT) \
	MPI(__MPI_BUFFER_ATTACH) \
	MPI(__MPI_BUFFER_DETACH) \
	MPI(__MPI_CANCEL) \
	MPI(__MPI_CART_COORDS) \
	MPI(__MPI_CART_CREATE) \
	MPI(__MPI_CART_GET) \
	MPI(__MPI_CART_MAP) \
	MPI(__MPI_CART_RANK) \
	MPI(__MPI_CART_SHIFT) \
	MPI(__MPI_CART_SUB) \
	MPI(__MPI_CARTDIM_GET) \
	MPI(__MPI_CLOSE_PORT) \
	MPI(__MPI_COMM_ACCEPT) \
	MPI(__MPI_FMPI_COMM_C2F) \
	MPI(__MPI_COMM_CALL_ERRHANDLER) \
	MPI(__MPI_COMM_COMPARE) \
	MPI(__MPI_COMM_CONNECT) \
	MPI(__MPI_COMM_CREATE_ERRHANDLER) \
	MPI(__MPI_COMM_CREATE_KEYVAL) \
	MPI(__MPI_COMM_CREATE_GROUP) \
	MPI(__MPI_COMM_CREATE) \
	MPI(__MPI_COMM_DELETE_ATTR) \
	MPI(__MPI_COMM_DISCONNECT) \
	MPI(__MPI_COMM_DUP) \
	MPI(__MPI_COMM_IDUP) \
	MPI(__MPI_COMM_DUP_WITH_INFO) \
	MPI(__MPI_COMM_F2C) \
	MPI(__MPI_COMM_FREE_KEYVAL) \
	MPI(__MPI_COMM_FREE) \
	MPI(__MPI_COMM_GET_ATTR) \
	MPI(__MPI_DIST_GRAPH_CREATE) \
	MPI(__MPI_DIST_GRAPH_CREATE_ADJACENT) \
	MPI(__MPI_DIST_GRAPH_NEIGHBORS) \
	MPI(__MPI_DIST_GRAPH_NEIGHBORS_COUNT) \
	MPI(__MPI_COMM_GET_ERRHANDLER) \
	MPI(__MPI_COMM_GET_INFO) \
	MPI(__MPI_COMM_GET_NAME) \
	MPI(__MPI_COMM_GET_PARENT) \
	MPI(__MPI_COMM_GROUP) \
	MPI(__MPI_COMM_JOIN) \
	MPI(__MPI_COMM_RANK) \
	MPI(__MPI_COMM_REMOTE_GROUP) \
	MPI(__MPI_COMM_REMOTE_SIZE) \
	MPI(__MPI_COMM_SET_ATTR) \
	MPI(__MPI_COMM_SET_ERRHANDLER) \
	MPI(__MPI_COMM_SET_INFO) \
	MPI(__MPI_COMM_SET_NAME) \
	MPI(__MPI_COMM_SIZE) \
	MPI(__MPI_COMM_SPAWN) \
	MPI(__MPI_COMM_SPAWN_MULTIPLE) \
	MPI(__MPI_COMM_SPLIT) \
	MPI(__MPI_COMM_SPLIT_TYPE) \
	MPI(__MPI_COMM_TEST_INTER) \
	MPI(__MPI_COMPARE_AND_SWAP) \
	MPI(__MPI_DIMS_CREATE) \
	MPI(__MPI_FMPI_ERRHANDLER_C2F) \
	MPI(__MPI_ERRHANDLER_CREATE) \
	MPI(__MPI_ERRHANDLER_F2C) \
	MPI(__MPI_ERRHANDLER_FREE) \
	MPI(__MPI_ERRHANDLER_GET) \
	MPI(__MPI_ERRHANDLER_SET) \
	MPI(__MPI_ERROR_CLASS) \
	MPI(__MPI_ERROR_STRING) \
	MPI(__MPI_EXSCAN) \
	MPI(__MPI_FETCH_AND_OP) \
	MPI(__MPI_IEXSCAN) \
	MPI(__MPI_FILE_C2F) \
	MPI(__MPI_FILE_F2C) \
	MPI(__MPI_FILE_CALL_ERRHANDLER) \
	MPI(__MPI_FILE_CREATE_ERRHANDLER) \
	MPI(__MPI_FILE_SET_ERRHANDLER) \
	MPI(__MPI_FILE_GET_ERRHANDLER) \
	MPI(__MPI_FILE_OPEN) \
	MPI(__MPI_FILE_CLOSE) \
	MPI(__MPI_FILE_DELETE) \
	MPI(__MPI_FILE_SET_SIZE) \
	MPI(__MPI_FILE_PREALLOCATE) \
	MPI(__MPI_FILE_GET_SIZE) \
	MPI(__MPI_FILE_GET_GROUP) \
	MPI(__MPI_FILE_GET_AMODE) \
	MPI(__MPI_FILE_SET_INFO) \
	MPI(__MPI_FILE_GET_INFO) \
	MPI(__MPI_FILE_SET_VIEW) \
	MPI(__MPI_FILE_GET_VIEW) \
	MPI(__MPI_FILE_READ_AT) \
	MPI(__MPI_FILE_READ_AT_ALL) \
	MPI(__MPI_FILE_WRITE_AT) \
	MPI(__MPI_FILE_WRITE_AT_ALL) \
	MPI(__MPI_FILE_IREAD_AT) \
	MPI(__MPI_FILE_IWRITE_AT) \
	MPI(__MPI_FILE_IREAD_AT_ALL) \
	MPI(__MPI_FILE_IWRITE_AT_ALL) \
	MPI(__MPI_FILE_READ) \
	MPI(__MPI_FILE_READ_ALL) \
	MPI(__MPI_FILE_WRITE) \
	MPI(__MPI_FILE_WRITE_ALL) \
	MPI(__MPI_FILE_IREAD) \
	MPI(__MPI_FILE_IWRITE) \
	MPI(__MPI_FILE_IREAD_ALL) \
	MPI(__MPI_FILE_IWRITE_ALL) \
	MPI(__MPI_FILE_SEEK) \
	MPI(__MPI_FILE_GET_POSITION) \
	MPI(__MPI_FILE_GET_BYTE_OFFSET) \
	MPI(__MPI_FILE_READ_SHARED) \
	MPI(__MPI_FILE_WRITE_SHARED) \
	MPI(__MPI_FILE_IREAD_SHARED) \
	MPI(__MPI_FILE_IWRITE_SHARED) \
	MPI(__MPI_FILE_READ_ORDERED) \
	MPI(__MPI_FILE_WRITE_ORDERED) \
	MPI(__MPI_FILE_SEEK_SHARED) \
	MPI(__MPI_FILE_GET_POSITION_SHARED) \
	MPI(__MPI_FILE_READ_AT_ALL_BEGIN) \
	MPI(__MPI_FILE_READ_AT_ALL_END) \
	MPI(__MPI_FILE_WRITE_AT_ALL_BEGIN) \
	MPI(__MPI_FILE_WRITE_AT_ALL_END) \
	MPI(__MPI_FILE_READ_ALL_BEGIN) \
	MPI(__MPI_FILE_READ_ALL_END) \
	MPI(__MPI_FILE_WRITE_ALL_BEGIN) \
	MPI(__MPI_FILE_WRITE_ALL_END) \
	MPI(__MPI_FILE_READ_ORDERED_BEGIN) \
	MPI(__MPI_FILE_READ_ORDERED_END) \
	MPI(__MPI_FILE_WRITE_ORDERED_BEGIN) \
	MPI(__MPI_FILE_WRITE_ORDERED_END) \
	MPI(__MPI_FILE_GET_TYPE_EXTENT) \
	MPI(__MPI_FILE_SET_ATOMICITY) \
	MPI(__MPI_FILE_GET_ATOMICITY) \
	MPI(__MPI_FILE_SYNC) \
	MPI(__MPI_FINALIZE) \
	MPI(__MPI_FINALIZED) \
	MPI(__MPI_FREE_MEM) \
	MPI(__MPI_GATHER) \
	MPI(__MPI_IGATHER) \
	MPI(__MPI_GATHERV) \
	MPI(__MPI_IGATHERV) \
	MPI(__MPI_GET_ADDRESS) \
	MPI(__MPI_GET_COUNT) \
	MPI(__MPI_GET_ELEMENTS) \
	MPI(__MPI_GET_ELEMENTS_X) \
	MPI(__MPI_GET) \
	MPI(__MPI_GET_ACCUMULATE) \
	MPI(__MPI_GET_LIBRARY_VERSION) \
	MPI(__MPI_GET_PROCESSOR_NAME) \
	MPI(__MPI_GET_VERSION) \
	MPI(__MPI_GRAPH_CREATE) \
	MPI(__MPI_GRAPH_GET) \
	MPI(__MPI_GRAPH_MAP) \
	MPI(__MPI_GRAPH_NEIGHBORS_COUNT) \
	MPI(__MPI_GRAPH_NEIGHBORS) \
	MPI(__MPI_GRAPHDIMS_GET) \
	MPI(__MPI_GREQUEST_COMPLETE) \
	MPI(__MPI_GREQUEST_START) \
	MPI(__MPI_FMPI_GROUP_C2F) \
	MPI(__MPI_GROUP_COMPARE) \
	MPI(__MPI_GROUP_DIFFERENCE) \
	MPI(__MPI_GROUP_EXCL) \
	MPI(__MPI_GROUP_F2C) \
	MPI(__MPI_GROUP_FREE) \
	MPI(__MPI_GROUP_INCL) \
	MPI(__MPI_GROUP_INTERSECTION) \
	MPI(__MPI_GROUP_RANGE_EXCL) \
	MPI(__MPI_GROUP_RANGE_INCL) \
	MPI(__MPI_GROUP_RANK) \
	MPI(__MPI_GROUP_SIZE) \
	MPI(__MPI_GROUP_TRANSLATE_RANKS) \
	MPI(__MPI_GROUP_UNION) \
	MPI(__MPI_IBSEND) \
	MPI(__MPI_IMPROBE) \
	MPI(__MPI_IMRECV) \
	MPI(__MPI_FMPI_INFO_C2F) \
	MPI(__MPI_INFO_CREATE) \
	MPI(__MPI_INFO_DELETE) \
	MPI(__MPI_INFO_DUP) \
	MPI(__MPI_INFO_F2C) \
	MPI(__MPI_INFO_FREE) \
	MPI(__MPI_INFO_GET) \
	MPI(__MPI_INFO_GET_NKEYS) \
	MPI(__MPI_INFO_GET_NTHKEY) \
	MPI(__MPI_INFO_GET_VALUELEN) \
	MPI(__MPI_INFO_SET) \
	MPI(__MPI_INIT) \
	MPI(__MPI_INITIALIZED) \
	MPI(__MPI_INIT_THREAD) \
	MPI(__MPI_INTERCOMM_CREATE) \
	MPI(__MPI_INTERCOMM_MERGE) \
	MPI(__MPI_IPROBE) \
	MPI(__MPI_IRECV) \
	MPI(__MPI_IRSEND) \
	MPI(__MPI_ISEND) \
	MPI(__MPI_ISSEND) \
	MPI(__MPI_IS_THREAD_MAIN) \
	MPI(__MPI_KEYVAL_CREATE) \
	MPI(__MPI_KEYVAL_FREE) \
	MPI(__MPI_LOOKUP_NAME) \
	MPI(__MPI_FMPI_MESSAGE_C2F) \
	MPI(__MPI_MESSAGE_F2C) \
	MPI(__MPI_MPROBE) \
	MPI(__MPI_MRECV) \
	MPI(__MPI_NEIGHBOR_ALLGATHER) \
	MPI(__MPI_INEIGHBOR_ALLGATHER) \
	MPI(__MPI_NEIGHBOR_ALLGATHERV) \
	MPI(__MPI_INEIGHBOR_ALLGATHERV) \
	MPI(__MPI_NEIGHBOR_ALLTOALL) \
	MPI(__MPI_INEIGHBOR_ALLTOALL) \
	MPI(__MPI_NEIGHBOR_ALLTOALLV) \
	MPI(__MPI_INEIGHBOR_ALLTOALLV) \
	MPI(__MPI_NEIGHBOR_ALLTOALLW) \
	MPI(__MPI_INEIGHBOR_ALLTOALLW) \
	MPI(__MPI_FMPI_OP_C2F) \
	MPI(__MPI_OP_COMMUTATIVE) \
	MPI(__MPI_OP_CREATE) \
	MPI(__MPI_OPEN_PORT) \
	MPI(__MPI_OP_F2C) \
	MPI(__MPI_OP_FREE) \
	MPI(__MPI_PACK_EXTERNAL) \
	MPI(__MPI_PACK_EXTERNAL_SIZE) \
	MPI(__MPI_PACK) \
	MPI(__MPI_PACK_SIZE) \
	MPI(__MPI_PCONTROL) \
	MPI(__MPI_PROBE) \
	MPI(__MPI_PUBLISH_NAME) \
	MPI(__MPI_PUT) \
	MPI(__MPI_QUERY_THREAD) \
	MPI(__MPI_RACCUMULATE) \
	MPI(__MPI_RECV_INIT) \
	MPI(__MPI_RECV) \
	MPI(__MPI_REDUCE) \
	MPI(__MPI_IREDUCE) \
	MPI(__MPI_REDUCE_LOCAL) \
	MPI(__MPI_REDUCE_SCATTER) \
	MPI(__MPI_IREDUCE_SCATTER) \
	MPI(__MPI_REDUCE_SCATTER_BLOCK) \
	MPI(__MPI_IREDUCE_SCATTER_BLOCK) \
	MPI(__MPI_REGISTER_DATAREP) \
	MPI(__MPI_FMPI_REQUEST_C2F) \
	MPI(__MPI_REQUEST_F2C) \
	MPI(__MPI_REQUEST_FREE) \
	MPI(__MPI_REQUEST_GET_STATUS) \
	MPI(__MPI_RGET) \
	MPI(__MPI_RGET_ACCUMULATE) \
	MPI(__MPI_RPUT) \
	MPI(__MPI_RSEND) \
	MPI(__MPI_RSEND_INIT) \
	MPI(__MPI_SCAN) \
	MPI(__MPI_ISCAN) \
	MPI(__MPI_SCATTER) \
	MPI(__MPI_ISCATTER) \
	MPI(__MPI_SCATTERV) \
	MPI(__MPI_ISCATTERV) \
	MPI(__MPI_SEND_INIT) \
	MPI(__MPI_SEND) \
	MPI(__MPI_SENDRECV) \
	MPI(__MPI_SENDRECV_REPLACE) \
	MPI(__MPI_SSEND_INIT) \
	MPI(__MPI_SSEND) \
	MPI(__MPI_START) \
	MPI(__MPI_STARTALL) \
	MPI(__MPI_STATUS_C2F) \
	MPI(__MPI_STATUS_F2C) \
	MPI(__MPI_STATUS_SET_CANCELLED) \
	MPI(__MPI_STATUS_SET_ELEMENTS) \
	MPI(__MPI_STATUS_SET_ELEMENTS_X) \
	MPI(__MPI_TESTALL) \
	MPI(__MPI_TESTANY) \
	MPI(__MPI_TEST) \
	MPI(__MPI_TEST_CANCELLED) \
	MPI(__MPI_TESTSOME) \
	MPI(__MPI_TOPO_TEST) \
	MPI(__MPI_FMPI_TYPE_C2F) \
	MPI(__MPI_TYPE_COMMIT) \
	MPI(__MPI_TYPE_CONTIGUOUS) \
	MPI(__MPI_TYPE_CREATE_DARRAY) \
	MPI(__MPI_TYPE_CREATE_F90_COMPLEX) \
	MPI(__MPI_TYPE_CREATE_F90_INTEGER) \
	MPI(__MPI_TYPE_CREATE_F90_REAL) \
	MPI(__MPI_TYPE_CREATE_HINDEXED_BLOCK) \
	MPI(__MPI_TYPE_CREATE_HINDEXED) \
	MPI(__MPI_TYPE_CREATE_HVECTOR) \
	MPI(__MPI_TYPE_CREATE_KEYVAL) \
	MPI(__MPI_TYPE_CREATE_INDEXED_BLOCK) \
	MPI(__MPI_TYPE_CREATE_STRUCT) \
	MPI(__MPI_TYPE_CREATE_SUBARRAY) \
	MPI(__MPI_TYPE_CREATE_RESIZED) \
	MPI(__MPI_TYPE_DELETE_ATTR) \
	MPI(__MPI_TYPE_DUP) \
	MPI(__MPI_TYPE_EXTENT) \
	MPI(__MPI_TYPE_FREE) \
	MPI(__MPI_TYPE_FREE_KEYVAL) \
	MPI(__MPI_TYPE_F2C) \
	MPI(__MPI_TYPE_GET_ATTR) \
	MPI(__MPI_TYPE_GET_CONTENTS) \
	MPI(__MPI_TYPE_GET_ENVELOPE) \
	MPI(__MPI_TYPE_GET_EXTENT) \
	MPI(__MPI_TYPE_GET_EXTENT_X) \
	MPI(__MPI_TYPE_GET_NAME) \
	MPI(__MPI_TYPE_GET_TRUE_EXTENT) \
	MPI(__MPI_TYPE_GET_TRUE_EXTENT_X) \
	MPI(__MPI_TYPE_HINDEXED) \
	MPI(__MPI_TYPE_HVECTOR) \
	MPI(__MPI_TYPE_INDEXED) \
	MPI(__MPI_TYPE_LB) \
	MPI(__MPI_TYPE_MATCH_SIZE) \
	MPI(__MPI_TYPE_SET_ATTR) \
	MPI(__MPI_TYPE_SET_NAME) \
	MPI(__MPI_TYPE_SIZE) \
	MPI(__MPI_TYPE_SIZE_X) \
	MPI(__MPI_TYPE_STRUCT) \
	MPI(__MPI_TYPE_UB) \
	MPI(__MPI_TYPE_VECTOR) \
	MPI(__MPI_UNPACK) \
	MPI(__MPI_UNPUBLISH_NAME) \
	MPI(__MPI_UNPACK_EXTERNAL ) \
	MPI(__MPI_WAITALL) \
	MPI(__MPI_WAITANY) \
	MPI(__MPI_WAIT) \
	MPI(__MPI_WAITSOME) \
	MPI(__MPI_WIN_ALLOCATE) \
	MPI(__MPI_WIN_ALLOCATE_SHARED) \
	MPI(__MPI_WIN_ATTACH) \
	MPI(__MPI_FMPI_WIN_C2F) \
	MPI(__MPI_WIN_CALL_ERRHANDLER) \
	MPI(__MPI_WIN_COMPLETE) \
	MPI(__MPI_WIN_CREATE) \
	MPI(__MPI_WIN_CREATE_DYNAMIC) \
	MPI(__MPI_WIN_CREATE_ERRHANDLER) \
	MPI(__MPI_WIN_CREATE_KEYVAL) \
	MPI(__MPI_WIN_DELETE_ATTR) \
	MPI(__MPI_WIN_DETACH) \
	MPI(__MPI_WIN_F2C) \
	MPI(__MPI_WIN_FENCE) \
	MPI(__MPI_WIN_FLUSH) \
	MPI(__MPI_WIN_FLUSH_ALL) \
	MPI(__MPI_WIN_FLUSH_LOCAL) \
	MPI(__MPI_WIN_FLUSH_LOCAL_ALL) \
	MPI(__MPI_WIN_FREE) \
	MPI(__MPI_WIN_FREE_KEYVAL) \
	MPI(__MPI_WIN_GET_ATTR) \
	MPI(__MPI_WIN_GET_ERRHANDLER) \
	MPI(__MPI_WIN_GET_GROUP) \
	MPI(__MPI_WIN_GET_INFO) \
	MPI(__MPI_WIN_GET_NAME) \
	MPI(__MPI_WIN_LOCK) \
	MPI(__MPI_WIN_LOCK_ALL) \
	MPI(__MPI_WIN_POST) \
	MPI(__MPI_WIN_SET_ATTR) \
	MPI(__MPI_WIN_SET_ERRHANDLER) \
	MPI(__MPI_WIN_SET_INFO) \
	MPI(__MPI_WIN_SET_NAME) \
	MPI(__MPI_WIN_SHARED_QUERY) \
	MPI(__MPI_WIN_START) \
	MPI(__MPI_WIN_SYNC) \
	MPI(__MPI_WIN_TEST) \
	MPI(__MPI_WIN_UNLOCK) \
	MPI(__MPI_WIN_UNLOCK_ALL) \
	MPI(__MPI_WIN_WAIT) \
	MPI(__MPI_SEND__BARRIER) \
	MPI(__MPI_SSEND__BARRIER) \
	MPI(__MPI_BSEND__BARRIER) \
	MPI(__MPI_RSEND__BARRIER) \
	MPI(__MPI_RECV__BARRIER) \
	MPI(__MPI_SENDRECV__BARRIER) \
	MPI(__MPI_SENDRECV_REPLACE__BARRIER) \
	MPI(__MPI_CART_CREATE__BARRIER) \
	MPI(__MPI_COMM_CREATE__BARRIER) \
	MPI(__MPI_COMM_SPLIT__BARRIER) \
	MPI(__MPI_ALLGATHER__BARRIER) \
	MPI(__MPI_ALLGATHERV__BARRIER) \
	MPI(__MPI_ALLREDUCE__BARRIER) \
	MPI(__MPI_ALLTOALL__BARRIER) \
	MPI(__MPI_ALLTOALLV__BARRIER) \
	MPI(__MPI_ALLTOALLW__BARRIER) \
	MPI(__MPI_BCAST__BARRIER) \
	MPI(__MPI_EXSCAN__BARRIER) \
	MPI(__MPI_SCAN__BARRIER) \
	MPI(__MPI_GATHER__BARRIER) \
	MPI(__MPI_GATHERV__BARRIER) \
	MPI(__MPI_REDUCE__BARRIER) \
	MPI(__MPI_REDUCE_SCATTER__BARRIER) \
	MPI(__MPI_SCATTER__BARRIER) \
	MPI(__MPI_SCATTERV__BARRIER) \
	MPI(__MPI_NEIGHBOR_ALLGATHER__BARRIER) \
	MPI(__MPI_NEIGHBOR_ALLGATHERV__BARRIER) \
	MPI(__MPI_NEIGHBOR_ALLTOALL__BARRIER) \
	MPI(__MPI_NEIGHBOR_ALLTOALLV__BARRIER) \
	MPI(__MPI_NEIGHBOR_ALLTOALLW__BARRIER) \
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
	int stacktrace;
	MPI_Type_t mpi_type;
	uint64_t net[2];
	uint64_t file[2];

	int flag_eam;
	double last_time_duration;

	// Profiling
	double timing_p[2];
	uint64_t tsc_p[2];
	uint64_t inst_ret_p[2];
	uint64_t clk_curr_p[2];
	uint64_t clk_ref_p[2];
	uint64_t pmc_p[2][8];
	uint64_t low_freq_count;
	uint64_t count;
} CNTD_Fermata_t;

typedef struct
{
	int task_id[2];
	int sched;
	double rates[NUM_PSTATE];
	MPI_Type_t mpi_type[2];

	// Current task info
	double epoch[2];
	uint64_t tsc[2];
	uint64_t fix[3][2];
	uint64_t pmc[8][2];

	// Profiling
	uint64_t sched_p[NUM_PSTATE];
	double timing_p[4];
	uint64_t tsc_p[4];
	uint64_t inst_ret_p[4];
	uint64_t clk_curr_p[4];
	uint64_t clk_ref_p[4];
	uint64_t pmc_p[4][8];
	uint64_t count;
	uint64_t miss_count;
} CNTD_Andante_t;

typedef struct
{
	// General
	char hostname[STRING_SIZE];
	char cpu_model_name[STRING_SIZE];

	// Info architecture
	int cpus;
	int sockets;
	int cores;
	int ht_enable;
	int turbo_ratio[MEM_SIZE];
	int num_procs_local_socket;
	int pstate[3];
	int nominal_freq;
} CNTD_Arch_t;

typedef struct
{
	// General
	int my_rank;
	int size;
	int cpu_id;
	int socket_id;
	int process_id;

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
	uint64_t mpi_pmc[NUM_MPI_TYPE][2][8];

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
	uint64_t pmc[3][8];
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
	int eam_policy;

	// Timing
	double epoch[2];

	// Network & File
	uint64_t net[2];
	int net_addr[2];
	uint64_t file[2];

	// Stacktrace hash
	int stacktrace;

	// HW perf counters
	uint64_t tsc[2];
	uint64_t fix[3][2];
	uint64_t pmc[8][2];
} CNTD_Call_t;

typedef struct
{
	// Architecture
	CNTD_Arch_t arch;

	// Timing
	double epoch[2];

	// Shared data
	CNTD_Rank_t *rank;
	CNTD_Rank_t **shmem_local_rank;
	CNTD_Cpu_t *cpu;
	CNTD_Socket_t *socket;

	CNTD_Rank_t *last_batch_ranks;
	CNTD_Cpu_t *last_batch_cpus;
	CNTD_Socket_t *last_batch_sockets;

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

	// Timeout
	double timeout;

	// Andante
	CNTD_Andante_t *andante_data;
	uint64_t andante_count;
	uint64_t andante_mem_limit;

	// Fermata
	CNTD_Fermata_t *fermata_data;
	uint64_t fermata_count;
	uint64_t fermata_mem_limit;

	// Timer of the time trace
	timer_t time_trace_timer;

	// Local master
	MPI_Comm comm_local_masters;
	MPI_Comm comm_local_procs;
	int my_local_rank;
	int local_size;

	// Flags actions
	int barrier;
	int fermata;
	int andante;
	int eam_call;
	int eam_slack;
	// Flags analisys
	int andante_analysis;
	int fermata_analysis;
	int eam_slack_analysis;
	int eam_call_analysis;
	// Flags trace
	int event_trace;
	int time_trace;
	int task_trace;
	// Other flags
	int pmc;
	int pcu;
	int debug_metrics;

	// RAPL
	struct rapl_units *ru;
	struct rapl_data *rd;
	uint64_t *rapl_flags;
	int ri_stat;

	// File descriptors
	char log_dir[STRING_SIZE];
	FILE *fd_event_trace;
	FILE *fd_task_trace;
	FILE *fd_time_trace;
} CNTD_t;

CNTD_t *cntd;

// HEADERS
// init.c
void start_cntd();
void stop_cntd();
void call_pre_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void call_start(CNTD_Call_t *call);
void call_end(CNTD_Call_t *call);

// barrier.c
int is_collective_barrier(MPI_Type_t mpi_type);
int is_p2p_barrier(MPI_Type_t mpi_type);
int is_wait_barrier(MPI_Type_t mpi_type);
int is_cntd_barrier(MPI_Type_t mpi_type);

void add_barrier(MPI_Type_t mpi_type, MPI_Comm comm, int addr);

// memory.c
void check_mem_andante();
void check_mem_fermata();
void check_mem_cntd_comm();
void check_mem_cntd_group();
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
void callback_batch(int sig, siginfo_t *si, void *uc);
double do_batch();
void update_last_batch(double epoch);
void update_batch(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket);

// log.c
void print_logs();

void open_event_trace_file();
void close_event_trace_file();
void print_event();

void open_task_trace_file();
void close_task_trace_file();
void print_task(CNTD_Andante_t *andante_task, CNTD_Call_t *call);

void open_time_trace_file(const char mode[]);
void close_time_trace_file();
void print_label_time_trace_file();
void print_time_trace(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket);

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
void read_arch_turbo_info();

// eam.c
void eam_init();
void eam_finalize();
void eam_callback(int signum);
void eam_pre_mpi(CNTD_Call_t *call);
void eam_post_mpi(CNTD_Call_t *call);

// eam_slack.c
void eam_slack_init();
void eam_slack_finalize();
void eam_slack_callback(int signum);
void eam_slack_pre_mpi(CNTD_Call_t *call);
void eam_slack_post_mpi(CNTD_Call_t *call);

// tool.c
int str_to_bool(const char str[]);
uint32_t diff_32(uint32_t end, uint32_t start);
uint64_t diff_48(uint64_t end, uint64_t start);
uint64_t diff_64(uint64_t end, uint64_t start);
void remove_spaces(char* str);
void create_dir(const char dir[]);
int mkpath(const char dir[], mode_t mode);
struct timespec time_sum(struct timespec par_1, struct timespec par_2);
struct timespec time_diff(struct timespec par_1, struct timespec par_2);
double timespec2double(struct timespec timing);
struct timespec double2timespec(double timing);
int timespec2str(char *buf, uint len, struct timespec ts);
int rmtree(const char *path);
int makeTimer(timer_t *timerID, int expire, int interval);
double standard_deviation(double data[], int n);
int world_rank_2_local_rank(int rank, CNTD_Group_t* group);

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
MPI_Datatype get_mpi_datatype_arch();

// stacktrace.h
int hash_backtrace(int fid);

// andante.c
void andante_init();
void andante_finalize();
void andante_pre_task(CNTD_Call_t *call, int task_id);
int andante_post_task(CNTD_Call_t *call, int task_id);

// fermata.c
void fermata_init();
void fermata_finalize();
void fermata_call_back(int signum);
void fermata_pre_mpi(CNTD_Call_t *call);
void fermata_post_mpi(CNTD_Call_t *call);

#endif // _CNTD_H_
