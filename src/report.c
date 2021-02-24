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

HIDDEN void print_final_report()
{
	int i, j;
	int world_rank, world_size, local_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_size(cntd->comm_local, &local_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Datatype cpu_type = get_mpi_datatype_cpu();
	MPI_Datatype node_type = get_mpi_datatype_node();
	
	CNTD_CPUInfo_t cpuinfo[world_size];
	CNTD_NodeInfo_t nodeinfo[local_size];

	PMPI_Gather(&cntd->cpu, 1, cpu_type, 
		cpuinfo, 1, cpu_type, 
		0, MPI_COMM_WORLD);
	if(cntd->iam_local_master)
	{
		PMPI_Gather(&cntd->node, 1, node_type, 
			nodeinfo, 1, node_type, 
			0, cntd->comm_local);
	}

	if(world_rank == 0)
	{
		double tot_energy_pkg = 0;
		double tot_energy_dram = 0;
		double tot_energy_gpu = 0;

		double exe_time = nodeinfo[0].exe_time[END] - nodeinfo[0].exe_time[START];
		for(i = 0; i < local_size; i++)
		{
            for(j = 0; j < nodeinfo[i].num_sockets; j++)
			{
				tot_energy_pkg += (double) nodeinfo[i].energy_pkg[j];
				tot_energy_dram += (double) nodeinfo[i].energy_dram[j];
			}
#ifdef CNTD_ENABLE_CUDA
			for(j = 0; j < nodeinfo[i].num_gpus; j++)
				tot_energy_gpu += (double) nodeinfo[i].energy_gpu[j];
#endif
		}

		// Conversions
		tot_energy_pkg = tot_energy_pkg / 1.0E6;
		tot_energy_dram = tot_energy_dram / 1.0E6;
#ifdef CNTD_ENABLE_CUDA
		tot_energy_gpu = tot_energy_gpu / 1.0E3;
#endif

		double app_time = 0;
		double mpi_time = 0;
		uint64_t mpi_type_cnt[NUM_MPI_TYPE] = {0};
		double mpi_type_time[NUM_MPI_TYPE] = {0};
		for(i = 0; i < world_size; i++)
		{
			app_time += cpuinfo[i].app_time;
			mpi_time += cpuinfo[i].mpi_time;
			for(j = 0; j < NUM_MPI_TYPE; j++)
			{
				if(cpuinfo[i].mpi_type_cnt[j] > 0)
				{
					mpi_type_cnt[j]++;
					mpi_type_time[j] += cpuinfo[i].mpi_type_time[j];
				}
			}
		}

		printf("#####################################\n");
		printf("############# COUNTDOWN #############\n");
		printf("#####################################\n");
		printf("Execution time: %.3f sec\n", exe_time);
		printf("############### ENERGY ##############\n");
		printf("Package energy: %.3f J\n", tot_energy_pkg);
		printf("DRAM energy: %.3f J\n", tot_energy_dram);
#ifdef CNTD_ENABLE_CUDA
		printf("GPU energy: %.3f J\n", tot_energy_gpu);
#endif
		printf("Total energy: %.3f J\n", tot_energy_pkg + tot_energy_dram + tot_energy_gpu);
		printf("############# AVG POWER #############\n");
		printf("AVG package power: %.2f W\n", tot_energy_pkg / exe_time);
		printf("AVG DRAM power: %.2f W\n", tot_energy_dram / exe_time);
#ifdef CNTD_ENABLE_CUDA
		printf("AVG GPU power: %.2f W\n", tot_energy_gpu / exe_time);
#endif
		printf("AVG power: %.2f W\n", (tot_energy_pkg + tot_energy_dram + tot_energy_gpu) / exe_time);
		printf("############## Timing ###############\n");
		printf("Application time: %.3f sec - %.2f%%\n", app_time, (app_time/(app_time+mpi_time))*100.0);
		printf("MPI time: %.3f sec - %.2f%%\n", mpi_time, (mpi_time/(app_time+mpi_time))*100.0);
		printf("########### MPI REPORTING ###########\n");
		for(j = 0; j < NUM_MPI_TYPE; j++)
		{
			if(mpi_type_cnt[j] > 0)
			{
				printf("%s: %d - %.3f Sec - %.2f%%\n", 
					mpi_type_str[j]+2, 
					mpi_type_cnt[j], 
					mpi_type_time[j], 
					(mpi_type_time[j]/mpi_time)*100.0);
			}
		}
		printf("#####################################\n");
	}
}

static FILE *timeseries_fd;
HIDDEN void init_timeseries_report()
{
	int i;
	char filename[STRING_SIZE];

	snprintf(filename, STRING_SIZE, "%s/%s.csv", cntd->log_dir, cntd->node.hostname);
	timeseries_fd = fopen(filename, "w");
	if(timeseries_fd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed create time-series file '%s'!\n", filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Time sample
	fprintf(timeseries_fd, "time-sample");

	// Energy
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";energy-pkg-%d;energy-dram-%d", i, i);
	}
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";energy-gpu-%d", i);
	}
	fprintf(timeseries_fd, ";energy-tot");

	// Power
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";power-pkg-%d;power-dram-%d", i, i);
	}
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";power-gpu-%d", i);
	}
	fprintf(timeseries_fd, ";power-tot\n");
}

HIDDEN void print_timeseries_report(double time_curr, double time_prev, uint64_t *energy_pkg_diff, uint64_t *energy_dram_diff, uint64_t *energy_gpu_diff)
{
	int i;
	double time_diff = time_curr - time_prev;

	// Time sample
	fprintf(timeseries_fd, "%.3f", time_curr - cntd->node.exe_time[START]);

	// Energy
	uint64_t energy_tot = 0;
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";%.2f;%.2f", 
			energy_pkg_diff[i]/1.0E6, 
			energy_dram_diff[i]/1.0E6);
		energy_tot += (energy_pkg_diff[i] + energy_dram_diff[i]);
	}
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";%.2f", 
			energy_gpu_diff[i]/1.0E3);
		energy_tot += energy_gpu_diff[i];
	}
	fprintf(timeseries_fd, ";%.2f", energy_tot/1.0E3);

	// Power
	for(i = 0; i < cntd->node.num_sockets; i++)
	{
		fprintf(timeseries_fd, ";%.2f;%.2f", 
			(energy_pkg_diff[i]/1.0E6)/time_diff, 
			(energy_dram_diff[i]/1.0E6)/time_diff);
	}
	for(i = 0; i < cntd->node.num_gpus; i++)
	{
		fprintf(timeseries_fd, ";%.2f", 
			(energy_gpu_diff[i]/1.0E3)/time_diff);
	}
	fprintf(timeseries_fd, ";%.2f\n", (energy_tot/1.0E6)/time_diff);
}

HIDDEN void finalize_timeseries_report()
{
	fclose(timeseries_fd);
}
