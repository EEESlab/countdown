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

void init_arch_conf()
{
	int i, j;
	DIR* dir;
	char dirname[STRING_SIZE], filename[STRING_SIZE], name[STRING_SIZE], energy_overflow[STRING_SIZE];

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

    cntd->sampling_cnt[CURR] = 0;
    cntd->sampling_cnt[MAX] = MEM_SIZE;
    
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
				// Increment the number of socket discovered
				cntd->num_sockets++;

				// Get socket id
				int socket_id;
				sscanf(name, "package-%d", &socket_id);

				// Find sysfs file of RAPL for package energy measurements
				snprintf(cntd->energy_pkg_name[socket_id], STRING_SIZE, PKG_ENERGY_UJ, i);

				// Read the energy overflow value
				snprintf(filename, STRING_SIZE, PKG_MAX_ENERGY_RANGE_UJ, i);
				if(read_str_from_file(filename, energy_overflow) < 0)
				{
					fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
					PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
				}
				cntd->energy_pkg_overflow[socket_id] = strtoul(energy_overflow, NULL, 10);

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
							// Open sysfs file of RAPL for dram energy measurements
							snprintf(cntd->energy_dram_name[socket_id], STRING_SIZE, DRAM_ENERGY_UJ, i, i, j);

							// Read the dram energy
							snprintf(filename, STRING_SIZE, DRAM_MAX_ENERGY_RANGE_UJ, i, i, j);
							if(read_str_from_file(filename, energy_overflow) < 0)
							{
								fprintf(stderr, "Error: <countdown> Failed read file '%s'!\n", filename);
								PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
							}
							cntd->energy_dram_overflow[socket_id] = strtoul(energy_overflow, NULL, 10);
						}
					}
				}
			}
		} 
	}
}