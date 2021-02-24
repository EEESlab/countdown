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
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

// MPI
#include <mpi.h>

// NVML
#ifdef CNTD_ENABLE_CUDA
#include <nvml.h>
#endif

// CNTD MPI Definitions
#include "cntd_mpi_def.h"


#ifndef __CNTD_H__
#define	__CNTD_H__

#ifdef __INTEL_COMPILER
#pragma warning disable 1786
#else	 /// GNU build
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
#define CPUINFO_MIN_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq"
#define CPUINFO_MAX_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"

#define INTEL_RAPL_PKG "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d"
#define INTEL_RAPL_PKG_NAME "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/name"
#define PKG_ENERGY_UJ "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/energy_uj"
#define PKG_MAX_ENERGY_RANGE_UJ "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/max_energy_range_uj"

#define INTEL_RAPL_DRAM "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d"
#define INTEL_RAPL_DRAM_NAME "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d/name"
#define DRAM_ENERGY_UJ "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d/energy_uj"
#define DRAM_MAX_ENERGY_RANGE_UJ "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d/max_energy_range_uj"

#define PACKAGE_ID "/sys/devices/system/cpu/cpu%d/topology/physical_package_id"

// MSRs
#define MSR_FILE "/dev/cpu/%d/msr"
#define MSRSAFE_FILE "/dev/cpu/%d/msr_safe"

// Intel frequency knob
#define IA32_PERF_CTL (0x199)

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

	double exe_time[2];

	// PKG energy
	char energy_pkg_file[MAX_NUM_SOCKETS][STRING_SIZE];
	double energy_pkg[MAX_NUM_SOCKETS];						// Micro joule
	double energy_pkg_overflow[MAX_NUM_SOCKETS];

	// DRAM energy
	char energy_dram_file[MAX_NUM_SOCKETS][STRING_SIZE];
	double energy_dram[MAX_NUM_SOCKETS];					// Micro joule
	double energy_dram_overflow[MAX_NUM_SOCKETS];

	// GPU energy
	double energy_gpu[MAX_NUM_GPUS];						// Milli joule
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
	int iam_local_master;

	// Runtime values
	timer_t timer;
	uint64_t num_sampling;

	CNTD_NodeInfo_t node;
	CNTD_CPUInfo_t cpu;
#ifdef CNTD_ENABLE_CUDA
	nvmlDevice_t gpu[MAX_NUM_GPUS];
#endif
} CNTD_t;

CNTD_t *cntd;

// HEADERS
// arch.c
void init_arch_conf();
void finalize_arch_conf();

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
void print_timeseries_report(double time_curr, double time_prev, uint64_t *energy_pkg, uint64_t *energy_dram, uint64_t *energy_gpu_diff);
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
