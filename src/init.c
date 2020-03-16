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

void start_cntd()
{
	cntd = (CNTD_t *) malloc(sizeof(CNTD_t));

	// P-states
  	FILE *fd_min = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", "r");
	FILE *fd_max = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");

	if(fd_min == NULL || fd_max == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed read file /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq and/or cpuinfo_max_freq!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	if(fscanf(fd_min, "%d", &min_pstate) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to read the minimum P-state!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	min_pstate /= 1E5;

	if(fscanf(fd_max, "%d", &max_pstate) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to read the minimum P-state!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	max_pstate /= 1E5;

	fclose(fd_min);
	fclose(fd_max);

	if(max_pstate < min_pstate)
	{
		fprintf(stderr, "Error: <countdown> User-defined P-state cannot less than minimum P-state!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

void stop_cntd()
{
	free(cntd);
}

void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	
}

void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	
}
