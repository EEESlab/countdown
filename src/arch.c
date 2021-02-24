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

HIDDEN void init_arch_conf()
{
	int i, j;
	DIR* dir;
	char dirname[STRING_SIZE], filename[STRING_SIZE], filevalue[STRING_SIZE];

	// Get hostname
	char host[STRING_SIZE];
	gethostname(cntd->node.hostname, sizeof(host));
	gethostname(cntd->cpu.hostname, sizeof(host));

	// Get number of cpus
	cntd->node.num_cpus = get_nprocs();

	// Get cpu id
	cntd->cpu.cpu_id = sched_getcpu();

	// Get socket id
	snprintf(filename, STRING_SIZE, PACKAGE_ID, cntd->cpu.cpu_id);
	if(read_str_from_file(filename, filevalue) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->cpu.socket_id = strtoul(filevalue, NULL, 10);

	// Read minimum p-state
	char min_pstate_value[STRING_SIZE];
	if(read_str_from_file(CPUINFO_MIN_FREQ, min_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", CPUINFO_MIN_FREQ);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->sys_pstate[MIN] = (int) (strtof(min_pstate_value, NULL) / 1.0E5);

	// Read maximum p-state
	char max_pstate_value[STRING_SIZE];
	if(read_str_from_file(CPUINFO_MAX_FREQ, max_pstate_value) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", CPUINFO_MAX_FREQ);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->sys_pstate[MAX] = (int) (strtof(max_pstate_value, NULL) / 1.0E5);

    cntd->num_sampling = 0;
    
	// Check all packages
	for(i = 0; i < MAX_NUM_SOCKETS; i++)
	{
		// Chech if i-th socket exist
		snprintf(dirname, STRING_SIZE, INTEL_RAPL_PKG, i);
		dir = opendir(dirname);
		if(dir)
		{
			closedir(dir);
			
			// Check if this domain is the package domain
			snprintf(filename, STRING_SIZE, INTEL_RAPL_PKG_NAME, i);
			if(read_str_from_file(filename, filevalue) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
			if(strstr(filevalue, "package") != NULL)
			{
				// Increment the number of socket discovered
				cntd->node.num_sockets++;

				// Get socket id
				int socket_id;
				sscanf(filevalue, "package-%d", &socket_id);

				// Find sysfs file of RAPL for package energy measurements
				snprintf(cntd->node.energy_pkg_file[socket_id], STRING_SIZE, PKG_ENERGY_UJ, i);

				// Read the energy overflow value
				snprintf(filename, STRING_SIZE, PKG_MAX_ENERGY_RANGE_UJ, i);
				if(read_str_from_file(filename, filevalue) < 0)
				{
					fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
					PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				}
				cntd->node.energy_pkg_overflow[socket_id] = strtoul(filevalue, NULL, 10);

				// Find DRAM domain in this package
				for(j = 0; j < 3; j++)
				{
					snprintf(dirname, STRING_SIZE, INTEL_RAPL_DRAM, i, i, j);
					dir = opendir(dirname);
					if(dir) {
						closedir(dir);
						
						// Check if this domain is the dram domain
						snprintf(filename, STRING_SIZE, INTEL_RAPL_DRAM_NAME, i, i, j);
						if(read_str_from_file(filename, filevalue) < 0)
						{
							fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
							PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
						}

						if(strstr(filevalue, "dram") != NULL)
						{
							// Open sysfs file of RAPL for dram energy measurements
							snprintf(cntd->node.energy_dram_file[socket_id], STRING_SIZE, DRAM_ENERGY_UJ, i, i, j);

							// Read the dram energy
							snprintf(filename, STRING_SIZE, DRAM_MAX_ENERGY_RANGE_UJ, i, i, j);
							if(read_str_from_file(filename, filevalue) < 0)
							{
								fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
								PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
							}
							cntd->node.energy_dram_overflow[socket_id] = strtoul(filevalue, NULL, 10);
						}
					}
				}
			}
		} 
	}
}
