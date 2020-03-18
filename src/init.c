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

static void read_env()
{
	// Enable countdown v.1
	char *cntd_enable = getenv("CNTD_ENABLE");
	if(str_to_bool(cntd_enable))
		cntd->enable_cntd = TRUE;

	// Used-defined max and min P-states
	char *max_pstate_str = getenv("CNTD_MAX_PSTATE");
	if(max_pstate_str != NULL)
		cntd->pstate[MAX] = strtoul(max_pstate_str, 0L, 10);
	char *min_pstate_str = getenv("CNTD_MIN_PSTATE");
	if(min_pstate_str != NULL)
		cntd->pstate[MIN] = strtoul(min_pstate_str, 0L, 10);

	// Timeout
	char *timeout_str = getenv("CNTD_TIMEOUT");
	if(timeout_str != NULL)
		cntd->timeout = strtoul(timeout_str, 0L, 10);
	else
		cntd->timeout = DEFAULT_TIMEOUT;

	// Check if the maximum p-state is greater than minimum p-state
	if(cntd->pstate[MAX] < cntd->pstate[MIN])
	{
		fprintf(stderr, "Error: <countdown> Maximum P-state cannot less than minimum P-state of the system!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

static void read_pstates()
{
	// Read minimum P-state
	char min_pstate_file[] = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq";
	char min_pstate_value[STRING_SIZE];
	if(read_str_from_file(min_pstate_file, min_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", min_pstate_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->pstate[MIN] = (int) (strtof(min_pstate_value, NULL) / 1.0E5);

	// Read maximum P-state
	char max_pstate_file[] = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";
	char max_pstate_value[STRING_SIZE];
	if(read_str_from_file(max_pstate_file, max_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", max_pstate_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->pstate[MAX] = (int) (strtof(max_pstate_value, NULL) / 1.0E5);
}

static void read_energy(int when)
{
	int i, j;
	DIR* dir;
	uint64_t energy_pkg = 0, energy_dram = 0;
	char dirname[STRING_SIZE], filename[STRING_SIZE], name[STRING_SIZE], energy_str[STRING_SIZE];

	for(i = 0; i < NUM_SOCKETS; i++)
	{
		// Check all packages
		snprintf(dirname, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d", i);
		dir = opendir(dirname);
		if(dir) {
			closedir(dir);
			
			// Check if this domain is the package domain
			snprintf(filename, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/name", i);
			if(read_str_from_file(filename, name) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			if(strstr(name, "package") != NULL)
			{
				// Read the package energy
				snprintf(filename, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/energy_uj", i);
				if(read_str_from_file(filename, energy_str) < 0)
				{
					fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
					PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				}
				cntd->energy[i][PKG][when] += strtoul(energy_str, NULL, 10);

				// Find DRAM domain in this package
				for(j = 0; j < 3; j++)
				{
					snprintf(dirname, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d", i, i, j);
					dir = opendir(dirname);
					if(dir) {
						closedir(dir);
						
						// Check if this domain is the dram domain
						snprintf(filename, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d/name", i, i, j);
						if(read_str_from_file(filename, name) < 0)
						{
							fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
							PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
						}

						if(strstr(name, "dram") != NULL)
						{
							// Read the dram energy
							snprintf(filename, STRING_SIZE, "/sys/devices/virtual/powercap/intel-rapl/intel-rapl:%d/intel-rapl:%d:%d/energy_uj", i, i, j);
							if(read_str_from_file(filename, energy_str) < 0)
							{
								fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
								PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
							}
							cntd->energy[i][DRAM][when] += strtoul(energy_str, NULL, 10);
						}
					}
				}
			}
		} 
	}
}

static void print_report()
{
	int i, j, k;
	int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	uint64_t energy[NUM_SOCKETS][RAPL_DOMAINS];
	uint64_t energy_world[world_size][NUM_SOCKETS][RAPL_DOMAINS];
	char host[STRING_SIZE];
	char host_world[world_size][STRING_SIZE];

	gethostname(host, sizeof(host));

	for(j = 0; j < NUM_SOCKETS; j++)
		for(k = 0; k < RAPL_DOMAINS; k++)
			energy[j][k] = cntd->energy[j][k][END] - cntd->energy[j][k][START];

	MPI_Gather(host, STRING_SIZE, MPI_CHAR, host_world, STRING_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Gather(energy, NUM_SOCKETS*RAPL_DOMAINS, MPI_UNSIGNED_LONG, 
		energy_world, NUM_SOCKETS*RAPL_DOMAINS*NUM_SOCKETS, MPI_UNSIGNED_LONG, 
		0, MPI_COMM_WORLD);

	if(world_rank == 0)
	{
		int flag = FALSE;
		uint64_t tot_energy_uj[2] = {0};
		double tot_energy[2];
		char host_sum[world_size][STRING_SIZE];
		int host_count = 0;

		double exe_time = cntd->exe_time[END] - cntd->exe_time[START];

		for(i = 0; i < world_size; i++)
		{
			for(j = 0; j < NUM_SOCKETS; j++)
			{
				for(k = 0; k < host_count; k++)
				{
					if(strcmp(host_sum[k], host_world[i]) == 0)
					{
						flag = TRUE;
						break;
					}
				}
				if(flag == FALSE)
				{
					strncpy(host_sum[host_count], host_world[i], STRING_SIZE);
					host_count++;
					tot_energy_uj[PKG] += energy_world[i][j][PKG];
					tot_energy_uj[DRAM] += energy_world[i][j][DRAM];
				}
				else
					flag = FALSE;
			}
		}
		tot_energy[PKG] = ((double) tot_energy_uj[PKG]) / 1.0E6;
		tot_energy[DRAM] = ((double) tot_energy_uj[DRAM]) / 1.0E6;

		printf("#####################################\n");
		printf("############# COUNTDOWN #############\n");
		printf("#####################################\n");
		printf("Execution time: %.3f sec\n", exe_time);
		printf("############### ENERGY ##############\n");
		printf("Package energy: %.3f J\n", tot_energy[PKG]);
		printf("DRAM energy: %.3f J\n", tot_energy[DRAM]);
		printf("Total energy: %.3f J\n", tot_energy[PKG] + tot_energy[DRAM]);
		printf("############# AVG POWER #############\n");
		printf("AVG Package power: %.2f W\n", tot_energy[PKG] / exe_time);
		printf("AVG DRAM power: %.2f W\n", tot_energy[DRAM]  / exe_time);
		printf("AVG power: %.2f W\n", (tot_energy[PKG] + tot_energy[DRAM]) / exe_time);
		printf("#####################################\n");
	}
}

void start_cntd()
{
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));

	// Read P-state cnfigurations
	read_pstates();

	// Read environment variables
	read_env();

	// Init energy-aware MPI
	if(cntd->enable_cntd)
		eam_init();
	else
		eam_slack_init();

	// Read the energy counter of package and DRAM
	read_energy(START);

	// Read time
	cntd->exe_time[START] = read_time();

	// Synchronize all ranks
	PMPI_Barrier(MPI_COMM_WORLD);
}

void stop_cntd()
{
	// Read time
	cntd->exe_time[END] = read_time();

	// Read the energy counter of package and DRAM
	read_energy(END);

	// Finalize energy-aware MPI
	eam_finalize();

	// Print the final report
	print_report();

	// Deallocate global variables
	free(cntd);
}

void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->enable_cntd)
		eam_start_mpi();
	else
		eam_slack_start_mpi(mpi_type, comm, addr);
}

void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->enable_cntd)
		eam_end_mpi();
	else
		eam_slack_end_mpi(mpi_type, comm, addr);
}