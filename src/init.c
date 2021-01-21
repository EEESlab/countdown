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
	// Enable countdown
	char *cntd_enable = getenv("CNTD_ENABLE");
	if(str_to_bool(cntd_enable))
		cntd->enable_cntd = TRUE;

	// Enable countdown slack
	char *enable_cntd_slack = getenv("CNTD_SLACK_ENABLE");
	if(str_to_bool(enable_cntd_slack))
		cntd->enable_cntd_slack = TRUE;

	// Disable P2P MPIs
	char *cntd_no_p2p = getenv("CNTD_NO_P2P");
	if(str_to_bool(cntd_no_p2p))
		cntd->no_p2p = TRUE;

	// Disable frequency selection
	char *cntd_no_freq = getenv("CNTD_NO_FREQ");
	if(str_to_bool(cntd_no_freq))
		cntd->no_freq = TRUE;

	// Enable sampling report
	char *cntd_sampling_report = getenv("CNTD_SAMPLING_REPORT");
	if(str_to_bool(cntd_sampling_report))
		cntd->sampling_report = TRUE;

	// Force the use of MSR
	char *cntd_force_msr = getenv("CNTD_FORCE_MSR");
	if(str_to_bool(cntd_force_msr))
		cntd->force_msr = TRUE;

	// Used-defined max and min p-states
	char *max_pstate_str = getenv("CNTD_MAX_PSTATE");
	if(max_pstate_str != NULL)
		cntd->user_pstate[MAX] = strtoul(max_pstate_str, 0L, 10);
	else
		cntd->user_pstate[MAX] = NO_CONF;

	// Set minimum p-state
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

void start_cntd()
{
	int i;

	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));
	for(i = 0; i < NUM_SOCKETS; i++)
	{
		cntd->energy_pkg_sampling[i] = (uint64_t *) malloc(MEM_SIZE * sizeof(uint64_t));
		cntd->energy_dram_sampling[i] = (uint64_t *) malloc(MEM_SIZE * sizeof(uint64_t));
	}

	// Read p-state cnfigurations
	init_arch_conf();

	// Read environment variables
	read_env();

	// Init energy-aware MPI
	if(cntd->enable_cntd)
		eam_init();
	else if(cntd->enable_cntd_slack)
		eam_slack_init();

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
	int i;

	// Delete sampling timer
	delete_timer(cntd->timer);

	// Synchronize ranks
	PMPI_Barrier(MPI_COMM_WORLD);

	// Read time
	cntd->exe_time[END] = read_time();

	// Read the energy counter of package and DRAM
	make_sample(0, NULL, NULL);

	// Finalize energy-aware MPI
	if(cntd->enable_cntd)
		eam_finalize();
	else if(cntd->enable_cntd_slack)
		eam_slack_finalize();

	// Print the final report
	print_report();

	// Deallocate global variables
	for(i = 0; i < NUM_SOCKETS; i++)
	{
		free(cntd->energy_pkg_sampling[i]);
		free(cntd->energy_dram_sampling[i]);
	}
	free(cntd);
}

// This is a prolog function for every intercepted MPI call
void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->enable_cntd)
		eam_start_mpi();
	else if(cntd->enable_cntd_slack)
		eam_slack_start_mpi(mpi_type, comm, addr);
}

// This is a epilogue function for every intercepted MPI call
void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->enable_cntd)
		eam_end_mpi();
	else if(cntd->enable_cntd_slack)
		eam_slack_end_mpi(mpi_type, comm, addr);
}