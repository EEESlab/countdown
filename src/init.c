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

	// Enable only profiling
	char *cntd_no_eam = getenv("CNTD_NO_EAM");
	if(str_to_bool(cntd_no_eam))
		cntd->no_eam = TRUE;

	// Disable P2P MPIs
	char *cntd_no_p2p = getenv("CNTD_NO_P2P");
	if(str_to_bool(cntd_no_p2p))
		cntd->no_p2p = TRUE;

	// Used-defined max and min p-states
	char *max_pstate_str = getenv("CNTD_MAX_PSTATE");
	if(max_pstate_str != NULL)
		cntd->user_pstate[MAX] = strtoul(max_pstate_str, 0L, 10);
	else
		cntd->user_pstate[MAX] = NO_CONF;

	char *min_pstate_str = getenv("CNTD_MIN_PSTATE");
	if(min_pstate_str != NULL)
		cntd->user_pstate[MIN] = strtoul(min_pstate_str, 0L, 10);
	else
		cntd->user_pstate[MIN] = NO_CONF;

	// Timeout
	char *timeout_str = getenv("CNTD_TIMEOUT");
	if(timeout_str != NULL)
		cntd->timeout = strtoul(timeout_str, 0L, 10);
	else
		cntd->timeout = DEFAULT_TIMEOUT;

	// Sampling time
	char *sampling_time_str = getenv("CNTD_SAMPLING_TIME");
	if(sampling_time_str != NULL)
		cntd->sampling_time = strtoul(sampling_time_str, 0L, 10);
	else
		cntd->sampling_time = DEFAULT_SAMPLING_TIME;

	// Check consistency
	if(cntd->user_pstate[MIN] != NO_CONF && cntd->user_pstate[MIN] < cntd->sys_pstate[MIN])
	{
		fprintf(stderr, "Error: <countdown> User-defined min p-state cannot be lower \
			than the min system p-state (min system p-state = %d)!\n", cntd->sys_pstate[MIN]);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	if(cntd->user_pstate[MAX] != NO_CONF && cntd->user_pstate[MAX] > cntd->sys_pstate[MAX])
	{
		fprintf(stderr, "Error: <countdown> User-defined max p-state cannot be greater \
			than the max system p-state (max system p-state = %d)!\n", cntd->sys_pstate[MAX]);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	if(cntd->user_pstate[MAX] != NO_CONF && cntd->user_pstate[MIN] != NO_CONF)
	{
		if(cntd->user_pstate[MIN] > cntd->user_pstate[MAX])
		{
			fprintf(stderr, "Error: <countdown> Max p-state cannot be greater than min p-state!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
}

static void read_arch_conf()
{
	int i, j;
	DIR* dir;
	char dirname[STRING_SIZE], filename[STRING_SIZE], name[STRING_SIZE], energy_overflow[STRING_SIZE];

	// Read minimum p-state
	char min_pstate_file[] = CPUINFO_MIN_FREQ;
	char min_pstate_value[STRING_SIZE];
	if(read_str_from_file(min_pstate_file, min_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", min_pstate_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->sys_pstate[MIN] = (int) (strtof(min_pstate_value, NULL) / 1.0E5);

	// Read maximum p-state
	char max_pstate_file[] = CPUINFO_MAX_FREQ;
	char max_pstate_value[STRING_SIZE];
	if(read_str_from_file(max_pstate_file, max_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", max_pstate_file);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->sys_pstate[MAX] = (int) (strtof(max_pstate_value, NULL) / 1.0E5);

	for(i = 0; i < NUM_SOCKETS; i++)
	{
		// Check all packages
		snprintf(dirname, STRING_SIZE, INTEL_RAPL_PKG, i);
		dir = opendir(dirname);
		if(dir)
		{
			closedir(dir);
			
			// Check if this domain is the package domain
			snprintf(filename, STRING_SIZE, INTEL_RAPL_PKG_NAME, i);
			if(read_str_from_file(filename, name) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			if(strstr(name, "package") != NULL)
			{
				// Read the energy overflow value
				snprintf(filename, STRING_SIZE, PKG_MAX_ENERGY_RANGE_UJ, i);
				if(read_str_from_file(filename, energy_overflow) < 0)
				{
					fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
					PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				}
				cntd->energy_pkg_overflow[i] = strtoul(energy_overflow, NULL, 10);

				// Find DRAM domain in this package
				for(j = 0; j < 3; j++)
				{
					snprintf(dirname, STRING_SIZE, INTEL_RAPL_DRAM, i, i, j);
					dir = opendir(dirname);
					if(dir) {
						closedir(dir);
						
						// Check if this domain is the dram domain
						snprintf(filename, STRING_SIZE, INTEL_RAPL_DRAM_NAME, i, i, j);
						if(read_str_from_file(filename, name) < 0)
						{
							fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
							PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
						}

						if(strstr(name, "dram") != NULL)
						{
							// Read the dram energy
							snprintf(filename, STRING_SIZE, DRAM_MAX_ENERGY_RANGE_UJ, i, i, j);
							if(read_str_from_file(filename, energy_overflow) < 0)
							{
								fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
								PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
							}
							cntd->energy_dram_overflow[i] = strtoul(energy_overflow, NULL, 10);
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
	
	char host_world[world_size][STRING_SIZE];
	uint64_t energy_pkg_world[world_size][NUM_SOCKETS];
	uint64_t energy_dram_world[world_size][NUM_SOCKETS];

	char host[STRING_SIZE];
	gethostname(host, sizeof(host));

	PMPI_Gather(host, STRING_SIZE, MPI_CHAR, host_world, STRING_SIZE, MPI_CHAR, 
		0, MPI_COMM_WORLD);
	PMPI_Gather(cntd->energy_pkg, NUM_SOCKETS, MPI_UNSIGNED_LONG, 
		energy_pkg_world, NUM_SOCKETS, MPI_UNSIGNED_LONG, 
		0, MPI_COMM_WORLD);
	PMPI_Gather(cntd->energy_dram, NUM_SOCKETS, MPI_UNSIGNED_LONG, 
		energy_dram_world, NUM_SOCKETS, MPI_UNSIGNED_LONG, 
		0, MPI_COMM_WORLD);

	if(world_rank == 0)
	{
		int flag = FALSE;
		uint64_t tot_energy_pkg_uj = 0;
		uint64_t tot_energy_dram_uj = 0;
		double tot_energy_pkg, tot_energy_dram;
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
					tot_energy_pkg_uj += energy_pkg_world[i][j];
					tot_energy_dram_uj += energy_dram_world[i][j];
				}
				else
					flag = FALSE;
			}
		}
		tot_energy_pkg = ((double) tot_energy_pkg_uj) / 1.0E6;
		tot_energy_dram = ((double) tot_energy_dram_uj) / 1.0E6;

		printf("#####################################\n");
		printf("############# COUNTDOWN #############\n");
		printf("#####################################\n");
		printf("Execution time: %.3f sec\n", exe_time);
		printf("############### ENERGY ##############\n");
		printf("Package energy: %.3f J\n", tot_energy_pkg);
		printf("DRAM energy: %.3f J\n", tot_energy_dram);
		printf("Total energy: %.3f J\n", tot_energy_pkg + tot_energy_dram);
		printf("############# AVG POWER #############\n");
		printf("AVG Package power: %.2f W\n", tot_energy_pkg / exe_time);
		printf("AVG DRAM power: %.2f W\n", tot_energy_dram  / exe_time);
		printf("AVG power: %.2f W\n", (tot_energy_pkg + tot_energy_dram) / exe_time);
		printf("#####################################\n");
	}
}

static void read_energy(uint64_t *energy_pkg, uint64_t *energy_dram)
{
	int i, j;
	DIR* dir;
	char dirname[STRING_SIZE], filename[STRING_SIZE], name[STRING_SIZE], energy_str[STRING_SIZE];

	for(i = 0; i < NUM_SOCKETS; i++)
	{
		// Check all packages
		snprintf(dirname, STRING_SIZE, INTEL_RAPL_PKG, i);
		dir = opendir(dirname);
		if(dir)
		{
			closedir(dir);
			
			// Check if this domain is the package domain
			snprintf(filename, STRING_SIZE, INTEL_RAPL_PKG_NAME, i);
			if(read_str_from_file(filename, name) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			if(strstr(name, "package") != NULL)
			{
				// Read the package energy
				snprintf(filename, STRING_SIZE, PKG_ENERGY_UJ, i);
				if(read_str_from_file(filename, energy_str) < 0)
				{
					fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
					PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				}
				energy_pkg[i] = strtoul(energy_str, NULL, 10);

				// Find DRAM domain in this package
				for(j = 0; j < 3; j++)
				{
					snprintf(dirname, STRING_SIZE, INTEL_RAPL_DRAM, i, i, j);
					dir = opendir(dirname);
					if(dir) {
						closedir(dir);
						
						// Check if this domain is the dram domain
						snprintf(filename, STRING_SIZE, INTEL_RAPL_DRAM_NAME, i, i, j);
						if(read_str_from_file(filename, name) < 0)
						{
							fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
							PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
						}

						if(strstr(name, "dram") != NULL)
						{
							// Read the dram energy
							snprintf(filename, STRING_SIZE, DRAM_ENERGY_UJ, i, i, j);
							if(read_str_from_file(filename, energy_str) < 0)
							{
								fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
								PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
							}
							energy_dram[i] = strtoul(energy_str, NULL, 10);
						}
					}
				}
			}
		} 
	}
}

void make_sample(int sig, siginfo_t *siginfo, void *context)
{
	static int init = FALSE;
	static int flip = 0;
	static uint64_t energy_pkg[2][NUM_SOCKETS];
	static uint64_t energy_dram[2][NUM_SOCKETS];

	if(init == FALSE)
	{
		read_energy(energy_pkg[0], energy_dram[0]);
		init = TRUE;
	}
	else
	{
		int i;
		int prev = flip;
		flip = (flip == 0) ? 1 : 0;
		int curr = flip;

		read_energy(energy_pkg[curr], energy_dram[curr]);

		for(i = 0; i < NUM_SOCKETS; i++)
		{
			cntd->energy_pkg[i] += diff_overflow(energy_pkg[curr][i], 
				energy_pkg[prev][i], 
				cntd->energy_pkg_overflow[i]);
			cntd->energy_dram[i] += diff_overflow(energy_dram[curr][i], 
				energy_dram[prev][i], 
				cntd->energy_pkg_overflow[i]);
		}
	}
}

void start_cntd()
{
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));

	// Read p-state cnfigurations
	read_arch_conf();

	// Read environment variables
	read_env();

	// Init energy-aware MPI
	if(!cntd->no_eam)
	{
		if(cntd->enable_cntd)
			eam_init();
		else
			eam_slack_init();
	}

	// Synchronize ranks
	PMPI_Barrier(MPI_COMM_WORLD);

	// Start timer
	make_timer(&cntd->timer, &make_sample, cntd->sampling_time, cntd->sampling_time);

	// Read time
	cntd->exe_time[START] = read_time();

	// Read the energy counter of package and DRAM
	make_sample(0, NULL, NULL);
}

void stop_cntd()
{
	// Delete sampling timer
	delete_timer(cntd->timer);

	// Synchronize ranks
	PMPI_Barrier(MPI_COMM_WORLD);

	// Read time
	cntd->exe_time[END] = read_time();

	// Read the energy counter of package and DRAM
	make_sample(0, NULL, NULL);

	// Finalize energy-aware MPI
	if(!cntd->no_eam)
	{
		if(cntd->enable_cntd)
			eam_finalize();
		else
			eam_slack_finalize();
	}

	// Print the final report
	print_report();

	// Deallocate global variables
	free(cntd);
}

void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(!cntd->no_eam)
	{
		if(cntd->enable_cntd)
			eam_start_mpi();
		else
			eam_slack_start_mpi(mpi_type, comm, addr);
	}
}

void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(!cntd->no_eam)
	{
		if(cntd->enable_cntd)
			eam_end_mpi();
		else
			eam_slack_end_mpi(mpi_type, comm, addr);
	}
}