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

#include "cntd.h"

#ifndef __INTEL_COMPILER
#include <math.h>
#endif

static double timing_event_sample[2] = {0};

#ifdef INTEL
static void read_energy_rapl(uint64_t *energy_pkg, uint64_t *energy_dram)
{
	int i, rv;
	char energy_str[STRING_SIZE];

	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		rv = lseek(cntd->energy_pkg_fd[i], 0, SEEK_SET);
		if(rv < 0)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to rewind the RAPL pkg interface of socket %d\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		rv = read(cntd->energy_pkg_fd[i], energy_str, STRING_SIZE);
		if(rv <= 0)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read the RAPL pkg interface of socket %d\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		sscanf(energy_str, "%llu\n", &energy_pkg[i]);

		if (cntd->energy_dram_fd[i] != -1) {
			rv = lseek(cntd->energy_dram_fd[i], 0, SEEK_SET);
			if(rv < 0)
			{
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to rewind the RAPL dram interface of socket %d\n",
					cntd->node.hostname, cntd->rank->world_rank, i);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			rv = read(cntd->energy_dram_fd[i], energy_str, STRING_SIZE);
			if(rv <= 0)
			{
				fprintf(stdout, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read the RAPL dram interface of socket %d\n",
					cntd->node.hostname, cntd->rank->world_rank, i);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			sscanf(energy_str, "%llu\n", &energy_dram[i]);
		}
		else
			energy_dram[i] = 0;
	}
}
#elif POWER9
static void *occ_buff[2][MAX_NUM_SOCKETS][OCC_SENSOR_DATA_BLOCK_SIZE];

static void make_occ_sample(int curr)
{
	int rc, bytes;

	for(int i = 0; i < cntd->node.num_sockets; i++)
	{
		for(rc = bytes = 0; bytes < OCC_SENSOR_DATA_BLOCK_SIZE; bytes += rc) 
		{
			rc = read(cntd->occ_fd, occ_buff[curr][i] + bytes, OCC_SENSOR_DATA_BLOCK_SIZE - bytes);
			if(!rc || rc < 0)
				break;
		}
	}
}

static void read_energy_occ(uint64_t *energy_sys, uint64_t *energy_pkg, uint64_t *energy_dram, uint64_t *energy_gpu, int curr)
{
	uint32_t offset, sensor_freq;
	uint8_t *ping;
	occ_sensor_record_t *sensor_data;

	int rv = lseek(cntd->occ_fd, 0, SEEK_SET);
	if(rv < 0)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read the occ\n",
			cntd->node.hostname, cntd->rank->world_rank);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	for(int i = 0; i < cntd->node.num_sockets; i++)
	{
		occ_sensor_data_header_t *hb = (occ_sensor_data_header_t *)(uint64_t)occ_buff[curr][i];
		occ_sensor_name_t *md = (occ_sensor_name_t *)((uint64_t)hb + be32toh(hb->names_offset));

		for(int j = 0; j < be16toh(hb->nr_sensors); j++)
		{
			offset = be32toh(md[j].reading_offset);

			if(be16toh(md[j].type) == OCC_SENSOR_TYPE_POWER)
			{
				ping = (uint8_t *)((uint64_t)hb + be32toh(hb->reading_ping_offset));
				sensor_data = (occ_sensor_record_t *)((uint64_t)ping + offset);
				sensor_freq = be32toh(md[j].freq);

				if(strncmp(md[j].name, "PWRSYS", STRING_SIZE) == 0)
					*energy_sys = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRPROC", STRING_SIZE) == 0)
					energy_pkg[i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRMEM", STRING_SIZE) == 0)
					energy_dram[i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRGPU", STRING_SIZE) == 0)
					energy_gpu[i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
			}
		}
	}
}
#elif THUNDERX2
static inline double cpu_temp(node_data_t *d, int c)
{
	return to_c(d->buf.tmon_cpu[c]);
}

static inline unsigned int cpu_freq(node_data_t *d, int c)
{
	return d->buf.freq_cpu[c];
}

static inline double to_v(int mv)
{
	return mv/1000.0;
}

static inline double to_w(int mw)
{
	return mw/1000.0;
}

static void make_tx2mon_sample()
{
	int i, rv;
	node_data_t *node;
	mc_oper_region_t *op; 

	for(i = 0; i < cntd->tx2mon.nodes; i++)
	{
		node = &cntd->tx2mon.node[i];
		op = &node->buf;
		
		rv = lseek(node->fd, 0, SEEK_SET);
		if(rv < 0)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read the tx2mon of socket %d\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		rv = read(node->fd, op, sizeof(*op));
		if(rv < sizeof(*op))
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read the tx2mon of socket %d\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		if(CMD_STATUS_READY(op->cmd_status) == 0)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> The tx2mon is not ready yet, please try again\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		if(CMD_VERSION(op->cmd_status) > 0)
			node->throttling_available =  1;
		else
			node->throttling_available =  0;
	}
}

static void read_energy_tx2mon(double *energy_pkg)
{
	int i;
	for(i = 0; i < cntd->tx2mon.nodes; i++)
	{
		energy_pkg[i] = to_w(cntd->tx2mon.node[i].buf.pwr_core);
		energy_pkg[i] += to_w(cntd->tx2mon.node[i].buf.pwr_sram);
		energy_pkg[i] += to_w(cntd->tx2mon.node[i].buf.pwr_mem);
		energy_pkg[i] += to_w(cntd->tx2mon.node[i].buf.pwr_soc);
	}
}
#endif

#ifdef NVIDIA_GPU
static void read_energy_gpu_nvidia(uint64_t energy_gpu[2][MAX_NUM_GPUS], int curr)
{
	int i;
	unsigned long long energy_mj;
	for(i = 0; i < cntd->gpu.num_gpus; i++)
	{
		if(nvmlDeviceGetTotalEnergyConsumption(cntd->gpu_device[i], &energy_mj))
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to read energy consumption from GPU number %d'\n", 
				cntd->node.hostname, cntd->rank->world_rank, i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		energy_gpu[curr][i] = (uint64_t)(energy_mj * 1000);
	}
}
#endif

static void read_energy(double *energy_sys, double energy_pkg[MAX_NUM_SOCKETS], double energy_dram[MAX_NUM_SOCKETS], double energy_gpu_sys[MAX_NUM_GPUS], double energy_gpu[MAX_NUM_GPUS], int curr, int prev)
{
	int i;
#if defined(INTEL) || defined(POWER9)
    static uint64_t energy_pkg_s[2][MAX_NUM_SOCKETS] = {0};
    static uint64_t energy_dram_s[2][MAX_NUM_SOCKETS] = {0};
#endif
#ifdef POWER9
	static uint64_t energy_gpu_sys_s[2][MAX_NUM_SOCKETS] = {0};
#endif
#ifdef NVIDIA_GPU
	static uint64_t energy_gpu_s[2][MAX_NUM_GPUS] = {0};
#endif

#ifdef INTEL
	*energy_sys = 0.0;

	read_energy_rapl(energy_pkg_s[curr], energy_dram_s[curr]);

	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		uint64_t energy_diff = diff_overflow(
			energy_pkg_s[curr][i], 
			energy_pkg_s[prev][i],
			cntd->energy_pkg_overflow[i]);
		energy_pkg[i] = (double)energy_diff / 1.0E6;

		energy_diff = diff_overflow(
			energy_dram_s[curr][i], 
			energy_dram_s[prev][i],
			cntd->energy_dram_overflow[i]);
		energy_dram[i] = (double)energy_diff / 1.0E6;
	}
#elif POWER9
	static uint64_t energy_sys_s[2] = {0};
	
	read_energy_occ(&energy_sys_s[curr], energy_pkg_s[curr], energy_dram_s[curr], energy_gpu_sys_s[curr], curr);

	*energy_sys = diff_overflow(
		energy_sys_s[curr], 
		energy_sys_s[prev],
		UINT64_MAX);
		
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		energy_pkg[i] = (double) diff_overflow(
			energy_pkg_s[curr][i], 
			energy_pkg_s[prev][i],
			UINT64_MAX);

		energy_dram[i] = (double) diff_overflow(
			energy_dram_s[curr][i], 
			energy_dram_s[prev][i], 
			UINT64_MAX);

		energy_gpu_sys[i] = (double) diff_overflow(
			energy_gpu_sys_s[curr][i], 
			energy_gpu_sys_s[prev][i], 
			UINT64_MAX);
	}
#elif THUNDERX2
	*energy_sys = 0.0;
	for(i = 0; i < cntd->node.num_sockets; i++)
		*energy_dram = 0.0;
	read_energy_tx2mon(energy_pkg);
#endif
#ifdef NVIDIA_GPU
	read_energy_gpu_nvidia(energy_gpu_s, curr);
	for(i = 0; i < cntd->gpu.num_gpus; i++)
	{
		uint64_t energy_diff = diff_overflow(
			energy_gpu_s[curr][i], 
			energy_gpu_s[prev][i], 
			UINT64_MAX);
		energy_gpu[i] = (double)energy_diff / 1.0E6;
	}
#endif
}

HIDDEN void time_sample(int sig, siginfo_t *siginfo, void *context)
{
	int i, j;
	static unsigned int init = FALSE;
	static int flip = 0;
	static double timing[3] = {0};
	static double time_region[MAX_NUM_CPUS][2][2] = {0};
	static uint64_t mpi_net[MAX_NUM_CPUS][2][2] = {0};
	static uint64_t mpi_file[MAX_NUM_CPUS][2][2] = {0};

	typedef struct read_format {
		uint64_t  raw_count;
		uint64_t  time_enabled;
		uint64_t  time_running;
	} read_format_t;
	// Objects with static storage duration will initialize to \"0\" if no
	// initializer is specified.
	static read_format_t perf[MAX_NUM_CPUS][MAX_NUM_PERF_EVENTS][2];

    double energy_pkg[MAX_NUM_SOCKETS] = {0};
    double energy_dram[MAX_NUM_SOCKETS] = {0};
	double energy_gpu_sys[MAX_NUM_SOCKETS] = {0};
	double energy_gpu[MAX_NUM_GPUS] = {0};
	double energy_sys = 0;

//#ifdef MOSQUITTO_ENABLED
//	char topic[STRING_SIZE];
//	char payload[STRING_SIZE];
//	int p_length; // \"payload\" length.
//	int rc = 0;
//	time_t utc_secs;
//	double exe_secs;
//	char postfix[STRING_SIZE];
//
//	exe_secs = 0.0;
//	if (cntd->rank->exe_is_started)
//		exe_secs = read_time() - cntd->rank->exe_time[START];
//	time(&utc_secs);
//	get_rand_postfix(postfix,
//					 STRING_SIZE);
//
//    snprintf(topic				   ,
//             STRING_SIZE		   ,
//             MQTT_TOPIC 		   ,
//			 postfix			   ,
//			 cntd->node.hostname   ,
//			 cntd->rank->cpu_id    ,
//			 cntd->rank->world_rank,
//			 "exe_time");
//    snprintf(payload	 ,
//             STRING_SIZE ,
//             MQTT_PAYLOAD,
//			 exe_secs	 ,
//			 utc_secs);
//	p_length = strlen(payload);
//
//	rc = mosquitto_connect(mosq		,
//						   MQTT_HOST,
//						   MQTT_PORT,
//						   MQTT_KEEPALIVE);
//
//	mosquitto_publish(mosq	  ,
//					  NULL	  ,
//					  topic	  ,
//					  p_length,
//					  payload ,
//					  MQTT_QOS,
//					  MQTT_RETAIN);
//#endif

	if(init == FALSE)
	{
		init = TRUE;
        timing[flip] = read_time();

		for(i = 0; i < cntd->local_rank_size; i++)
		{
			mpi_net[i][SEND][flip] = cntd->local_ranks[i]->mpi_net_data[SEND][TOT];
			mpi_net[i][RECV][flip] = cntd->local_ranks[i]->mpi_net_data[RECV][TOT];

			mpi_file[i][WRITE][flip] = cntd->local_ranks[i]->mpi_file_data[WRITE][TOT];
			mpi_file[i][READ][flip] = cntd->local_ranks[i]->mpi_file_data[READ][TOT];

			if(cntd->enable_perf)
			{
				read(cntd->perf_fd[i][PERF_INST_RET], &perf[i][PERF_INST_RET][flip], sizeof(perf[i][PERF_INST_RET][flip]));
				read(cntd->perf_fd[i][PERF_CYCLES], &perf[i][PERF_CYCLES][flip], sizeof(perf[i][PERF_CYCLES][flip]));
#ifdef INTEL
				read(cntd->perf_fd[i][PERF_CYCLES_REF], &perf[i][PERF_CYCLES_REF][flip], sizeof(perf[i][PERF_CYCLES_REF][flip]));
#endif
				for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
					if(cntd->perf_fd[i][j] > 0)
						read(cntd->perf_fd[i][j], &perf[i][j][flip], sizeof(perf[i][j][flip]));
			}
		}

		if(cntd->enable_power_monitor)
		{
#ifdef POWER9
			make_occ_sample(flip);
#elif THUNDERX2
			make_tx2mon_sample();
#endif
			read_energy(&energy_sys, energy_pkg, energy_dram, energy_gpu_sys, energy_gpu, 0, 1);
		}
	}
	else
	{
		int prev = flip;
		flip = (flip == 0) ? 1 : 0;
		int curr = flip;

		// Do sample
        timing[curr] = read_time();
		for(i = 0; i < cntd->local_rank_size; i++)
		{
			time_region[i][APP][curr] = cntd->local_ranks[i]->app_time[TOT];
			time_region[i][MPI][curr] = cntd->local_ranks[i]->mpi_time[TOT];
			if(cntd->into_mpi)
			{
				if(time_region[i][MPI][curr] < time_region[i][MPI][prev])
				{
					time_region[i][MPI][curr] = time_region[i][MPI][prev] + cntd->sampling_time;
					cntd->local_ranks[i]->mpi_time[CURR] = cntd->sampling_time;
					cntd->local_ranks[i]->app_time[CURR] = 0;
				}
				else
				{
					time_region[i][MPI][curr] += timing[curr] - timing_event_sample[START];
					cntd->local_ranks[i]->mpi_time[CURR] = time_region[i][MPI][curr] - time_region[i][MPI][prev];
					cntd->local_ranks[i]->app_time[CURR] = time_region[i][APP][curr] - time_region[i][APP][prev];
				}
			}
			else
			{
				if(time_region[i][APP][curr] < time_region[i][APP][prev])
				{
					time_region[i][APP][curr] = time_region[i][APP][prev] + cntd->sampling_time;
					cntd->local_ranks[i]->app_time[CURR] = cntd->sampling_time;
					cntd->local_ranks[i]->mpi_time[CURR] = 0;
				}
				else
				{
					time_region[i][APP][curr] += timing[curr] - timing_event_sample[END];
					cntd->local_ranks[i]->app_time[CURR] = time_region[i][APP][curr] - time_region[i][APP][prev];
					cntd->local_ranks[i]->mpi_time[CURR] = time_region[i][MPI][curr] - time_region[i][MPI][prev];
				}
			}

			mpi_net[i][SEND][curr] = cntd->local_ranks[i]->mpi_net_data[SEND][TOT];
			mpi_net[i][RECV][curr] = cntd->local_ranks[i]->mpi_net_data[RECV][TOT];

			mpi_file[i][WRITE][curr] = cntd->local_ranks[i]->mpi_file_data[WRITE][TOT];
			mpi_file[i][READ][curr] = cntd->local_ranks[i]->mpi_file_data[READ][TOT];

			// Perf events
			if(cntd->enable_perf)
			{
				read(cntd->perf_fd[i][PERF_INST_RET], &perf[i][PERF_INST_RET][curr], sizeof(perf[i][PERF_INST_RET][curr]));
				read(cntd->perf_fd[i][PERF_CYCLES], &perf[i][PERF_CYCLES][curr], sizeof(perf[i][PERF_CYCLES][curr]));
#ifdef INTEL
				read(cntd->perf_fd[i][PERF_CYCLES_REF], &perf[i][PERF_CYCLES_REF][curr], sizeof(perf[i][PERF_CYCLES_REF][curr]));
#endif
				for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
					if(cntd->perf_fd[i][j] > 0)
						read(cntd->perf_fd[i][j], &perf[i][j][curr], sizeof(perf[i][j][curr]));
			}
		}

		if(cntd->enable_power_monitor)
		{
#ifdef POWER9
			make_occ_sample(curr);
#elif THUNDERX2
			make_tx2mon_sample();
#endif
			read_energy(&energy_sys, energy_pkg, energy_dram, energy_gpu_sys, energy_gpu, curr, prev);

			// Update energy
			cntd->node.energy_sys += energy_sys;
			for(i = 0; i < cntd->node.num_sockets; i++)
			{
				cntd->node.energy_pkg[i] += energy_pkg[i];
				cntd->node.energy_dram[i] += energy_dram[i];
#ifdef POWER9
				cntd->node.energy_gpu[i] += energy_gpu_sys[i];
#endif
			}
		}

		unsigned int util_gpu[MAX_NUM_GPUS] = {0};
		unsigned int util_mem_gpu[MAX_NUM_GPUS] = {0};
		unsigned int temp_gpu[MAX_NUM_GPUS] = {0};
		unsigned int clock_gpu[MAX_NUM_GPUS] = {0};
#ifdef NVIDIA_GPU
		nvmlUtilization_t nvml_util;

		for(int i = 0; i < cntd->gpu.num_gpus; i++)
		{
			// Energy
			cntd->gpu.energy[i] += energy_gpu[i];

			// Utilization
			nvmlDeviceGetUtilizationRates(cntd->gpu_device[i], &nvml_util);
			util_gpu[i] = nvml_util.gpu;
			util_mem_gpu[i] = nvml_util.memory;
			cntd->gpu.util[i] += nvml_util.gpu;
			cntd->gpu.util_mem[i] += nvml_util.memory;

			// Temperature
			nvmlDeviceGetTemperature(cntd->gpu_device[i], NVML_TEMPERATURE_GPU, &temp_gpu[i]);
			cntd->gpu.temp[i] += temp_gpu[i];

			// Clock
			nvmlDeviceGetClock(cntd->gpu_device[i], NVML_CLOCK_SM, NVML_CLOCK_ID_CURRENT, &clock_gpu[i]);
			cntd->gpu.clock[i] += clock_gpu[i];
		}
#endif

		// Calculate sample
		for(i = 0; i < cntd->local_rank_size; i++)
		{
			cntd->local_ranks[i]->mpi_net_data[SEND][CURR] = mpi_net[i][SEND][curr] - mpi_net[i][SEND][prev];
			cntd->local_ranks[i]->mpi_net_data[RECV][CURR] = mpi_net[i][RECV][curr] - mpi_net[i][RECV][prev];

			cntd->local_ranks[i]->mpi_file_data[WRITE][CURR] = mpi_file[i][WRITE][curr] - mpi_file[i][WRITE][prev];
			cntd->local_ranks[i]->mpi_file_data[READ][CURR] = mpi_file[i][READ][curr] - mpi_file[i][READ][prev];

			if(cntd->enable_perf)
			{
				for(j = 0; j < MAX_NUM_PERF_EVENTS; j++)
				{
					double time_en_c = 0.0;
					double time_run_c = 0.0;
					double time_mul_c = 0.0;
					double d_raw_count_c;
					double d_total_c;
					double time_en_p = 0.0;
					double time_run_p = 0.0;
					double time_mul_p = 0.0;
					double d_raw_count_p;
					double d_total_p;

					time_en_c = (double)perf[i][j][curr].time_enabled;
					time_run_c = (double)perf[i][j][curr].time_running;
					time_mul_c = time_en_c/time_run_c;
					time_en_p = (double)perf[i][j][prev].time_enabled;
					time_run_p = (double)perf[i][j][prev].time_running;
					time_mul_p = time_en_p/time_run_p;

					d_raw_count_c = (double)perf[i][j][curr].raw_count;
					d_total_c = d_raw_count_c * time_mul_c;
					d_raw_count_p = (double)perf[i][j][prev].raw_count;
					d_total_p = d_raw_count_p * time_mul_p;

					cntd->local_ranks[i]->perf[j][CURR] = diff_overflow((uint64_t)d_total_c, (uint64_t)d_total_p, UINT64_MAX);
					cntd->local_ranks[i]->perf[j][TOT] += cntd->local_ranks[i]->perf[j][CURR];
				}
			}

			cntd->local_ranks[i]->num_sampling++;
		}

		if(cntd->enable_timeseries_report)
		{
			print_timeseries_report(timing[curr], timing[prev], 
				energy_sys, energy_pkg, energy_dram, 
				energy_gpu_sys, energy_gpu,
				util_gpu, util_mem_gpu, temp_gpu, clock_gpu);
		}
	}
}

HIDDEN void init_time_sample()
{
	if(cntd->rank->local_rank == 0)
	{
		if(cntd->enable_power_monitor)
		{
#ifdef INTEL
			init_rapl();
#elif POWER9
			init_occ();
#elif THUNDERX2
			init_tx2mon(&cntd->tx2mon);
#endif
		}

#ifdef NVIDIA_GPU
		init_nvml();
#endif
		if(cntd->enable_perf)
			init_perf();

		// Start timer
		PMPI_Barrier(cntd->comm_local_masters);
		make_timer(&cntd->timer, &time_sample, cntd->sampling_time, cntd->sampling_time);
		PMPI_Barrier(cntd->comm_local_masters);
		time_sample(0, NULL, NULL);
	}
}

HIDDEN void finalize_time_sample()
{
	if(cntd->rank->local_rank == 0)
	{
		// Delete sampling timer
		delete_timer(cntd->timer);

		// Last sample
		time_sample(0, NULL, NULL);

		if(cntd->enable_power_monitor)
		{
#ifdef INTEL
			finalize_rapl();
#elif POWER9
			finalize_occ();
#elif THUNDERX2
			finalize_tx2mon(&cntd->tx2mon);
#endif
		}
#ifdef NVIDIA_GPU
		finalize_nvml();
#endif
		if(cntd->enable_perf)
			finalize_perf();
	}

	// Memory usage
	struct rusage r_usage;
	getrusage(RUSAGE_SELF, &r_usage);
	cntd->rank->max_mem_usage = r_usage.ru_maxrss;

	PMPI_Barrier(MPI_COMM_WORLD);
}

HIDDEN void event_sample_start(MPI_Type_t mpi_type)
{
	timing_event_sample[START] = read_time();

	if(mpi_type == __MPI_INIT || mpi_type == __MPI_INIT_THREAD) {
		cntd->rank->exe_time[START] = timing_event_sample[START];
		cntd->rank->exe_is_started = 1;
	}
	else
		cntd->rank->app_time[TOT] += timing_event_sample[START] - timing_event_sample[END];
}

HIDDEN void event_sample_end(MPI_Type_t mpi_type, int eam_flag)
{
	timing_event_sample[END] = read_time();

	double mpi_time = timing_event_sample[END] - timing_event_sample[START];
	cntd->rank->mpi_time[TOT] += mpi_time;
	cntd->rank->mpi_type_time[mpi_type] += mpi_time;
	cntd->rank->mpi_type_cnt[mpi_type]++;

	if(cntd->enable_cntd && eam_flag)
	{
		if(mpi_time > cntd->eam_timeout)
		{
			cntd->rank->cntd_mpi_type_time [mpi_type] += mpi_time - cntd->eam_timeout;
			cntd->rank->cntd_mpi_type_cnt[mpi_type]++;
		}
	}
	else if(cntd->enable_cntd_slack && eam_flag)
	{
		if(mpi_time > cntd->eam_timeout)
		{
			cntd->rank->cntd_mpi_type_time[mpi_type] += mpi_time - cntd->eam_timeout;
			cntd->rank->cntd_mpi_type_cnt[mpi_type]++;
		}
	}

	if(mpi_type == __MPI_FINALIZE)
		cntd->rank->exe_time[END] = timing_event_sample[END];
}
