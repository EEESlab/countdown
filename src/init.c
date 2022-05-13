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

CNTD_t *cntd;
#ifdef MOSQUITTO_ENABLED
MOSQUITTO_t* mosq;
#endif

static void read_env()
{
	int i, j, world_rank;
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

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
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> The option '%s' is not available for CNTD_ENABLE parameter\n", 
				hostname, world_rank, cntd_enable);
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
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> The option '%s' is not available for CNTD_SLACK_ENABLE parameter\n", 
				hostname, world_rank, cntd_slack_enable_str);
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
	char *hw_monitor_str = getenv("CNTD_DISABLE_POWER_MONITOR");
	if(str_to_bool(hw_monitor_str))
		cntd->enable_power_monitor = FALSE;
	else
		cntd->enable_power_monitor = TRUE;

	// Enable HW time-series report
	char *cntd_enable_ts_report = getenv("CNTD_ENABLE_TIMESERIES_REPORT");
	if(str_to_bool(cntd_enable_ts_report))
		cntd->enable_timeseries_report = TRUE;
	else
		cntd->enable_timeseries_report = FALSE;

	// Sampling time
	char *sampling_time_str = getenv("CNTD_SAMPLING_TIME");
	if(sampling_time_str != NULL)
		cntd->sampling_time = strtoul(sampling_time_str, 0L, 10);
	else
		cntd->sampling_time = DEFAULT_SAMPLING_TIME_REPORT;

	// Enable MPI report per rank
	char *cntd_enable_report = getenv("CNTD_ENABLE_REPORT");
	if(str_to_bool(cntd_enable_report))
		cntd->enable_report = TRUE;
	else
		cntd->enable_report = FALSE;

	// Enable perf
	char *cntd_disable_perf = getenv("CNTD_DISABLE_PERF");
	if(str_to_bool(cntd_disable_perf))
		cntd->enable_perf = FALSE;
	else
		cntd->enable_perf = TRUE;

	// Enable custom perf
	for(j = 0; j < MAX_NUM_CUSTOM_PERF; j++)
	{
		char perf_env[STRING_SIZE];
		snprintf(perf_env, sizeof(perf_env), "CNTD_PERF_EVENT_%d", j);
		char *cntd_perf_event = getenv(perf_env);
		if(cntd_perf_event != NULL)
			for(i = 0; i < cntd->local_rank_size; i++)
				cntd->perf_fd[i][j] = (int) strtoul(cntd_perf_event, 0L, 16);
		else
			for(i = 0; i < cntd->local_rank_size; i++)
				cntd->perf_fd[i][j] = 0;
	}

	// Output directory
	char *output_dir = getenv("CNTD_OUTPUT_DIR");
	if(output_dir != NULL && strcmp(output_dir, "") != 0)
	{
		strncpy(cntd->log_dir, output_dir, STRING_SIZE);

		// Create log dir
		if(world_rank == 0)
		{
			if(makedir(cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Cannot create output directory: %s\n", 
					hostname, world_rank, cntd->log_dir);
        		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	}
	else
	{
		if(getcwd(cntd->log_dir, STRING_SIZE) == NULL)
		{
			fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Failed to get path name of output directory!\n",
				hostname, world_rank);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	// Temporary directory
	char *tmp_dir = getenv("CNTD_TMP_DIR");
	if(tmp_dir != NULL && strcmp(tmp_dir, "") != 0)
	{
		strncpy(cntd->tmp_dir, tmp_dir, STRING_SIZE);

		// Create tmp dir
		if(world_rank == 0)
		{
			if(makedir(cntd->tmp_dir) < 0)
			{
				fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> Cannot create tmp directory: %s\n", 
					hostname, world_rank, cntd->tmp_dir);
        		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	}
	else
		strncpy(cntd->tmp_dir, cntd->log_dir, STRING_SIZE);

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->sampling_time > MAX_SAMPLING_TIME_REPORT)
	{
		fprintf(stderr, "Error: <COUNTDOWN-node:%s-rank:%d> The sampling time cannot exceed %d seconds!\n", 
			hostname, world_rank, MAX_SAMPLING_TIME_REPORT);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
}

static void init_local_masters()
{
	int i, local_master;
	int iam_local_master;
	int world_rank, local_rank, world_size;
	char hostname[STRING_SIZE];
	char postfix[STRING_SIZE], shmem_name[STRING_SIZE];

	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	PMPI_Comm_size(MPI_COMM_WORLD, &world_size);

	char global_hostname[world_size][STRING_SIZE];
	gethostname(hostname, sizeof(hostname));

	// Create local communicators and master communicators
	PMPI_Allgather(hostname, STRING_SIZE, MPI_CHAR, global_hostname, STRING_SIZE, MPI_CHAR, MPI_COMM_WORLD);

	// Find local master and local communicators
	for(i = 0; i < world_size; i++)
	{
		if(strncmp(hostname, global_hostname[i], STRING_SIZE) == 0)
		{
			local_master = i;
			break;
		}
	}
	if(world_rank == local_master)
		iam_local_master = TRUE;
	else
		iam_local_master = FALSE;
	PMPI_Comm_split(MPI_COMM_WORLD, iam_local_master, 0, &cntd->comm_local_masters);
	PMPI_Comm_split(MPI_COMM_WORLD, local_master, 0, &cntd->comm_local);
	PMPI_Comm_rank(cntd->comm_local, &local_rank);

	// Init shared memory
	get_rand_postfix(postfix, STRING_SIZE);
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, local_rank, postfix);
	cntd->local_ranks[local_rank] = create_shmem_rank(shmem_name, 1);
	cntd->rank = cntd->local_ranks[local_rank];
	cntd->rank->exe_is_started = 0;

	PMPI_Comm_size(cntd->comm_local, &cntd->local_rank_size);

	PMPI_Barrier(MPI_COMM_WORLD);

	cntd->rank->world_rank = world_rank;
	cntd->rank->local_rank = local_rank;

	for(i = 0; i < cntd->local_rank_size; i++)
	{
		if(i == local_rank)
			continue;
		else
		{
			snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, i, postfix);
			cntd->local_ranks[i] = get_shmem_cpu(shmem_name, 1);
		}
	}
}

static void finalize_local_masters()
{
	char postfix[STRING_SIZE], shmem_name[STRING_SIZE];
	
	get_rand_postfix(postfix, STRING_SIZE);
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, cntd->rank->local_rank, postfix);
	destroy_shmem_cpu(cntd->rank, 1, shmem_name);
}

HIDDEN void start_cntd()
{
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));

	hwp_usage = 0;

	// Init local masters
	init_local_masters();

	// Read environment variables
	read_env();

	// Init PM
	if(cntd->enable_eam_freq) {
		pm_init();
		// Checking HWP-States' usability.
#ifdef HWP_AVAIL
		uint64_t pstate;

		pstate = read_msr(IA32_PM_ENABLE);

		if (pstate)
			hwp_usage = 1;
		else
			fprintf(stdout, "Warning: HWP-States available, but not usable.\n");
#endif
	}

	// Read P-state configurations
	init_arch_conf();

#ifdef MOSQUITTO_ENABLED
	if(cntd->rank->local_rank == 0) {
		char client_id[STRING_SIZE];

		memset(client_id,
			   0	   	,
			   STRING_SIZE);
		snprintf(client_id						  ,
				 STRING_SIZE					  ,
				 "COUNTDOWN-MQTT-node:%s-rank:%d,",
				 cntd->node.hostname			  ,
				 cntd->rank->world_rank);

		mosquitto_lib_init();

		mosq = mosquitto_new(client_id,
							 true	  ,
							 0);
		mosquitto_username_pw_set(mosq,
								  "your_username",
								  "your_password");
	}
#endif

	// Init the node sampling
	init_time_sample();

	if(cntd->enable_timeseries_report)
		init_timeseries_report();

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

	finalize_time_sample();

#ifdef MOSQUITTO_ENABLED
	if(cntd->rank->local_rank == 0) {
		mosquitto_destroy(mosq);

		mosquitto_lib_cleanup();
	}
#endif

	// Finalize PM
	if(cntd->enable_eam_freq)
		pm_finalize();

	print_final_report();
	
	if(cntd->enable_timeseries_report)
		finalize_timeseries_report();

	finalize_local_masters();
	
	free(cntd);
}

// This is a prolog function for every intercepted MPI call
HIDDEN void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	cntd->into_mpi = TRUE;

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

	cntd->into_mpi = FALSE;
}
