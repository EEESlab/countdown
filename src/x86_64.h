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

#ifndef __X86_64_H__
#define __X86_64_H__

#include <stdint.h>
#include "common_def.h"
#include "cntd_common.h"

#ifdef CNTD_MAX_NUM_MEM_CHANNELS_PER_SOCKET
#define MAX_NUM_MEM_CHANNELS_PER_SOCKET CNTD_MAX_NUM_MEM_CHANNELS_PER_SOCKET
#else
#define MAX_NUM_MEM_CHANNELS_PER_SOCKET 6
#endif

#define PERF_SCALAR_DOUBLE (MAX_NUM_CUSTOM_PERF + 3)
#define PERF_SCALAR_SINGLE (MAX_NUM_CUSTOM_PERF + 4)
#define PERF_128_PACKED_DOUBLE (MAX_NUM_CUSTOM_PERF + 5)
#define PERF_128_PACKED_SINGLE (MAX_NUM_CUSTOM_PERF + 6)
#define PERF_256_PACKED_DOUBLE (MAX_NUM_CUSTOM_PERF + 7)
#define PERF_256_PACKED_SINGLE (MAX_NUM_CUSTOM_PERF + 8)
#define PERF_512_PACKED_DOUBLE (MAX_NUM_CUSTOM_PERF + 9)
#define PERF_512_PACKED_SINGLE (MAX_NUM_CUSTOM_PERF + 10)
#define PERF_CAS_COUNT_ALL (MAX_NUM_CUSTOM_PERF + 11)

// INTEL SPECIFIC HACK. TODO: FIX IT IN A MORE GENERAL WAY!
//#define MAX_NUM_PERF_EVENTS				(MAX_NUM_CUSTOM_PERF + (MAX_NUM_MEM_CHANNELS_PER_SOCKET * 2)  - 1 + 12)	// Max supported perf events

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

#define INTEL_RAPL_PKG "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u"
#define INTEL_RAPL_PKG_NAME \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/name"
#define PKG_ENERGY_UJ \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/energy_uj"
#define PKG_MAX_ENERGY_RANGE_UJ \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/max_energy_range_uj"

#define INTEL_RAPL_DRAM \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u"
#define INTEL_RAPL_DRAM_NAME \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/name"
#define DRAM_ENERGY_UJ \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/energy_uj"
#define DRAM_MAX_ENERGY_RANGE_UJ \
	"/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%u/intel-rapl:%u:%u/max_energy_range_uj"

// MSRs
#define MSR_FILE "/dev/cpu/%u/msr"
#define MSRSAFE_FILE "/dev/cpu/%u/msr_safe"

// Intel HWP knobs
#define IA32_PM_ENABLE (0x770)
#define IA32_HWP_CAPABILITIES (0x771)
#define IA32_HWP_REQUEST_PKG (0x772)
#define IA32_HWP_INTERRUPT (0x773)
#define IA32_HWP_REQUEST (0x774)
#define IA32_HWP_PECI_REQUEST_INFO (0x775)
#define IA32_HWP_STATUS (0x777)
// Intel frequency knob
#define IA32_PERF_CTL (0x199)
#define MSR_TURBO_RATIO_LIMIT (0x1AD)

void init_rapl();
void finalize_rapl();
void perf_x_roofline(int i, uint32_t perf_event);
void perf_x_memory_roofline(int i, uint32_t perf_event);
void perf_open_roofline(struct perf_event_attr *perf_pe, int i, int pid,
			char *hostname, int world_rank);
void perf_enable_roofline(int i);
void perf_close_roofline(int i);
void perf_disable_roofline(int i);
void read_tsc(uint64_t *tsc);
// hwp.c
void set_max_epp();
void set_min_epp();
void set_max_aw();
void set_min_aw();
void time_sample_roofline(READ_FORMAT_t (*perf)[MAX_NUM_PERF_EVENTS][2], int i,
			  int flip);
#endif // __X86_64_H__
