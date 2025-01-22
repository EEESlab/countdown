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

#ifndef __POWER9_H__
#define __POWER9_H__

#include <stdint.h>
#define OCC_INBAND_SENSORS "/sys/firmware/opal/exports/occ_inband_sensors"

#define MAX_OCCS 8
#define MAX_CHARS_SENSOR_NAME 16
#define MAX_CHARS_SENSOR_UNIT 4

#define OCC_SENSOR_DATA_BLOCK_OFFSET 0x00580000
#define OCC_SENSOR_DATA_BLOCK_SIZE 0x00025800

enum occ_sensor_type {
	OCC_SENSOR_TYPE_GENERIC = 0x0001,
	OCC_SENSOR_TYPE_CURRENT = 0x0002,
	OCC_SENSOR_TYPE_VOLTAGE = 0x0004,
	OCC_SENSOR_TYPE_TEMPERATURE = 0x0008,
	OCC_SENSOR_TYPE_UTILIZATION = 0x0010,
	OCC_SENSOR_TYPE_TIME = 0x0020,
	OCC_SENSOR_TYPE_FREQUENCY = 0x0040,
	OCC_SENSOR_TYPE_POWER = 0x0080,
	OCC_SENSOR_TYPE_PERFORMANCE = 0x0200,
};

enum occ_sensor_location {
	OCC_SENSOR_LOC_SYSTEM = 0x0001,
	OCC_SENSOR_LOC_PROCESSOR = 0x0002,
	OCC_SENSOR_LOC_PARTITION = 0x0004,
	OCC_SENSOR_LOC_MEMORY = 0x0008,
	OCC_SENSOR_LOC_VRM = 0x0010,
	OCC_SENSOR_LOC_OCC = 0x0020,
	OCC_SENSOR_LOC_CORE = 0x0040,
	OCC_SENSOR_LOC_GPU = 0x0080,
	OCC_SENSOR_LOC_QUAD = 0x0100,
};

enum sensor_struct_type {
	OCC_SENSOR_READING_FULL = 0x01,
	OCC_SENSOR_READING_COUNTER = 0x02,
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

#define TO_FP(f) ((f >> 8) * pow(10, ((int8_t)(f & 0xFF))))

void init_occ();
void finalize_occ();

#endif
