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

static void read_env()
{
	// Enable countdown
	char *cntd_enable = getenv("CNTD_ENABLE");
	if(cntd_enable != NULL)
	{ 
		if(strcasecmp(cntd_enable, "analysis") == 0)
		{
			cntd->enable_cntd = TRUE;
			cntd->enable_cntd_slack = FALSE;
			cntd->enable_eam_freq = FALSE;
		}
		else if(str_to_bool(cntd_enable))
		{
			cntd->enable_cntd = TRUE;
			cntd->enable_cntd_slack = FALSE;
			cntd->enable_eam_freq = TRUE;
		}
		else
		{
			fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> The option '%s' is not available for CNTD_ENABLE parameter\n", 
				cntd->node.hostname, cntd->rank->world_rank, cntd_enable);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	// Enable countdown slack
	char *cntd_slack_enable_str = getenv("CNTD_SLACK_ENABLE");
	if(cntd_slack_enable_str != NULL)
	{
		if(strcasecmp(cntd_slack_enable_str, "analysis") == 0)
		{
			cntd->enable_cntd = FALSE;
			cntd->enable_cntd_slack = TRUE;
			cntd->enable_eam_freq = FALSE;
		}
		else if(str_to_bool(cntd_slack_enable_str))
		{
			cntd->enable_cntd = FALSE;
			cntd->enable_cntd_slack = TRUE;
			cntd->enable_eam_freq = TRUE;
		}
		else
		{
			fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> The option '%s' is not available for CNTD_SLACK_ENABLE parameter\n", 
				cntd->node.hostname, cntd->rank->world_rank, cntd_slack_enable_str);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	// Set maximum p-state
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

	// Force the use of MSR (require root)
	char *cntd_force_msr = getenv("CNTD_FORCE_MSR");
	if(str_to_bool(cntd_force_msr))
		cntd->force_msr = TRUE;
	else
		cntd->force_msr = FALSE;

	// Timeout value for COUNTDOWN timer
	char *timeout_str = getenv("CNTD_TIMEOUT");
	if(timeout_str != NULL)
		cntd->eam_timeout = (double) strtoul(timeout_str, 0L, 10) / 1.0E6;
	else
		cntd->eam_timeout = DEFAULT_TIMEOUT;

	// Disable hardware monitor
	char *hw_monitor_str = getenv("CNTD_DISABLE_HW_MONITOR");
	if(str_to_bool(hw_monitor_str))
		cntd->enable_hw_monitor = FALSE;
	else
		cntd->enable_hw_monitor = TRUE;

	// Enable HW time-series report
	char *cntd_enable_hw_ts_report = getenv("CNTD_ENABLE_HW_TIMESERIES_REPORT");
	if(str_to_bool(cntd_enable_hw_ts_report))
		cntd->enable_hw_ts_report = TRUE;
	else
		cntd->enable_hw_ts_report = FALSE;

	// Sampling time
	char *hw_sampling_time_str = getenv("CNTD_HW_SAMPLING_TIME");
	if(hw_sampling_time_str != NULL)
		cntd->hw_sampling_time = strtoul(hw_sampling_time_str, 0L, 10);
	else
		cntd->hw_sampling_time = DEFAULT_SAMPLING_TIME_REPORT;

	// Enable MPI report per rank
	char *cntd_enable_rank_report = getenv("CNTD_ENABLE_RANK_REPORT");
	if(str_to_bool(cntd_enable_rank_report))
		cntd->enable_rank_report = TRUE;
	else
		cntd->enable_rank_report = FALSE;

	// Output directory
	char *output_dir = getenv("CNTD_OUT_DIR");
	if(output_dir != NULL && strcmp(output_dir, "") != 0)
	{
		strncpy(cntd->log_dir, output_dir, STRING_SIZE);

		// Create log dir
		int my_rank;
		PMPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
		if(my_rank == 0)
		{
			if(makedir(cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Cannot create directory: %s\n", 
					cntd->node.hostname, cntd->rank->world_rank, cntd->log_dir);
        		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	}
	else
	{
		if(getcwd(cntd->log_dir, STRING_SIZE) == NULL)
		{
			fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed to get path name of log directory!\n",
				cntd->node.hostname, cntd->rank->world_rank);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
	PMPI_Barrier(MPI_COMM_WORLD);

	// Check consistency
	if(cntd->user_pstate[MIN] != NO_CONF && cntd->user_pstate[MIN] < cntd->sys_pstate[MIN])
	{
		fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> User-defined min p-state cannot be lower \
			than the min system p-state (min system p-state = %d)!\n", 
			cntd->node.hostname, cntd->rank->world_rank, cntd->sys_pstate[MIN]);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	if(cntd->user_pstate[MAX] != NO_CONF && cntd->user_pstate[MAX] > cntd->sys_pstate[MAX])
	{
		fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> User-defined max p-state cannot be greater \
			than the max system p-state (max system p-state = %d)!\n", 
			cntd->node.hostname, cntd->rank->world_rank, cntd->sys_pstate[MAX]);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	if(cntd->user_pstate[MAX] != NO_CONF && cntd->user_pstate[MIN] != NO_CONF)
	{
		if(cntd->user_pstate[MIN] > cntd->user_pstate[MAX])
		{
			fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Max p-state cannot be greater than min p-state!\n",
				cntd->node.hostname, cntd->rank->world_rank);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	if(cntd->hw_sampling_time > MAX_SAMPLING_TIME_REPORT)
	{
		fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> The sampling time cannot exceed %d seconds!\n", 
			cntd->node.hostname, cntd->rank->world_rank, MAX_SAMPLING_TIME_REPORT);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

static void init_local_masters()
{
	int i;
	int world_rank, local_rank;
	char shmem_name[STRING_SIZE];
	char username[STRING_SIZE];

	// Get world rank
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Create local communicators and master communicators
	PMPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &cntd->comm_local);
	PMPI_Comm_rank(cntd->comm_local, &local_rank);
	PMPI_Comm_split(MPI_COMM_WORLD, local_rank, 0, &cntd->comm_local_masters);

	// Init shared memory
	getlogin_r(username, STRING_SIZE);
	if(username == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed to discover the username!\n",
			cntd->node.hostname, cntd->rank->world_rank);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, local_rank, username);
	cntd->local_ranks[local_rank] = create_shmem_rank(shmem_name, 1);
	cntd->rank = cntd->local_ranks[local_rank];

	PMPI_Comm_size(MPI_COMM_WORLD, &cntd->num_local_ranks);

	PMPI_Barrier(MPI_COMM_WORLD);

	cntd->rank->world_rank = world_rank;
	cntd->rank->local_rank = local_rank;

	for(i = 0; i < cntd->num_local_ranks; i++)
	{
		if(i == local_rank)
			continue;
		else
		{
			snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, i, username);
			cntd->local_ranks[i] = get_shmem_cpu(shmem_name, 1);
		}
	}
}

static void finalize_local_masters()
{
	char username[STRING_SIZE];
	char shmem_name[STRING_SIZE];

	getlogin_r(username, STRING_SIZE);
	if(username == NULL)
	{
		fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed to discover the username!\n",
			cntd->node.hostname, cntd->rank->world_rank);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, cntd->rank->local_rank, username);
	destroy_shmem_cpu(cntd->rank, 1, shmem_name);
}

HIDDEN void start_cntd()
{
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));

	// Init local masters
	init_local_masters();

	// Read P-state configurations
	init_arch_conf();

	// Read environment variables
	read_env();

	if(cntd->enable_hw_monitor)
		init_time_sample();

	// Init energy-aware MPI
	if(cntd->enable_cntd)
		eam_init();
	else if(cntd->enable_cntd_slack)
		eam_slack_init();
}

HIDDEN void stop_cntd()
{
	// Finalize energy-aware MPI
	if(cntd->enable_cntd)
		eam_finalize();
	else if(cntd->enable_cntd_slack)
		eam_slack_finalize();

	if(cntd->enable_hw_monitor)
		finalize_time_sample();
	print_final_report();

	finalize_local_masters();
	
	free(cntd);
}

// This is a prolog function for every intercepted MPI call
HIDDEN void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->enable_cntd)
		eam_start_mpi();
	else if(cntd->enable_cntd_slack)
		eam_slack_start_mpi(mpi_type, comm, addr);
	
	event_sample_start(mpi_type);
}

// This is a epilogue function for every intercepted MPI call
HIDDEN void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	int eam_flag = FALSE;

	if(cntd->enable_cntd)
		eam_flag = eam_end_mpi();
	else if(cntd->enable_cntd_slack)
		eam_flag = eam_slack_end_mpi(mpi_type, comm, addr);
	
	event_sample_end(mpi_type, eam_flag);
}
