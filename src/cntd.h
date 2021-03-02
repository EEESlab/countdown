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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

// MPI
#include <mpi.h>

// NVML
#ifdef NVIDIA_GPU
#include <nvml.h>
#endif

// CNTD MPI Definitions
#include "cntd_mpi_def.h"


#ifndef __CNTD_H__
#define	__CNTD_H__

#ifdef __INTEL_COMPILER
#pragma warning disable 1786
#else	 // GNU build
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

// General configurations
#define DEFAULT_SAMPLING_TIME 600		// 10 minutes
#define DEFAULT_SAMPLING_TIME_REPORT 1	// 1 second
#define MAX_NUM_SOCKETS 8				// Max supported sockets in a single node
#define MAX_NUM_GPUS 16					// Max supported gpus in a single node

// EAM configurations
#define DEFAULT_TIMEOUT 500				// 500us

#define MEM_SIZE 128
#define STRING_SIZE 128

// Hide symbols for external linking
#define HIDDEN  __attribute__((visibility("hidden")))

// Constants
#define FALSE 0
#define TRUE 1

#define NO_CONF -1

#define CURR 0

#define MIN 0
#define MAX 1

#define MPI_NONE -1
#define MPI_ALL -2
#define MPI_ALLV -3
#define MPI_ALLW -4

#define START 0
#define END 1
#define INIT 2

#define PKG 0
#define DRAM 1

// System files
#define CORE_SIBLINGS_LIST 			"/sys/devices/system/cpu/cpu%u/topology/core_siblings_list"
#define PACKAGE_ID 					"/sys/devices/system/cpu/cpu%u/topology/physical_package_id"
#define CPUINFO_MIN_FREQ 			"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
#define CPUINFO_MAX_FREQ 			"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"

#ifdef INTEL

#define INTEL_RAPL_PKG 				"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u"
#define INTEL_RAPL_PKG_NAME 		"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/name"
#define PKG_ENERGY_UJ 				"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/energy_uj"
#define PKG_MAX_ENERGY_RANGE_UJ 	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/max_energy_range_uj"

#define INTEL_RAPL_DRAM 			"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u"
#define INTEL_RAPL_DRAM_NAME 		"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/name"
#define DRAM_ENERGY_UJ 				"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/energy_uj"
#define DRAM_MAX_ENERGY_RANGE_UJ	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/max_energy_range_uj"

// MSRs
#define MSR_FILE 					"/dev/cpu/%u/msr"
#define MSRSAFE_FILE 				"/dev/cpu/%u/msr_safe"

// Intel frequency knob
#define IA32_PERF_CTL (0x199)

#elif POWER9

#define OCC_INBAND_SENSORS 			"/sys/firmware/opal/exports/occ_inband_sensors"

#define MAX_OCCS					 8
#define MAX_CHARS_SENSOR_NAME		16
#define MAX_CHARS_SENSOR_UNIT		 4

#define OCC_SENSOR_DATA_BLOCK_OFFSET	0x00580000
#define OCC_SENSOR_DATA_BLOCK_SIZE		0x00025800

enum occ_sensor_type {
	OCC_SENSOR_TYPE_GENERIC		= 0x0001,
	OCC_SENSOR_TYPE_CURRENT		= 0x0002,
	OCC_SENSOR_TYPE_VOLTAGE		= 0x0004,
	OCC_SENSOR_TYPE_TEMPERATURE	= 0x0008,
	OCC_SENSOR_TYPE_UTILIZATION	= 0x0010,
	OCC_SENSOR_TYPE_TIME		= 0x0020,
	OCC_SENSOR_TYPE_FREQUENCY	= 0x0040,
	OCC_SENSOR_TYPE_POWER		= 0x0080,
	OCC_SENSOR_TYPE_PERFORMANCE	= 0x0200,
};

enum occ_sensor_location {
	OCC_SENSOR_LOC_SYSTEM		= 0x0001,
	OCC_SENSOR_LOC_PROCESSOR	= 0x0002,
	OCC_SENSOR_LOC_PARTITION	= 0x0004,
	OCC_SENSOR_LOC_MEMORY		= 0x0008,
	OCC_SENSOR_LOC_VRM			= 0x0010,
	OCC_SENSOR_LOC_OCC			= 0x0020,
	OCC_SENSOR_LOC_CORE			= 0x0040,
	OCC_SENSOR_LOC_GPU			= 0x0080,
	OCC_SENSOR_LOC_QUAD			= 0x0100,
};

enum sensor_struct_type {
	OCC_SENSOR_READING_FULL		= 0x01,
	OCC_SENSOR_READING_COUNTER	= 0x02,
};

typedef struct {
	uint8_t valid;
	uint8_t version;
	uint16_t nr_sensors;
	uint8_t reading_version;
	uint8_t pad[3];
	uint32_t names_offset;
	uint8_t names_version;
	uint8_t name_length;
	uint16_t reserved;
	uint32_t reading_ping_offset;
	uint32_t reading_pong_offset;
} __attribute__((__packed__)) occ_sensor_data_header_t;

typedef struct {
	char name[MAX_CHARS_SENSOR_NAME];
	char units[MAX_CHARS_SENSOR_UNIT];
	uint16_t gsid;
	uint32_t freq;
	uint32_t scale_factor;
	uint16_t type;
	uint16_t location;
	uint8_t structure_type;
	uint32_t reading_offset;
	uint8_t sensor_data;
	uint8_t pad[8];
} __attribute__((__packed__)) occ_sensor_name_t;

typedef struct {
	uint16_t gsid;
	uint64_t timestamp;
	uint16_t sample;
	uint16_t sample_min;
	uint16_t sample_max;
	uint16_t csm_min;
	uint16_t csm_max;
	uint16_t profiler_min;
	uint16_t profiler_max;
	uint16_t job_scheduler_min;
	uint16_t job_scheduler_max;
	uint64_t accumulator;
	uint32_t update_tag;
	uint8_t pad[8];
} __attribute__((__packed__)) occ_sensor_record_t;

typedef struct {
	uint16_t gsid;
	uint64_t timestamp;
	uint64_t accumulator;
	uint8_t sample;
	uint8_t pad[5];
} __attribute__((__packed__)) occ_sensor_counter_t;

enum sensor_attr {
	SENSOR_SAMPLE,
	SENSOR_ACCUMULATOR,
};

#define TO_FP(f)    ((f >> 8) * pow(10, ((int8_t)(f & 0xFF))))

#elif THUNDERX2

#define PATH_T99MON_NODE0     "/sys/devices/platform/tx2mon/node0_raw"
#define PATH_T99MON_NODE1     "/sys/devices/platform/tx2mon/node1_raw"
#define PATH_T99MON_SOCINFO   "/sys/devices/platform/tx2mon/socinfo"

#define MAX_CPUS_PER_SOC 32

// for cmd_status below
#define CMD_STATUS_READY(cmd) 	(((cmd) >> 1 ) & 1)
#define CMD_VERSION(cmd) 		(((cmd) >> 24) & 0xff)

// MC val to celsius
#define to_c(val)	((446.18 + 7.92) - ((val) * 0.5582))

// MC operating region layout
typedef struct
{
    uint32_t	cmd_status;
    uint32_t	counter;
    uint32_t	resv0;
    uint32_t	temp_abs_max;
    uint32_t	temp_soft_thresh;
    uint32_t	temp_hard_thresh;
    uint32_t	resv1;
    uint32_t	resv2;
    uint32_t	freq_cpu[MAX_CPUS_PER_SOC];
    int32_t		resv3[MAX_CPUS_PER_SOC];
    uint16_t	tmon_cpu[MAX_CPUS_PER_SOC];
    uint32_t	tmon_soc_avg;
    uint32_t	freq_mem_net;
    uint32_t	freq_socs;
    uint32_t	freq_socn;
    uint32_t	freq_max;
    uint32_t	freq_min;
    uint32_t	pwr_core;
    uint32_t	pwr_sram;
    uint32_t	pwr_mem;
    uint32_t	pwr_soc;
    uint32_t	v_core;
    uint32_t	v_sram;
    uint32_t	v_mem;
    uint32_t	v_soc;
    uint32_t	resv4;
    uint32_t	resv5;
    uint32_t	resv6;
    uint32_t	resv7;
    uint32_t	resv8;
    uint32_t	resv9;
    uint32_t	resv10;
    uint32_t	resv11;
    uint32_t	resv12;
    uint32_t	resv13;
    uint32_t	resv14;
    uint32_t 	active_evt;
    uint32_t 	temp_evt_cnt;
    uint32_t 	pwr_evt_cnt;
    uint32_t 	ext_evt_cnt;
    uint32_t 	pwr_throttle_ms;
    uint32_t 	ext_throttle_ms;
} mc_oper_region_t;

typedef struct {
	char *cl;
	char *nl;
} term_seq_t;

typedef struct {
	int fd;
	int	cores;
	int	node;
	mc_oper_region_t buf;
	unsigned int throttling_available:1;
} node_data_t;

typedef struct {
	int	nodes;
	node_data_t node[2];
} tx2mon_t;

#endif

typedef struct
{
	char hostname[STRING_SIZE];
	unsigned int cpu_id;
	unsigned int socket_id;

	double exe_time[2];
	double app_time;
	double mpi_time;

	uint64_t mpi_type_cnt[NUM_MPI_TYPE];
	double mpi_type_time[NUM_MPI_TYPE];
} CNTD_CPUInfo_t;

typedef struct
{
	char hostname[STRING_SIZE];
	unsigned int num_sockets;
	unsigned int num_cpus;
	unsigned int num_gpus;

	double exe_time[2];						// Seconds

	// Energy
	double energy_sys;						// Joules
	double energy_pkg[MAX_NUM_SOCKETS];		// Joules
	double energy_dram[MAX_NUM_SOCKETS];	// Joules
	double energy_gpu[MAX_NUM_GPUS];		// Joules
} CNTD_NodeInfo_t;

// Global variables
typedef struct
{
	// User-defined values
	uint64_t timeout;
	int sys_pstate[2];
	int user_pstate[2];
	int enable_cntd;
	int enable_cntd_slack;
	int no_p2p;
	int no_freq;
	int timeseries_report;
	int force_msr;
	double sampling_time;
	char log_dir[STRING_SIZE];

	MPI_Comm comm_local;
	MPI_Comm comm_local_masters;
	int iam_local_master;

	// Runtime values
	timer_t timer;
	uint64_t num_sampling;

	CNTD_NodeInfo_t node;
	CNTD_CPUInfo_t cpu;

#ifdef NVIDIA_GPU
	nvmlDevice_t gpu[MAX_NUM_GPUS];
#endif

#ifdef INTEL
	int energy_pkg_fd[MAX_NUM_SOCKETS];
	double energy_pkg_overflow[MAX_NUM_SOCKETS];
	int energy_dram_fd[MAX_NUM_SOCKETS];
	double energy_dram_overflow[MAX_NUM_SOCKETS];
#elif POWER9
	int occ_fd;
#elif THUNDERX2
	tx2mon_t tx2mon;
#endif
} CNTD_t;

CNTD_t *cntd;

// HEADERS
// arch.c
#ifdef INTEL
void init_rapl();
void finalize_rapl();
#elif POWER9
void init_occ();
void finalize_occ();
#elif THUNDERX2
void init_tx2mon();
void finalize_tx2mon();
#endif
#ifdef NVIDIA_GPU
void init_nvml();
void finalize_nvml();
#endif
void init_arch_conf();

// init.c
void start_cntd();
void stop_cntd();
void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr);

// eam_slack.c
void eam_slack_start_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void eam_slack_end_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void eam_slack_init();
void eam_slack_finalize();

// pm.c
void set_pstate(int pstate);
void set_max_pstate();
void set_min_pstate();
void pm_init();
void pm_finalize();

// eam.c
void eam_start_mpi();
void eam_end_mpi();
void eam_init();
void eam_finalize();

// report.c
void print_final_report();
void init_timeseries_report();
void print_timeseries_report(double time_curr, double time_prev, double energy_sys, double *energy_pkg, double *energy_dram, double *energy_gpu_diff);
void finalize_timeseries_report();

// sampling.c
void event_sample(MPI_Type_t mpi_type,int phase);
void time_sample(int sig, siginfo_t *siginfo, void *context);

// tool.c
int str_to_bool(const char str[]);
int read_str_from_file(char *filename, char *str);
double read_time();
int make_timer(timer_t *timerID, void (*func)(int, siginfo_t*, void*), int interval, int expire);
int delete_timer(timer_t timerID);
uint64_t diff_overflow(uint64_t end, uint64_t start, uint64_t overflow);
void makedir(const char dir[]);
MPI_Datatype get_mpi_datatype_cpu();
MPI_Datatype get_mpi_datatype_node();

#endif // __CNTD_H__
