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

#include "cntd.h"

#ifdef INTEL
static void read_energy_rapl(uint64_t energy_pkg[2][MAX_NUM_SOCKETS], uint64_t energy_dram[2][MAX_NUM_SOCKETS], int curr)
{
	int i, rv;
	char energy_str[STRING_SIZE];

	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		rv = lseek(cntd->energy_pkg_fd[i], 0, SEEK_SET);
		if(rv < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to rewind the RAPL pkg interface of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		rv = read(cntd->energy_pkg_fd[i], energy_str, STRING_SIZE);
		if(rv <= 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to read the RAPL pkg interface of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		sscanf(energy_str, "%llu\n", &energy_pkg[curr][i]);

		rv = lseek(cntd->energy_dram_fd[i], 0, SEEK_SET);
		if(rv < 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to rewind the RAPL dram interface of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		rv = read(cntd->energy_dram_fd[i], energy_str, STRING_SIZE);
		if(rv <= 0)
		{
			fprintf(stderr, "Error: <countdown> Failed to read the RAPL dram interface of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		sscanf(energy_str, "%llu\n", &energy_dram[curr][i]);
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

static void read_energy_occ(uint64_t energy_sys[2], uint64_t energy_pkg[2][MAX_NUM_SOCKETS], uint64_t energy_dram[2][MAX_NUM_SOCKETS], uint64_t energy_gpu[2][MAX_NUM_GPUS], int curr)
{
	uint32_t offset, sensor_freq;
	uint8_t *ping;
	occ_sensor_record_t *sensor_data;

	int rv = lseek(cntd->occ_fd, 0, SEEK_SET);
	if(rv < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to read the occ\n");
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
					energy_sys[curr] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRPROC", STRING_SIZE) == 0)
					energy_pkg[curr][i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRMEM", STRING_SIZE) == 0)
					energy_dram[curr][i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
				else if(strncmp(md[j].name, "PWRGPU", STRING_SIZE) == 0)
					energy_gpu[curr][i] = (uint64_t)(be64toh(sensor_data->accumulator) / TO_FP(sensor_freq));
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
			fprintf(stderr, "Error: <countdown> Failed to read the tx2mon of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		rv = read(node->fd, op, sizeof(*op));
		if(rv < sizeof(*op))
		{
			fprintf(stderr, "Error: <countdown> Failed to read the tx2mon of socket %d\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		if(CMD_STATUS_READY(op->cmd_status) == 0)
		{
			fprintf(stderr, "Error: <countdown> The tx2mon is not ready yet, please try again\n", i);
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
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		if(nvmlDeviceGetTotalEnergyConsumption(cntd->gpu[i], &energy_mj))
		{
			fprintf(stderr, "Error: <countdown> Failed to read energy consumption from GPU number %d'\n", i);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
		energy_gpu[curr][i] = (uint64_t)(energy_mj * 1000);
	}
}
#endif

static void read_energy(double *energy_sys, double energy_pkg[MAX_NUM_SOCKETS], double energy_dram[MAX_NUM_SOCKETS], double energy_gpu[MAX_NUM_GPUS], int curr, int prev)
{
	int i;
#if defined(INTEL) || defined(POWER9)
    static uint64_t energy_pkg_s[2][MAX_NUM_SOCKETS] = {0};
    static uint64_t energy_dram_s[2][MAX_NUM_SOCKETS] = {0};
#endif
#if defined(POWER9) || defined(NVIDIA_GPU)
	static uint64_t energy_gpu_s[2][MAX_NUM_GPUS] = {0};
#endif

#ifdef INTEL
	*energy_sys = 0.0;

	read_energy_rapl(energy_pkg_s, energy_dram_s, curr);

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
	
	read_energy_occ(energy_sys_s, energy_pkg_s, energy_dram_s, energy_gpu_s, curr);

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

	#ifndef NVIDIA_GPU
		energy_gpu[i] = (double) diff_overflow(
			energy_gpu_s[curr][i], 
			energy_gpu_s[prev][i], 
			UINT64_MAX);
	#endif
	}
#elif THUNDERX2
	*energy_sys = 0.0;
	for(i = 0; i < cntd->node.num_sockets; i++)
		*energy_dram = 0.0;
	read_energy_tx2mon(energy_pkg);
#endif
#ifdef NVIDIA_GPU
	read_energy_gpu_nvidia(energy_gpu_s, curr);
	for(i = 0; i < cntd->node.num_gpus; i++)
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
	static int i, init = FALSE;
	static int flip = 0;
	static double timing[2];
	double energy_sys = 0;
    double energy_pkg[MAX_NUM_SOCKETS] = {0};
    double energy_dram[MAX_NUM_SOCKETS] = {0};
	double energy_gpu[MAX_NUM_GPUS] = {0};

	if(init == FALSE)
	{
        timing[flip] = read_time();

		if(cntd->enable_hw_monitor)
		{
#ifdef POWER9
			make_occ_sample(flip);
#elif THUNDERX2
			make_tx2mon_sample();
#endif
			read_energy(&energy_sys, energy_pkg, energy_dram, energy_gpu, 0, 1);
		}

        cntd->num_sampling++;
		init = TRUE;
	}
	else
	{
		int prev = flip;
		flip = (flip == 0) ? 1 : 0;
		int curr = flip;

        timing[curr] = read_time();

		if(cntd->enable_hw_monitor)
		{
#ifdef POWER9
			make_occ_sample(curr);
#elif THUNDERX2
			make_tx2mon_sample();
#endif
			read_energy(&energy_sys, energy_pkg, energy_dram, energy_gpu, curr, prev);

			// Update energy
			cntd->node.energy_sys += energy_sys;
			for(i = 0; i < cntd->node.num_sockets; i++)
			{
				cntd->node.energy_pkg[i] += energy_pkg[i];
				cntd->node.energy_dram[i] += energy_dram[i];
#if defined(POWER9) && !defined(NVIDIA_GPU)
				cntd->node.energy_gpu[i] += energy_gpu[i];
#endif
			}
#ifdef NVIDIA_GPU
			for(int i = 0; i < cntd->node.num_gpus; i++)
				cntd->node.energy_gpu[i] += energy_gpu[i];
#endif
		}
		if(cntd->enable_ts_report)
			print_timeseries_report(timing[curr], timing[prev], energy_sys, energy_pkg, energy_dram, energy_gpu);

        cntd->num_sampling++;
	}
}

HIDDEN void event_sample(MPI_Type_t mpi_type, int phase)
{
	static double timing[2] = {0};

	if(phase == START)
	{
		timing[START] = read_time();
		if(mpi_type == __MPI_INIT)
		{
			cntd->cpu.exe_time[START] = timing[START];
			cntd->node.exe_time[START] = timing[START];
		}
		else
			cntd->cpu.app_time += timing[START] - timing[END];
	}
	else if(phase == END)
	{
		timing[END] = read_time();

		double mpi_time = timing[END] - timing[START];
		cntd->cpu.mpi_time += mpi_time;
		cntd->cpu.mpi_type_time[mpi_type] += mpi_time;
		cntd->cpu.mpi_type_cnt[mpi_type]++;

		if(mpi_type == __MPI_FINALIZE)
		{
			cntd->cpu.exe_time[END] = timing[END];
			cntd->node.exe_time[END] = timing[END];
		}
	}
}
