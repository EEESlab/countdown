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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>

// MPI
#include <mpi.h>

// hwloc
#include <hwloc.h>

// NVML
#ifdef NVIDIA_GPU
#include <nvml.h>
#endif

#ifdef MOSQUITTO_ENABLED
#include "mosquitto.h"

//#define MQTT_HOST	   "localhost"
#define MQTT_HOST	   "137.204.213.192"
#define MQTT_KEEPALIVE 60
#define MQTT_PAYLOAD   "%f;%ld"
#define MQTT_PORT	   1883
#define MQTT_QOS	   0
#define MQTT_RETAIN	   0
#define MQTT_TOPIC	   "org/cineca/plugin/cntd_pub/job_id/%s/node/%s/cpu/%u/w_rank/%u/l_rank/%u/%s"
#endif

// CNTD MPI Definitions
#include "cntd_mpi_def.h"


#ifndef __CNTD_H__
#define	__CNTD_H__

// General configurations
#define MAX_SAMPLING_TIME_REPORT		600		// 600 seconds (10 min)
#define DEFAULT_SAMPLING_TIME_REPORT 	1		// 1 second
#define MAX_NUM_SOCKETS 				16		// Max supported sockets in a single node
#define MAX_NUM_GPUS 					16		// Max supported gpus in a single node
#define MAX_NUM_CPUS					1024	// Max supported CPUS in a single node

// EAM configurations
#define DEFAULT_TIMEOUT 				0.0005	// 500us

#define MEM_SIZE 						1024
#define STRING_SIZE 					1024

// Filenames
#define SUMMARY_REPORT_FILE 			"cntd_summary.csv"
#define RANK_REPORT_FILE				"cntd_rank.csv"
#define MPI_REPORT_FILE					"cntd_mpi.csv"
#define RANK_MPI_REPORT_FILE			"cntd_rank_mpi.csv"
#define EAM_REPORT_FILE					"cntd_eam.csv"
#define EAM_SLACK_REPORT_FILE			"cntd_eam_slack.csv"
#define TMP_TIME_SERIES_FILE			"%s/cntd_%s.%s.csv"
#define TIME_SERIES_FILE				"%s/cntd_%s.csv"
#define SHM_FILE						"/cntd_local_rank_%d.%s"

// Hide symbols for external linking
#define HIDDEN  __attribute__((visibility("hidden")))

// Constants
#define CNTD_MPI_TAG 					666

#define FALSE							0
#define TRUE 							1

#define APP 							0
#define MPI 							1

#define ENABLE_FREQ						2
#define DISABLE_FREQ					3
#define ONLY_TIMER						4

#define NO_CONF							-1

#define CURR 							0
#define MIN 							0
#define MAX 							1
#define DIFF 							2

#define TOT 							1

#define SEND 							0
#define RECV 							1

#define READ 							0
#define WRITE 							1

#define MPI_NONE 						-1000
#define MPI_ALL  						-2000
#define MPI_ALLV 						-3000
#define MPI_ALLW 						-4000

#define START 							0
#define END 							1
#define INIT 							2

#define PKG  							0
#define DRAM 							1

#define POW_2_10  						1024
#define POW_2_20  						1048576
#define POW_2_30  						1073741824
#define POW_2_40  						1099511627776
#define POW_2_50  						1125899906842624
#define POW_2_60  						1152921504606846976

#define PERF_EVENT_0 					0
#define PERF_EVENT_1 					1
#define PERF_EVENT_2 					2
#define PERF_EVENT_3 					3
#define PERF_EVENT_4 					4
#define PERF_EVENT_5 					5
#define PERF_EVENT_6 					6
#define PERF_EVENT_7 					7
#ifdef CNTD_MAX_NUM_CUSTOM_PERF
#define MAX_NUM_CUSTOM_PERF             CNTD_MAX_NUM_CUSTOM_PERF
#else
#define MAX_NUM_CUSTOM_PERF				8
#endif
#define PERF_INST_RET 					MAX_NUM_CUSTOM_PERF
#define PERF_CYCLES 					(MAX_NUM_CUSTOM_PERF + 1)
#define PERF_CYCLES_REF					(MAX_NUM_CUSTOM_PERF + 2)

#define MAX_NUM_PERF_EVENTS				(MAX_NUM_CUSTOM_PERF + 3)	// Max supported perf events

// The libpfm4 library can be used to translate from
// the name in the architectural manuals to the raw hex value
// perf_event_open() expects in this field.
// https://github.com/wcohen/libpfm4

// Typical 		attributes on a x86 platform 32bit
// 
// event		8: Set the first 8 bit event code (required)
// umask		8: Set the 8 bit umask. Event code and umask together select a
// 				hardware event.
// cmask		8: Set the 8 bit counter Mask. Only increment counters when at
// 				least cmask events happen during the same cycle.
// inv			1: (1bit flag) Invert the cmask condition. Only valid with
// 				cmask>0.
// edge			1: (1bit flag) Only increment the event when the condition
// 				changes (starts happening)
// any			1: (1bit flag) Count on both threads of a core
// pc			1: (1bit flag) Toggle the PMi pins when the condition happens

// System files
#define CPUINFO_MIN_FREQ 				"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
#ifdef USERSPACE_GOV
#define CPUINFO_MAX_FREQ				"/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#else
#define CPUINFO_MAX_FREQ 				"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
#endif
#define CUR_CPUINFO_MIN_FREQ			"/sys/devices/system/cpu/cpu%u/cpufreq/scaling_min_freq"
#define CUR_CPUINFO_MAX_FREQ			"/sys/devices/system/cpu/cpu%u/cpufreq/scaling_max_freq"

#ifdef INTEL	

#define INTEL_RAPL_PKG 					"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u"
#define INTEL_RAPL_PKG_NAME 			"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/name"
#define PKG_ENERGY_UJ 					"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/energy_uj"
#define PKG_MAX_ENERGY_RANGE_UJ 		"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/max_energy_range_uj"

#define INTEL_RAPL_DRAM 				"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u"
#define INTEL_RAPL_DRAM_NAME 			"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/name"
#define DRAM_ENERGY_UJ 					"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/energy_uj"
#define DRAM_MAX_ENERGY_RANGE_UJ		"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/max_energy_range_uj"

// MSRs	
#define MSR_FILE 						"/dev/cpu/%u/msr"
#define MSRSAFE_FILE 					"/dev/cpu/%u/msr_safe"

#ifdef HWP_AVAIL
// Intel HWP knobs
#define IA32_PM_ENABLE                  (0x770)
#define IA32_HWP_CAPABILITIES           (0x771)
#define IA32_HWP_REQUEST_PKG            (0x772)
#define IA32_HWP_INTERRUPT              (0x773)
#define IA32_HWP_REQUEST                (0x774)
#define IA32_HWP_PECI_REQUEST_INFO      (0x775)
#define IA32_HWP_STATUS                 (0x777)
#endif
// Intel frequency knob	
#define IA32_PERF_CTL 					(0x199)
#define MSR_TURBO_RATIO_LIMIT			(0x1AD)

#elif POWER9	

#define OCC_INBAND_SENSORS 				"/sys/firmware/opal/exports/occ_inband_sensors"

#define MAX_OCCS						8
#define MAX_CHARS_SENSOR_NAME			16
#define MAX_CHARS_SENSOR_UNIT			4

#define OCC_SENSOR_DATA_BLOCK_OFFSET	0x00580000
#define OCC_SENSOR_DATA_BLOCK_SIZE		0x00025800

enum occ_sensor_type {
	OCC_SENSOR_TYPE_GENERIC	= 			0x0001,
	OCC_SENSOR_TYPE_CURRENT	= 			0x0002,
	OCC_SENSOR_TYPE_VOLTAGE	= 			0x0004,
	OCC_SENSOR_TYPE_TEMPERATURE	= 		0x0008,
	OCC_SENSOR_TYPE_UTILIZATION	= 		0x0010,
	OCC_SENSOR_TYPE_TIME = 				0x0020,
	OCC_SENSOR_TYPE_FREQUENCY = 		0x0040,
	OCC_SENSOR_TYPE_POWER = 			0x0080,
	OCC_SENSOR_TYPE_PERFORMANCE	= 		0x0200,
};

enum occ_sensor_location {
	OCC_SENSOR_LOC_SYSTEM = 			0x0001,
	OCC_SENSOR_LOC_PROCESSOR = 			0x0002,
	OCC_SENSOR_LOC_PARTITION = 			0x0004,
	OCC_SENSOR_LOC_MEMORY = 			0x0008,
	OCC_SENSOR_LOC_VRM = 				0x0010,
	OCC_SENSOR_LOC_OCC = 				0x0020,
	OCC_SENSOR_LOC_CORE = 				0x0040,
	OCC_SENSOR_LOC_GPU = 				0x0080,
	OCC_SENSOR_LOC_QUAD = 				0x0100,
};

enum sensor_struct_type {
	OCC_SENSOR_READING_FULL = 			0x01,
	OCC_SENSOR_READING_COUNTER = 		0x02,
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

#define PATH_T99MON_NODE0     			"/sys/devices/platform/tx2mon/node0_raw"
#define PATH_T99MON_NODE1     			"/sys/devices/platform/tx2mon/node1_raw"
#define PATH_T99MON_SOCINFO   			"/sys/devices/platform/tx2mon/socinfo"

#define MAX_CPUS_PER_SOC 32

// for cmd_status below
#define CMD_STATUS_READY(cmd) 			(((cmd) >> 1 ) & 1)
#define CMD_VERSION(cmd) 				(((cmd) >> 24) & 0xff)

// MC val to celsius
#define to_c(val)						((446.18 + 7.92) - ((val) * 0.5582))

// MC operating region layout
typedef struct
{
    uint32_t cmd_status;
    uint32_t counter;
    uint32_t resv0;
    uint32_t temp_abs_max;
    uint32_t temp_soft_thresh;
    uint32_t temp_hard_thresh;
    uint32_t resv1;
    uint32_t resv2;
    uint32_t freq_cpu[MAX_CPUS_PER_SOC];
    int32_t	resv3[MAX_CPUS_PER_SOC];
    uint16_t tmon_cpu[MAX_CPUS_PER_SOC];
    uint32_t tmon_soc_avg;
    uint32_t freq_mem_net;
    uint32_t freq_socs;
    uint32_t freq_socn;
    uint32_t freq_max;
    uint32_t freq_min;
    uint32_t pwr_core;
    uint32_t pwr_sram;
    uint32_t pwr_mem;
    uint32_t pwr_soc;
    uint32_t v_core;
    uint32_t v_sram;
    uint32_t v_mem;
    uint32_t v_soc;
    uint32_t resv4;
    uint32_t resv5;
    uint32_t resv6;
    uint32_t resv7;
    uint32_t resv8;
    uint32_t resv9;
    uint32_t resv10;
    uint32_t resv11;
    uint32_t resv12;
    uint32_t resv13;
    uint32_t resv14;
    uint32_t active_evt;
    uint32_t temp_evt_cnt;
    uint32_t pwr_evt_cnt;
    uint32_t ext_evt_cnt;
    uint32_t pwr_throttle_ms;
    uint32_t ext_throttle_ms;
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
	int world_rank;
	int local_rank;

	char hostname[STRING_SIZE];
	int cpu_id;
	int pid;

	int exe_is_started;

	uint64_t num_sampling;

	double exe_time[2];
	double app_time[2];
	double mpi_time[2];

	long max_mem_usage;
	uint64_t mpi_net_data[2][2];
	uint64_t mpi_file_data[2][2];

	uint64_t perf[MAX_NUM_PERF_EVENTS][2];

	uint64_t mpi_type_cnt[NUM_MPI_TYPE];
	double mpi_type_time[NUM_MPI_TYPE];
	uint64_t mpi_type_data[2][NUM_MPI_TYPE];

	uint64_t cntd_mpi_type_cnt[NUM_MPI_TYPE];
	double cntd_mpi_type_time[NUM_MPI_TYPE];
} CNTD_RankInfo_t;

typedef struct
{
	char hostname[STRING_SIZE];
	unsigned int num_gpus;

	uint64_t util[MAX_NUM_GPUS];			// Percentage - counter (sample period may be between 1 second and 1/6 second)
	uint64_t util_mem[MAX_NUM_GPUS];		// Percentage - counter (sample period may be between 1 second and 1/6 second)

	uint64_t temp[MAX_NUM_GPUS];			// Celsius - counter
	uint64_t clock[MAX_NUM_GPUS];			// Clock in MHz - counter 

	double energy[MAX_NUM_GPUS];			// Joules - counter
} CNTD_GPUInfo_t;

typedef struct
{
	char hostname[STRING_SIZE];
	int num_sockets;
	int num_cores;
	int num_cpus;
	int num_gpus;

	// Energy
	double energy_sys;						// Joules - counter
	double energy_pkg[MAX_NUM_SOCKETS];		// Joules - counter
	double energy_dram[MAX_NUM_SOCKETS];	// Joules - counter
	double energy_gpu[MAX_NUM_SOCKETS];		// Joules - counter - only for Power9
} CNTD_NodeInfo_t;

// Global variables
typedef struct
{
	// User-defined values
	double eam_timeout;
	int sys_pstate[2];
	int user_pstate[2];
	double sampling_time;
	char log_dir[STRING_SIZE];
	char tmp_dir[STRING_SIZE];

	unsigned int force_msr:1;
	unsigned int enable_cntd:1;
	unsigned int enable_cntd_slack:1;
	unsigned int enable_eam_freq:1;
	unsigned int enable_power_monitor:1;
	unsigned int enable_timeseries_report:1;
	unsigned int enable_report:1;
	unsigned int enable_perf:1;

	MPI_Comm comm_local;
	MPI_Comm comm_local_masters;
	int local_rank_size;

	unsigned int into_mpi:1;

	// Runtime values
	timer_t timer;

	// Linux Perf
	int perf_fd[MAX_NUM_CPUS][MAX_NUM_PERF_EVENTS];

	CNTD_RankInfo_t *local_ranks[MAX_NUM_CPUS];
	CNTD_RankInfo_t *rank;
#ifdef NVIDIA_GPU
	CNTD_GPUInfo_t gpu;
#endif
	CNTD_NodeInfo_t node;

#ifdef INTEL
	int nom_freq_mhz;
	int msr_fd;
	int energy_pkg_fd[MAX_NUM_SOCKETS];
	double energy_pkg_overflow[MAX_NUM_SOCKETS];
	int energy_dram_fd[MAX_NUM_SOCKETS];
	double energy_dram_overflow[MAX_NUM_SOCKETS];
#elif POWER9
	int occ_fd;
#elif THUNDERX2
	tx2mon_t tx2mon;
#endif
#ifdef NVIDIA_GPU
	nvmlDevice_t gpu_device[MAX_NUM_GPUS];
#endif
} CNTD_t;

extern CNTD_t *cntd;

extern _Bool hwp_usage;

#ifdef MOSQUITTO_ENABLED
typedef struct mosquitto MOSQUITTO_t;

extern MOSQUITTO_t* mosq;
#endif

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
void init_perf();
void finalize_perf();
void init_arch_conf();

// init.c
void start_cntd();
void stop_cntd();
void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr);

// eam.c
void eam_start_mpi();
int eam_end_mpi();
void eam_init();
void eam_finalize();

// eam_slack.c
void eam_slack_start_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
int eam_slack_end_mpi(MPI_Type_t mpi_type, MPI_Comm comm, int addr);
void eam_slack_init();
void eam_slack_finalize();

// pm.c
void set_pstate(int pstate);
void set_max_pstate();
void set_min_pstate();
int get_maximum_turbo_frequency();
int get_minimum_frequency();
void pm_init();
void pm_finalize();
void write_msr(int offset, uint64_t value);
uint64_t read_msr(int offset);

// hwp.c
void set_max_epp();
void set_min_epp();
void set_max_aw();
void set_min_aw();

// report.c
void print_final_report();
void init_timeseries_report();
void send_mosquitto_report(char* topic_ending,
						   int local_rank	 ,
						   double payload_value);
void print_timeseries_report(
	double time_curr, double time_prev, 
	double energy_sys, 
	double *energy_pkg, double *energy_dram, 
	double *energy_gpu_sys, double *energy_gpu,
	unsigned int *util, unsigned int *util_mem, 
	unsigned int *temp, unsigned int *clock);
void finalize_timeseries_report();

// sampling.c
void event_sample_start(MPI_Type_t mpi_type);
void event_sample_end(MPI_Type_t mpi_type, int eam);
void init_time_sample();
void finalize_time_sample();
void time_sample(int sig, siginfo_t *siginfo, void *context);

// timer.c
void start_timer();
void reset_timer();
void init_timer();
void finalize_timer();
int make_timer(timer_t *timerID, void (*func)(int, siginfo_t*, void*), int interval, int expire);
int delete_timer(timer_t timerID);

// tool.c
int str_to_bool(const char str[]);
int read_str_from_file(char *filename, char *str);
int write_int_to_file(char* filename, int value);
double read_time();
uint64_t diff_overflow(uint64_t end, uint64_t start, uint64_t overflow);
int makedir(const char dir[]);
int copyFile(char *source, char *desitnation);
MPI_Datatype get_mpi_datatype_rank();
MPI_Datatype get_mpi_datatype_node();
MPI_Datatype get_mpi_datatype_gpu();
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags);
HIDDEN CNTD_RankInfo_t* create_shmem_rank(const char shmem_name[], int num_elem);
void destroy_shmem_cpu(CNTD_RankInfo_t *shmem_ptr, int num_elem, const char shmem_name[]);
CNTD_RankInfo_t* get_shmem_cpu(const char shmem_name[], int num_elem);
// Add network count only collective and P2P primitives
void add_network(MPI_Comm comm, MPI_Type_t type,
    const int *send_count, MPI_Datatype *send_type, int dest,
	const int *recv_count, MPI_Datatype *recv_type, int source);
void add_file(MPI_Type_t type,
	int read_count, MPI_Datatype read_datatype,
	int write_count, MPI_Datatype write_datatype);
void get_rand_postfix(char *postfix, int size);
#ifdef INTEL
int read_intel_nom_freq();
#endif

#endif // __CNTD_H__
