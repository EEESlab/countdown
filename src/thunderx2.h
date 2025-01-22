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

#ifndef _THUNDERX2_H_
#define _THUNDERX2_H_

#include <stdint.h>
#define PATH_T99MON_NODE0 "/sys/devices/platform/tx2mon/node0_raw"
#define PATH_T99MON_NODE1 "/sys/devices/platform/tx2mon/node1_raw"
#define PATH_T99MON_SOCINFO "/sys/devices/platform/tx2mon/socinfo"

#define MAX_CPUS_PER_SOC 32

// for cmd_status below
#define CMD_STATUS_READY(cmd) (((cmd) >> 1) & 1)
#define CMD_VERSION(cmd) (((cmd) >> 24) & 0xff)

// MC val to celsius
#define to_c(val) ((446.18 + 7.92) - ((val) * 0.5582))

// MC operating region layout
typedef struct {
	uint32_t cmd_status;
	uint32_t counter;
	uint32_t resv0;
	uint32_t temp_abs_max;
	uint32_t temp_soft_thresh;
	uint32_t temp_hard_thresh;
	uint32_t resv1;
	uint32_t resv2;
	uint32_t freq_cpu[MAX_CPUS_PER_SOC];
	int32_t resv3[MAX_CPUS_PER_SOC];
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
	int cores;
	int node;
	mc_oper_region_t buf;
	unsigned int throttling_available : 1;
} node_data_t;

typedef struct {
	int nodes;
	node_data_t node[2];
} tx2mon_t;

void init_tx2mon();
void finalize_tx2mon();

#endif
