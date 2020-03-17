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
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));

	// Read minimum P-state
  	FILE *fd_min = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", "r");
	if(fd_min == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed read file /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	if(fscanf(fd_min, "%d", &(cntd->pstate[MIN])) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to read the minimum P-state!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->pstate[MIN] /= 1E5;
	fclose(fd_min);

	// Read maximum P-state
	FILE *fd_max = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
	if(fd_max == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed read file /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	if(fscanf(fd_max, "%d", &(cntd->pstate[MAX])) < 0)
	{
		fprintf(stderr, "Error: <countdown> Failed to read the maximum P-state!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->pstate[MAX] /= 1E5;
	fclose(fd_max);

	// Check maximum p-state greater than minimum p-state
	if(cntd->pstate[MAX] < cntd->pstate[MIN])
	{
		fprintf(stderr, "Error: <countdown> Maximum P-state cannot less than minimum P-state of the system!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	// Init energy-aware MPI
	eam_init();

	// Synchronize all ranks
	PMPI_Barrier(MPI_COMM_WORLD);
}

void stop_cntd()
{
	// Finalize energy-aware MPI
	eam_finalize();

	// Deallocate global variables
	free(cntd);
}

void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	eam_start_mpi();
}

void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	eam_end_mpi();
}
