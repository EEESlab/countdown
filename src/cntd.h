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
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <mpi.h>


#ifndef _CNTD_H_
#define	_CNTD_H_

#ifdef __INTEL_COMPILER
#pragma warning disable 1786
#else   /// GNU build
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#define MEM_SIZE 128
#define STRING_SIZE 256

// Sample time for advanced metrics [seconds]
#define ADV_METRICS_TIMEOUT 1

// Callback time [microseconds]
#define EAM_TIMEOUT 500

#define CLOCK_TYPE CLOCK_REALTIME
//#define CLOCK_TYPE CLOCK_MONOTONIC

//#define READ_TSC RDTSC
#define READ_TSC RDTSCP


#define FALSE 0
#define TRUE 1

#define START 0
#define END 1
#define DURATION 2

#define APP 0
#define MPI 1

#define MPI_NONE -1
#define MPI_ALL -2
#define MPI_ALLV -3
#define MPI_ALLW -4

#define ROOT_MPI 0
#define CNTD_GROUP_WORLD_IDX 0
#define CNTD_COMM_WORLD_IDX 0

#define FORTRAN 0
#define C_CPP	1

#define MAX_48_BIT_UNSIGN 281474976710655

// MSR
#define IA32_MPERF						(0xE7)
#define IA32_APERF						(0xE8)
#define MSR_PLATFORM_INFO				(0xCE)

// P-STATE SET
#define IA32_PERF_STATUS 				(0x198)
#define IA32_PERF_CTL 					(0x199)

// T-STATE SET
#define IA32_CLOCK_MODULATION			(0x19A)

// TURBO
#define MSR_TURBO_RATIO_LIMIT			(0x1AD)

// RAPL PKG
#define MSR_RAPL_POWER_UNIT             (0x606)
#define MSR_PKG_POWER_LIMIT             (0x610)
#define MSR_PKG_ENERGY_STATUS           (0x611)
#define MSR_PKG_PERF_STATUS             (0x613)
#define MSR_PKG_POWER_INFO              (0x614)

// RAPL DRAM
#define MSR_DRAM_POWER_LIMIT            (0x618)
#define MSR_DRAM_ENERGY_STATUS          (0x619)
#define MSR_DRAM_PERF_STATUS            (0x61B)
#define MSR_DRAM_POWER_INFO             (0x61C)

// THERMAL
#define MSR_PACKAGE_THERM_STATUS        (0x1B1)
#define MSR_IA32_THERM_STATUS           (0x19C)

// UNCORE FREQ
#define MSR_U_PMON_UCLK_FIXED_CTL		(0x703)
#define MSR_U_PMON_UCLK_FIXED_CTR		(0x704)
#define CONF_UNCORE_FREQ_ENABLE			(0x400000)

// CORE C-STATES
#define MSR_CORE_C3_RESIDENCY           (0x3FC)
#define MSR_CORE_C6_RESIDENCY           (0x3FD)
#define MSR_CORE_C7_RESIDENCY           (0x3FE)

// PKG C-STATES
#define MSR_PKG_C2_RESIDENCY            (0x60D)
#define MSR_PKG_C3_RESIDENCY            (0x3F8)
#define MSR_PKG_C6_RESIDENCY            (0x3F9)

#define MSR_PKG_C8_RESIDENCY            (0x630)
#define MSR_PKG_C9_RESIDENCY            (0x631)
#define MSR_PKG_C10_RESIDENCY           (0x632)

// FIXED COUNTERS
#define IA32_PERF_GLOBAL_CTRL			(0x38F)
#define IA32_FIXED_CTR_CTRL				(0x38D)

// CONF FIXED COUNTERS
#define CONF_ENABLE_FIXED				(0x700000000)
#define CONF_START_FIXED				(0x333)

// FIXED PMC
#define RDPMC_INSTR		(1 << 30)
#define RDPMC_CLKCURR	((1 << 30) + 1)
#define RDPMC_CLKREF	((1 << 30) + 2)


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

// Intel P6
#define	PENTIUM_M_BANIAS     9
#define	PENTIUM_M_DOTHAN     13
#define	CORE_DUO             14
#define	CORE2_65             15
#define	CORE2_45             23
#define	ATOM                 28
#define	ATOM_45              38
#define	ATOM_32              54
#define	ATOM_22              39
#define	ATOM_SILVERMONT_E    55
#define	ATOM_SILVERMONT_C    77
#define	ATOM_SILVERMONT_Z1   74
#define	ATOM_SILVERMONT_Z2   90
#define	ATOM_SILVERMONT_F    93
#define	ATOM_SILVERMONT_AIR  76
#define	ATOM_SILVERMONT_GOLD 92
#define	ATOM_DENVERTON       95
#define	ATOM_GOLDMONT_PLUS   122
#define	NEHALEM_BLOOMFIELD   26
#define	NEHALEM_LYNNFIELD    30
#define	NEHALEM_LYNNFIELD_M  31
#define	NEHALEM_WESTMERE     44
#define	NEHALEM_WESTMERE_M   37
#define	SANDYBRIDGE          42
#define	SANDYBRIDGE_EP       45
#define	HASWELL              60
#define	HASWELL_EP           63
#define	HASWELL_M1           69
#define	HASWELL_M2           70
#define	IVYBRIDGE            58
#define	IVYBRIDGE_EP         62
#define	NEHALEM_EX           46
#define	WESTMERE_EX          47
#define	XEON_MP              29
#define	BROADWELL            61
#define	BROADWELL_E          79
#define	BROADWELL_D          86
#define	BROADWELL_E3         71
#define	SKYLAKE1             78
#define	SKYLAKE2             94
#define	SKYLAKEX             85
#define	KABYLAKE1            142
#define	KABYLAKE2            158
#define	CANNONLAKE           102
// Intel MIC
#define	XEON_PHI           	 1
#define	XEON_PHI_KNL       	 87
#define	XEON_PHI_KML       	 133

enum reg_ddcm
{
	clk_100_0 = 0,
	clk_12_5 = 1,
	clk_25_0 = 2,
	clk_37_5 = 3,
	clk_50_0 = 4,
	clk_63_5 = 5,
	clk_75_0 = 6,
	clk_87_5 = 7
};

enum reg_cpuid
{
	eax,
	ebx,
	ecx,
	edx
};

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
	char model_name[STRING_SIZE];
	char arch_name[STRING_SIZE];
	int smt;
	int nominal_freq;
	int num_cpus;
	int num_cores;
	int num_hw_threads;
	int num_sockets;
	int num_cores_per_socket;
	int num_hw_threads_per_socket;

	// Performance info
	int maximum_pstate;
	int nominal_pstate;
	int minimum_pstate;

	// RAPL settings
	double rapl_power_unit;
	double rapl_energy_unit;
	double rapl_time_unit;
	double rapl_thermal_spec_power;
} CNTD_Arch_t;

__attribute__((unused)) static const char *cust_metr_str[] = {
	"<500ns", 
	"500ns-5us", 
	"5us-50us", 
	"50us-500us", 
	"500us-5ms",
	"5ms-50ms", 
	"50ms-500ms", 
	"500ms-5s", 
	"5s-50s", 
	"50s-500s", 
	">500s"
};

enum cust_metr_enum
{
	_500ns,
	_500ns_5us,
	_5us_50us,
	_50us_500us,
	_500us_5ms,
	_5ms_50ms,
	_50ms_500ms,
	_500ms_5s,
	_5s_50s,
	_50s_500s,
	_500s,
	custmetr_enum_size
};

typedef struct
{
	double timing[11];
	double load[11];
	double core_freq[11];
	double cpi[11];

	uint64_t count[11];
} CNTD_CustMetr_t;

typedef struct
{
	uint64_t idx;
	uint64_t mpi_type;
	uint64_t comm;
	uint64_t group;

	double epoch[3];
	
	double timing[2];
	uint64_t tsc[2];

	// Network
	uint64_t tot_net_send;
	uint64_t tot_net_recv;
	int *net_send;
	int *net_recv;

	// Metrics
	uint64_t inst_ret[2];
	uint64_t clk_curr[2];
	uint64_t clk_ref[2];
	double load[2];
	double core_freq[2];
	double cpi[2];

	// PMU HW perf counters
	uint64_t pmu_1[2];
	uint64_t pmu_2[2];
	uint64_t pmu_3[2];
	uint64_t pmu_4[2];
	uint64_t pmu_5[2];
	uint64_t pmu_6[2];
	uint64_t pmu_7[2];
	uint64_t pmu_8[2];
} CNTD_Call_Phase_t;

typedef struct
{
	CNTD_Comm_t *cntd_comm;
	uint64_t idx;
	MPI_Type_t mpi_type;

	// Timing
	double epoch[2];
	uint64_t tsc[2];

	// Network
	uint64_t tot_net_send;
	int *net_send;
	uint64_t tot_net_recv;
	int *net_recv;

	// Fixed HW perf counters
	uint64_t fixed_1[2];
	uint64_t fixed_2[2];
	uint64_t fixed_3[2];

	// PMU HW perf counters
	uint64_t pmu_1[2];
	uint64_t pmu_2[2];
	uint64_t pmu_3[2];
	uint64_t pmu_4[2];
	uint64_t pmu_5[2];
	uint64_t pmu_6[2];
	uint64_t pmu_7[2];
	uint64_t pmu_8[2];
} CNTD_Call_t;

typedef struct
{
	uint64_t idx[2];
	char when[2];
	MPI_Type_t mpi_type[2];

	double timing[3];

	// Core metrics
	uint64_t tsc;
	uint64_t inst_ret;
	uint64_t clk_curr; 
	uint64_t clk_ref; 
	double load;
	double core_freq;
	double cpi;

	uint64_t pmu_1;
	uint64_t pmu_2;
	uint64_t pmu_3;
	uint64_t pmu_4;
	uint64_t pmu_5;
	uint64_t pmu_6;
	uint64_t pmu_7;
	uint64_t pmu_8;

	uint64_t aperf; 
	uint64_t mperf; 
	
	double core_c0; 
	double core_c1; 
	double core_c3; 
	double core_c6;

	int core_temp;

	// Uncore metrics
	double uncore_freq;

	double pkg_energy; 
	double dram_energy; 
	double pkg_power; 
	double dram_power;

	double pkg_c0; 
	double pkg_c2; 
	double pkg_c3; 
	double pkg_c6;

	int pkg_temp;

	// Network
	uint64_t tot_net_send;
	uint64_t tot_net_recv;
	uint64_t *net_send;
	uint64_t *net_recv;
} CNTD_AdvMetrics_Phase_t;

typedef struct
{
	// Call info
	uint64_t call_idx;
	int when;
	MPI_Type_t mpi_type;

	// Time
	uint64_t tsc;
	double epoch;

	// Core HW perf counters
	uint64_t fixed_1;
	uint64_t fixed_2;
	uint64_t fixed_3;

	uint64_t pmu_1;
	uint64_t pmu_2;
	uint64_t pmu_3;
	uint64_t pmu_4;
	uint64_t pmu_5;
	uint64_t pmu_6;
	uint64_t pmu_7;
	uint64_t pmu_8;

	uint64_t core_mperf;
	uint64_t core_aperf;
	uint64_t core_c3;
	uint64_t core_c6;
	uint64_t core_temp;

	// Uncore HW perf counters
	uint64_t uncore_clk;
	uint64_t pkg_energy;
	uint64_t dram_energy;
	uint64_t pkg_c2;
	uint64_t pkg_c3;
	uint64_t pkg_c6;
	uint64_t pkg_temp;

	// Network
	uint64_t tot_net_send;
	uint64_t *net_send;
	uint64_t tot_net_recv;
	uint64_t *net_recv;
} CNTD_AdvMetrics_t;

typedef struct
{
	// General 
	char hostname[STRING_SIZE];
	int cpu_id;
	int socket_id;
	int process_id;
	int mpi_rank;
	int mpi_size;

	// Groups
	CNTD_Group_t *group;
	uint64_t group_count;
	uint64_t group_mem_limit;

	// Communicators
	CNTD_Comm_t *comm;
	uint64_t comm_count;
	uint64_t comm_mem_limit;

	// MPI calls
	CNTD_Call_t call[3];
	int prev_call;
	int curr_call;
	uint64_t call_count;

	// Settings
	int call_prof_ctr;
	int net_prof_ctr;
	int fix_perf_ctr;
	int pmu_perf_ctr;
	int adv_metrics_ctr;
	int eamo;

	// Advanced metrics
	CNTD_AdvMetrics_t adv_metrics[2];
	int adv_metrics_prev;
	int adv_metrics_curr;
	double adv_metrics_timeout;
	double adv_metrics_last_epoch;

	// Timing
	double epoch[3];
	uint64_t tsc[3];

	// Network
	uint64_t tot_net_send;
	uint64_t *net_send;
	uint64_t tot_net_recv;
	uint64_t *net_recv;

	// Core metrics
	uint64_t inst_ret;
	double load;
	double core_freq;
	double cpi;

	uint64_t pmu_1;
	uint64_t pmu_2;
	uint64_t pmu_3;
	uint64_t pmu_4;
	uint64_t pmu_5;
	uint64_t pmu_6;
	uint64_t pmu_7;
	uint64_t pmu_8;

	double core_c0; 
	double core_c1; 
	double core_c3; 
	double core_c6;

	int core_temp;

	// Uncore metrics
	double uncore_freq;

	double pkg_energy; 
	double dram_energy; 
	double pkg_power; 
	double dram_power;

	double pkg_c0; 
	double pkg_c2; 
	double pkg_c3; 
	double pkg_c6;

	int pkg_temp;

	// Custom metrics
	CNTD_CustMetr_t cust_metr[2];

	// Energy aware MPI
	int eam;
	int eam_timeout;
	int eam_flag;

	// Energy-aware MPI Oracle (EAMO)
	CNTD_EAMO_t *eamo_map;
	uint64_t eamo_count;
	uint64_t eamo_curr_pstate;
	uint64_t eamo_curr_tstate;

	// File descriptors
	int fd_msr;
	FILE *fd_cntd;
	FILE *fd_group;
	FILE *fd_comm;
	FILE *fd_call;
	FILE *fd_adv_metrics;
	FILE *fd_cust_metr;
} CNTD_t;

CNTD_Arch_t *arch;
CNTD_t *cntd;

// HEADERS
// init.c
void init_cntd();
void initialize_cntd(CNTD_Call_t *call);
void finalize_cntd(CNTD_Call_t *call);
void call_start(CNTD_Call_t *call);
void call_end(CNTD_Call_t *call);

// tool.c
void open_msr(char *dev);
void close_msr();
uint64_t read_msr(int offset);
void write_msr(int offset, uint64_t value);
uint32_t diff_32(uint32_t end, uint32_t start);
uint64_t diff_48(uint64_t end, uint64_t start);
uint64_t diff_64(uint64_t end, uint64_t start);
int world_rank_2_local_rank(int rank, CNTD_Group_t* group);
struct timespec time_sum(struct timespec par_1, struct timespec par_2);
struct timespec time_diff(struct timespec par_1, struct timespec par_2);
double timespec2double(struct timespec timing);
void remove_spaces(char* str);
int mkpath(char *dir, mode_t mode);
int read_current_pstate();
int read_target_pstate();
void write_pstate();
void reset_pstate();
int read_tstate();
void write_tstate();
void reset_tstate();

// eam.c
void load_eamo_files(char *output_dir);
void call_back_eam(int signum);
void eam(CNTD_Call_t *call, int when);
void sched_next_eamo_conf(CNTD_Call_t *call, int when);

// log.c
void open_log_files(char *output_dir);
void close_log_files();
void print_call_info(char *log_dir);
void print_cntd_static_info();
void print_cntd_dynamic_info();
void print_group();
void print_comm();
void print_call(CNTD_Call_Phase_t phase);
void print_adv_metrics(CNTD_AdvMetrics_Phase_t data);
void print_custmetr();

// memory.c
CNTD_Comm_t* add_cntd_comm(MPI_Comm mpi_comm);
void check_mem_cntd_comm();
void check_mem_cntd_group();
CNTD_Group_t* lookup_cntd_group(MPI_Group group);
CNTD_Comm_t* lookup_cntd_comm(MPI_Comm comm);
CNTD_Call_t* add_cntd_call(MPI_Type_t mpi_type, MPI_Comm mpi_comm);

// monitor.c
uint64_t RDTSC();
uint64_t RDTSCP();
uint64_t RDPMC(unsigned pmc);
void CPUID(unsigned int *regs);
void add_timing(CNTD_Call_t *call, int when);
void add_network(CNTD_Call_t *call, 
	const int *send_count, MPI_Datatype *send_type, int dest, 
	const int *recv_count, MPI_Datatype *recv_type, int source);
void add_storage(CNTD_Call_t *call, 
	int read_count, MPI_Datatype read_datatype,
	int write_count, MPI_Datatype write_datatype);
void add_perf(CNTD_Call_t *call, int when);
uint64_t get_ref_tsc(uint64_t value);
void check_adv_metrics(CNTD_Call_t *call, int when);
void update_adv_metrics(CNTD_Call_t *call, int when);
void update_curr_call();
void update_cntd_advmetr(CNTD_AdvMetrics_Phase_t data);

// topology.c
void detect_topology();
int get_cpu_id();
int get_socket_id();
void read_rapl_units();
void enable_fix_ctr();
void enable_uncore_freq_ctr();

// calc.c
void update_cust_metr(CNTD_Call_Phase_t phase);
CNTD_Call_Phase_t calc_call();
CNTD_AdvMetrics_Phase_t calc_adv_metrics();

#endif // _CNTD_H_
