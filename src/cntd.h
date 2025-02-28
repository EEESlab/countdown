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

#ifndef __CNTD_H__
#define __CNTD_H__

#define _GNU_SOURCE
#include <stddef.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <sys/file.h> // for \"flock\".
#include <math.h> // for \"ceil\".
#include "cntd_common.h"

//TABLE
#include "fort.h"

// MPI
#include <mpi.h>

// hwloc
#include <hwloc.h>

// CNTD MPI Definitions
#include "cntd_mpi_def.h"

// General configurations
#define MAX_SAMPLING_TIME_REPORT 600 // 600 seconds (10 min)
#define DEFAULT_SAMPLING_TIME_REPORT 1 // 1 second
#define MAX_NUM_SOCKETS 16 // Max supported sockets in a single node
#define MAX_NUM_GPUS 16 // Max supported gpus in a single node
#define MAX_NUM_CPUS 8192 // Max supported CPUS in a single node

// EAM configurations
#define DEFAULT_TIMEOUT 0.0005 // 500us

#define MEM_SIZE 1024
#define STRING_SIZE 1024

// Filenames
#define SUMMARY_REPORT_FILE "cntd_summary_%s.csv"
#define RANK_REPORT_FILE "cntd_rank_%s.csv"
#define MPI_REPORT_FILE "cntd_mpi_%s.csv"
#define RANK_MPI_REPORT_FILE "cntd_rank_mpi_%s.csv"
#define EAM_REPORT_FILE "cntd_eam_%s.csv"
#define EAM_SLACK_REPORT_FILE "cntd_eam_slack_%s.csv"
#define TIME_SERIES_FILE "%s/cntd_%s_%s.csv"
#define SHM_FILE "/cntd_local_rank_%d.%s"
#define CNTD_TMP_DIR "/tmp"

// Hide symbols for external linking
#define HIDDEN __attribute__((visibility("hidden")))

// Constants
#define CNTD_MPI_TAG 666

#define FALSE 0
#define TRUE 1

#define APP 0
#define MPI 1

#define ENABLE_FREQ 2
#define DISABLE_FREQ 3
#define ONLY_TIMER 4

#define NO_CONF -1

#define CURR 0
#define MIN 0
#define MAX 1
#define DIFF 2

#define TOT 1

#define SEND 0
#define RECV 1

#define READ 0
#define WRITE 1

#define MPI_NONE -1000
#define MPI_ALL -2000
#define MPI_ALLV -3000
#define MPI_ALLW -4000

#define START 0
#define END 1
#define INIT 2

#define PKG 0
#define DRAM 1

#define POW_2_10 1024
#define POW_2_20 1048576
#define POW_2_30 1073741824
#define POW_2_40 1099511627776
#define POW_2_50 1125899906842624
#define POW_2_60 1152921504606846976

#define PERF_EVENT_0 0
#define PERF_EVENT_1 1
#define PERF_EVENT_2 2
#define PERF_EVENT_3 3
#define PERF_EVENT_4 4
#define PERF_EVENT_5 5
#define PERF_EVENT_6 6
#define PERF_EVENT_7 7
#ifdef CNTD_MAX_NUM_CUSTOM_PERF
#define MAX_NUM_CUSTOM_PERF CNTD_MAX_NUM_CUSTOM_PERF
#else
#define MAX_NUM_CUSTOM_PERF 8
#endif
#define PERF_INST_RET MAX_NUM_CUSTOM_PERF
#define PERF_CYCLES (MAX_NUM_CUSTOM_PERF + 1)
#define PERF_CYCLES_REF (MAX_NUM_CUSTOM_PERF + 2)

// \"cpufre\" files
#define CPUINFO_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"
#define CPUINFO_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
#define SCALING_MAX_FREQ \
	"/sys/devices/system/cpu/cpu%u/cpufreq/scaling_max_freq"
#define SCALING_MIN_FREQ \
	"/sys/devices/system/cpu/cpu%u/cpufreq/scaling_min_freq"
#define SCALING_GOVERNOR "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define SCALING_SETSPEED \
	"/sys/devices/system/cpu/cpu%u/cpufreq/scaling_setspeed"

#if (defined INTEL || defined AMD)
#include "x86_64.h"
#elif POWER9
#include "power9.h"
#elif THUNDERX2
#include "thunderx2.h"
#endif

// NVML
#ifdef NVIDIA_GPU
#include "nvidia.h"
#endif

#ifdef MOSQUITTO_ENABLED
#include "mosquitto_cntd.h"
#endif

typedef struct {
	int world_rank;
	int local_rank;

	int world_size;
	int local_size;

	char hostname[STRING_SIZE];
	int cpu_id;
	int pid;

	uint64_t num_sampling;

	double exe_time[2];
	double app_time[2];
	double mpi_time[2];

	long max_mem_usage;
	uint64_t mpi_net_data[2][2];
	uint64_t mpi_file_data[2][2];

	int access_shmem;
	int into_mpi;
	double timing_event_sample[2];

	uint64_t perf[MAX_NUM_PERF_EVENTS][2];
	uint64_t perf_te[MAX_NUM_PERF_EVENTS]
			[2]; // \"perf_te\" = \"perf time enabled\"
	uint64_t perf_tr[MAX_NUM_PERF_EVENTS]
			[2]; // \"perf_tr\" = \"perf time running\"
	double perf_tm[MAX_NUM_PERF_EVENTS]
		      [2]; // \"perf_tm\" = \"perf time multiplier\"
	uint64_t tsc[2];
	double load[2];

	uint64_t mpi_type_cnt[NUM_MPI_TYPE];
	double mpi_type_time[NUM_MPI_TYPE];
	uint64_t mpi_type_data[2][NUM_MPI_TYPE];

	uint64_t cntd_mpi_type_cnt[NUM_MPI_TYPE];
	double cntd_mpi_type_time[NUM_MPI_TYPE];
} CNTD_RankInfo_t;

typedef struct {
	char hostname[STRING_SIZE];
	unsigned int num_gpus;

	uint64_t util[MAX_NUM_GPUS]; // Percentage - counter (sample period may be between 1 second and 1/6 second)
	uint64_t util_mem
		[MAX_NUM_GPUS]; // Percentage - counter (sample period may be between 1 second and 1/6 second)

	uint64_t temp[MAX_NUM_GPUS]; // Celsius - counter
	uint64_t clock[MAX_NUM_GPUS]; // Clock in MHz - counter

	double energy[MAX_NUM_GPUS]; // Joules - counter
} CNTD_GPUInfo_t;

typedef struct {
	char hostname[STRING_SIZE];
	int num_sockets;
	int num_cores;
	int num_cpus;
	int num_gpus;

	// Energy
	double energy_sys; // Joules - counter
	double energy_pkg[MAX_NUM_SOCKETS]; // Joules - counter
	double energy_dram[MAX_NUM_SOCKETS]; // Joules - counter
	double energy_gpu[MAX_NUM_SOCKETS]; // Joules - counter - only for Power9
} CNTD_NodeInfo_t;

// Global variables
typedef struct {
	// User-defined values
	double eam_timeout;
	int sys_freq_khz[2];
	int user_freq_khz[2];
	double sampling_time;
	char log_dir[STRING_SIZE];
	char tmp_dir[STRING_SIZE];

	unsigned int use_cpufreq : 1;
	unsigned int force_msr : 1;
	unsigned int enable_eam : 1;
	unsigned int enable_eam_slack : 1;
	unsigned int enable_eam_analysis : 1;
	unsigned int enable_power_monitor : 1;
	unsigned int enable_timeseries_report : 1;
	unsigned int enable_report : 1;
	unsigned int enable_perf : 1;

	// CPUFREQ
	int cpuinfo_max_freq;
	int cpuinfo_min_freq;
	int scaling_max_freq_fd;
	int scaling_min_freq_fd;
	int scaling_setspeed_fd;

	// MPI Communicators
	MPI_Comm comm_local;
	MPI_Comm comm_masters;
	// MPI Ranks
	// World rank of node master
	int master_rank;
	int iam_master;

	// Runtime values
	timer_t timer;

	// Linux Perf
	int perf_fd[MAX_NUM_CPUS][MAX_NUM_PERF_EVENTS];

	CNTD_RankInfo_t *local_ranks[MAX_NUM_CPUS];
	CNTD_RankInfo_t *rank;
	CNTD_GPUInfo_t gpu;
	CNTD_NodeInfo_t node;

	// \"cpufreq\" values.
	char scaling_governor[STRING_SIZE];
	int userspace_governor;

	int nom_freq_mhz;
#if (defined INTEL || defined AMD)
	_Bool hwp_usage;
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

// HEADERS
// arch.c
void get_access(int var);
void release_access(int var);
void init_perf();
void finalize_perf();
void init_arch_conf();
//cpufreq
void init_cpufreq();
void finalize_cpufreq();
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
void set_freq(int freq_khz);
void set_user_max_freq();
void set_user_min_freq();
void set_sys_max_freq();
void set_sys_min_freq();
int get_maximum_turbo_frequency();
int get_minimum_frequency();
void pm_init();
void pm_finalize();
void write_msr(int offset, uint64_t value);
uint64_t read_msr(int offset);
// report.c
void print_final_report();
void init_timeseries_report();
void send_mosquitto_report(char *topic_ending, int local_rank,
			   double payload_value);
void print_timeseries_report(double time_curr, double time_prev,
			     double energy_sys, double *energy_pkg,
			     double *energy_dram, double *energy_gpu_sys,
			     double *energy_gpu, unsigned int *util,
			     unsigned int *util_mem, unsigned int *temp,
			     unsigned int *clock);
void finalize_timeseries_report();
// sampling.c
void time_sample_region(int prev, int curr, int init, double timing[2]);
void time_sample_net(int prev, int curr, int init);
void time_sample_file(int prev, int curr, int init);
void time_sample_perf(int prev, int curr, int init);
void time_sample_sys_energy(int prev, int curr, int init,
			    double energy_pkg[MAX_NUM_SOCKETS],
			    double energy_dram[MAX_NUM_SOCKETS],
			    double energy_sys_gpu[MAX_NUM_SOCKETS],
			    double *energy_sys);
void time_sample_gpu(int prev, int curr, int init,
		     unsigned int util_gpu[MAX_NUM_GPUS],
		     unsigned int util_mem_gpu[MAX_NUM_GPUS],
		     unsigned int temp_gpu[MAX_NUM_GPUS],
		     unsigned int clock_gpu[MAX_NUM_GPUS],
		     double energy_gpu[MAX_NUM_GPUS]);
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
int make_timer(timer_t *timerID, void (*func)(int, siginfo_t *, void *),
	       int interval, int expire);
int delete_timer(timer_t timerID);
// tool.c
void print_stack_trace();
int str_to_bool(const char str[]);
int read_str_from_file(char *filename, char *str);
int open_file(char *file_name, int flags);
void write_int_to_file(char *filename, int fd, int value);
int read_int_from_file(char *file_name);
double read_time();
uint64_t diff_overflow(uint64_t end, uint64_t start, uint64_t overflow);
int makedir(const char dir[]);
int copyFile(char *source, char *desitnation);
MPI_Datatype get_mpi_datatype_rank();
MPI_Datatype get_mpi_datatype_node();
MPI_Datatype get_mpi_datatype_gpu();
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu,
		     int group_fd, unsigned long flags);
HIDDEN CNTD_RankInfo_t *create_shmem_rank(const char shmem_name[],
					  int num_elem);
void destroy_shmem_cpu(CNTD_RankInfo_t *shmem_ptr, int num_elem,
		       const char shmem_name[]);
CNTD_RankInfo_t *get_shmem_cpu(const char shmem_name[], int num_elem);
// Add network count only collective and P2P primitives
void add_network(MPI_Comm comm, MPI_Type_t type, const int *send_count,
		 MPI_Datatype *send_type, int dest, const int *recv_count,
		 MPI_Datatype *recv_type, int source);
void add_file(MPI_Type_t type, int read_count, MPI_Datatype read_datatype,
	      int write_count, MPI_Datatype write_datatype);
void get_rand_postfix(char *postfix, int size);
int read_nom_freq();

#endif // __CNTD_H__
