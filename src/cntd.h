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
#include <sys/time.h>

// MPI
#include <mpi.h>

// CNTD MPI Definitions
#include "cntd_mpi_def.h"


#ifndef _CNTD_H_
#define	_CNTD_H_

#ifdef __INTEL_COMPILER
#pragma warning disable 1786
#else	 /// GNU build
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

// General configurations
#define DEFAULT_SAMPLING_TIME 600		// 10 min
#define NUM_SOCKETS 8					// Max intel sockets in a single node

// EAM configurations
#define DEFAULT_TIMEOUT 500				// 500us

#define MEM_SIZE 128
#define STRING_SIZE 128

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
	int force_msr;
	double sampling_time;

	// arch
	int num_sockets;

	// Runtime values
	double exe_time[2];
	timer_t timer;

	// PKG energy
	char energy_pkg_name[NUM_SOCKETS][STRING_SIZE];
	uint64_t energy_pkg[NUM_SOCKETS];
	uint64_t energy_pkg_overflow[NUM_SOCKETS];

	// DRAM energy
	char energy_dram_name[NUM_SOCKETS][STRING_SIZE];
	uint64_t energy_dram[NUM_SOCKETS];
	uint64_t energy_dram_overflow[NUM_SOCKETS];
} CNTD_t;

CNTD_t *cntd;

// HEADERS
// arch.c
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
void print_report();

// sampling.c
void make_sample(int sig, siginfo_t *siginfo, void *context);

// tool.c
int str_to_bool(const char str[]);
int read_str_from_file(char *filename, char *str);
double read_time();
int make_timer(timer_t *timerID, void (*func)(int, siginfo_t*, void*), int interval, int expire);
int delete_timer(timer_t timerID);
uint64_t diff_overflow(uint64_t end, uint64_t start, uint64_t overflow);

#endif // _CNTD_H_
