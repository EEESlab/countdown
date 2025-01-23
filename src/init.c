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
#include <strings.h>

CNTD_t *cntd;
#ifdef MOSQUITTO_ENABLED
MOSQUITTO_t *mosq;
#endif

static void read_env()
{
	int i, j, world_rank;
	char hostname[STRING_SIZE];

	gethostname(hostname, sizeof(hostname));
	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Enable countdown
	char *cntd_eam_enable = getenv("CNTD_EAM_ENABLE");
	if (cntd_eam_enable != NULL) {
		if (strcasecmp(cntd_eam_enable, "analysis") == 0) {
			cntd->enable_eam = TRUE;
			cntd->enable_eam_analysis = TRUE;
		} else if (str_to_bool(cntd_eam_enable)) {
			// Enable frequency shifting (EAM)
			cntd->enable_eam = TRUE;
		} else {
			fprintf(stderr,
				"Error: <COUNTDOWN-node:%s-rank:%d> The option '%s' is not available for CNTD_EAM_ENABLE parameter\n",
				hostname, world_rank, cntd_eam_enable);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	} else {
		// Enable countdown slack
		char *cntd_eam_slack_enable_str =
			getenv("CNTD_EAM_SLACK_ENABLE");
		if (cntd_eam_slack_enable_str != NULL) {
			if (strcasecmp(cntd_eam_slack_enable_str, "analysis") ==
			    0) {
				cntd->enable_eam_slack = TRUE;
				cntd->enable_eam_analysis = TRUE;
			} else if (str_to_bool(cntd_eam_slack_enable_str)) {
				cntd->enable_eam_slack = TRUE;
			} else {
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> The option '%s' is not available for CNTD_EAM_SLACK_ENABLE parameter\n",
					hostname, world_rank,
					cntd_eam_slack_enable_str);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	}

	// Set maximum frequency
	char *max_freq_str = getenv("CNTD_MAX_FREQ_MHZ");
	if (max_freq_str != NULL)
		cntd->user_freq_khz[MAX] =
			strtoul(max_freq_str, 0L, 10) * 1.0E3;
	else
		cntd->user_freq_khz[MAX] = NO_CONF;

	// Set minimum frequency
	char *min_freq_str = getenv("CNTD_MIN_FREQ_MHZ");
	if (min_freq_str != NULL)
		cntd->user_freq_khz[MIN] =
			strtoul(min_freq_str, 0L, 10) * 1.0E3;
	else
		cntd->user_freq_khz[MIN] = NO_CONF;

	// Force the use of MSR (require root)
	char *cntd_force_msr = getenv("CNTD_FORCE_MSR");
	if (str_to_bool(cntd_force_msr))
		cntd->force_msr = TRUE;
	else
		cntd->force_msr = FALSE;

	// Timeout value for COUNTDOWN timer
	char *timeout_str = getenv("CNTD_TIMEOUT");
	if (timeout_str != NULL)
		cntd->eam_timeout =
			(double)strtoul(timeout_str, 0L, 10) / 1.0E6;
	else
		cntd->eam_timeout = DEFAULT_TIMEOUT;

	// Disable hardware monitor
	char *hw_monitor_str = getenv("CNTD_DISABLE_POWER_MONITOR");
	if (str_to_bool(hw_monitor_str))
		cntd->enable_power_monitor = FALSE;
	else
		cntd->enable_power_monitor = TRUE;

	// Enable time-series report
	char *cntd_enable_ts_report = getenv("CNTD_ENABLE_TIMESERIES_REPORT");
	if (str_to_bool(cntd_enable_ts_report))
		cntd->enable_timeseries_report = TRUE;
	else
		cntd->enable_timeseries_report = FALSE;

	// Sampling time
	char *sampling_time_str = getenv("CNTD_SAMPLING_TIME");
	if (sampling_time_str != NULL) {
		cntd->sampling_time = strtoul(sampling_time_str, 0L, 10);
		if (cntd->sampling_time > MAX_SAMPLING_TIME_REPORT) {
			fprintf(stderr,
				"Error: <COUNTDOWN-node:%s-rank:%d> The sampling time cannot exceed %d seconds!\n",
				hostname, world_rank, MAX_SAMPLING_TIME_REPORT);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	} else
		cntd->sampling_time = DEFAULT_SAMPLING_TIME_REPORT;

	// Enable MPI report per rank
	char *cntd_enable_report = getenv("CNTD_ENABLE_REPORT");
	if (str_to_bool(cntd_enable_report))
		cntd->enable_report = TRUE;
	else
		cntd->enable_report = FALSE;

	// Enable perf
	char *cntd_disable_perf = getenv("CNTD_DISABLE_PERF");
	if (str_to_bool(cntd_disable_perf))
		cntd->enable_perf = FALSE;
	else
		cntd->enable_perf = TRUE;

	// Enable custom perf
	for (j = 0; j < MAX_NUM_CUSTOM_PERF; j++) {
		char perf_env[STRING_SIZE];
		snprintf(perf_env, sizeof(perf_env), "CNTD_PERF_EVENT_%d", j);
		char *cntd_perf_event = getenv(perf_env);
		if (cntd_perf_event != NULL)
			for (i = 0; i < cntd->rank->local_size; i++)
				cntd->perf_fd[i][j] =
					(int)strtoul(cntd_perf_event, 0L, 16);
		else
			for (i = 0; i < cntd->rank->local_size; i++)
				cntd->perf_fd[i][j] = 0;
	}

	// Output directory
	char *output_dir = getenv("CNTD_OUTPUT_DIR");
	if (output_dir != NULL && strcmp(output_dir, "") != 0) {
		strncpy(cntd->log_dir, output_dir, STRING_SIZE);

		// Create log dir
		if (world_rank == 0) {
			if (makedir(cntd->log_dir) < 0) {
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> Cannot create output directory: %s\n",
					hostname, world_rank, cntd->log_dir);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	} else {
		if (getcwd(cntd->log_dir, STRING_SIZE) == NULL) {
			fprintf(stderr,
				"Error: <COUNTDOWN-node:%s-rank:%d> Failed to get path name of output directory!\n",
				hostname, world_rank);
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}

	// Temporary directory
	char *tmp_dir = getenv("CNTD_TMP_DIR");
	if (tmp_dir != NULL && strcmp(tmp_dir, "") != 0) {
		strncpy(cntd->tmp_dir, tmp_dir, STRING_SIZE);

		// Create tmp dir
		if (world_rank == 0) {
			if (makedir(cntd->tmp_dir) < 0) {
				fprintf(stderr,
					"Error: <COUNTDOWN-node:%s-rank:%d> Cannot create tmp directory: %s\n",
					hostname, world_rank, cntd->tmp_dir);
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}
		}
	} else
		strncpy(cntd->tmp_dir, CNTD_TMP_DIR, STRING_SIZE);

	// CPUFREQ
	char *use_cpufreq = getenv("CNTD_USE_CPUFREQ");
	if (str_to_bool(use_cpufreq))
		cntd->use_cpufreq = TRUE;
	else
		cntd->use_cpufreq = FALSE;

	PMPI_Barrier(MPI_COMM_WORLD);
}

static void init_masters()
{
	int i;
	int world_rank, local_rank, world_size;
	char hostname[STRING_SIZE];
	char postfix[STRING_SIZE], shmem_name[STRING_SIZE];

	PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	PMPI_Comm_size(MPI_COMM_WORLD, &world_size);

	char global_hostname[world_size][STRING_SIZE];
	gethostname(hostname, sizeof(hostname));

	// Create local communicators and master communicators
	PMPI_Allgather(hostname, STRING_SIZE, MPI_CHAR, global_hostname,
		       STRING_SIZE, MPI_CHAR, MPI_COMM_WORLD);

	// Find local master and local communicators
	for (i = 0; i < world_size; i++) {
		if (strncmp(hostname, global_hostname[i], STRING_SIZE) == 0) {
			cntd->master_rank = i;
			break;
		}
	}
	if (world_rank == cntd->master_rank)
		cntd->iam_master = TRUE;
	// Create local masters' group communicator  aka group of masters
	PMPI_Comm_split(MPI_COMM_WORLD, cntd->iam_master, 0,
			&cntd->comm_masters);
	// Create node communicator aka master + slaves
	PMPI_Comm_split(MPI_COMM_WORLD, cntd->master_rank, 0,
			&cntd->comm_local);
	PMPI_Comm_rank(cntd->comm_local, &local_rank);

	PMPI_Comm_size(cntd->comm_local, &cntd->rank->local_size);
	cntd->rank->world_size = world_size;

	PMPI_Barrier(MPI_COMM_WORLD);

	cntd->rank->world_rank = world_rank;
	cntd->rank->local_rank = local_rank;
}

static void init_shmem()
{
	char postfix[STRING_SIZE], shmem_name[STRING_SIZE];

	// Init shared memory
	get_rand_postfix(postfix, STRING_SIZE);
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE,
		 cntd->rank->local_rank, postfix);
	cntd->local_ranks[cntd->rank->local_rank] =
		create_shmem_rank(shmem_name, 1);
	cntd->rank = cntd->local_ranks[cntd->rank->local_rank];

	// Get shared memory for other local tasks
	for (int i = 0; i < cntd->rank->local_size; i++) {
		if (i == cntd->rank->local_rank)
			continue;
		else {
			snprintf(shmem_name, sizeof(shmem_name), SHM_FILE, i,
				 postfix);
			cntd->local_ranks[i] = get_shmem_cpu(shmem_name, 1);
		}
	}
}
static void finalize_shmem()
{
	char postfix[STRING_SIZE], shmem_name[STRING_SIZE];

	get_rand_postfix(postfix, STRING_SIZE);
	snprintf(shmem_name, sizeof(shmem_name), SHM_FILE,
		 cntd->rank->local_rank, postfix);
	destroy_shmem_cpu(cntd->rank, 1, shmem_name);
}

HIDDEN void start_cntd()
{
	cntd = (CNTD_t *)calloc(1, sizeof(CNTD_t));

	// Init local masters
	init_masters();

	// Init shared memory
	init_shmem();

	// Read environment variables
	read_env();

	// Init PM
	if (cntd->enable_eam || cntd->enable_eam_slack) {
		pm_init();
		// Read P-state configurations
		// Opening frequency files
		init_cpufreq();
	}

	// Read architecture configuration + get pid + get cpu id
	init_arch_conf();

#ifdef MOSQUITTO_ENABLED
	if (cntd->iam_master) {
		init_mosquitto();
	}
#endif

	// Init the node sampling
	init_time_sample();

	if (cntd->enable_timeseries_report)
		init_timeseries_report();

	// Init energy-aware MPI
	if (cntd->enable_eam)
		eam_init();
	else if (cntd->enable_eam_slack)
		eam_slack_init();
}

HIDDEN void stop_cntd()
{
	// Finalize energy-aware MPI
	if (cntd->enable_eam)
		eam_finalize();
	else if (cntd->enable_eam_slack)
		eam_slack_finalize();

	finalize_time_sample();

#ifdef MOSQUITTO_ENABLED
	if (cntd->iam_master) {
		mosquitto_destroy(mosq);

		mosquitto_lib_cleanup();
	}
#endif

	if (cntd->enable_eam || cntd->enable_eam_slack) {
		if (!cntd->enable_eam_analysis) {
			if (cntd->use_cpufreq) {
				if (!cntd->userspace_governor) {
					char filename[STRING_SIZE];

					snprintf(filename, STRING_SIZE,
						 SCALING_MAX_FREQ,
						 cntd->rank->cpu_id);
					write_int_to_file(
						filename,
						cntd->scaling_max_freq_fd,
						cntd->sys_freq_khz[MAX]);

					snprintf(filename, STRING_SIZE,
						 SCALING_MIN_FREQ,
						 cntd->rank->cpu_id);
					write_int_to_file(
						filename,
						cntd->scaling_min_freq_fd,
						cntd->sys_freq_khz[MIN]);
				}
			}
			// Finalize PM
			pm_finalize();
		}
	}

	finalize_cpufreq();

	print_final_report();

	if (cntd->enable_timeseries_report)
		finalize_timeseries_report();

	finalize_shmem();

	free(cntd);
}

// This is a prolog function for every intercepted MPI call
HIDDEN void call_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if (cntd->enable_eam)
		eam_start_mpi();
	else if (cntd->enable_eam_slack)
		eam_slack_start_mpi(mpi_type, comm, addr);

	event_sample_start(mpi_type);
}

// This is a epilogue function for every intercepted MPI call
HIDDEN void call_end(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	int eam_flag = FALSE;

	if (cntd->enable_eam)
		eam_flag = eam_end_mpi();
	else if (cntd->enable_eam_slack)
		eam_flag = eam_slack_end_mpi(mpi_type, comm, addr);

	event_sample_end(mpi_type, eam_flag);
}

#ifdef MOSQUITTO_ENABLED
HIDDEN void init_mosquitto()
{
	char client_id[STRING_SIZE];

	memset(client_id, 0, STRING_SIZE);
	snprintf(client_id, STRING_SIZE, "COUNTDOWN-MQTT-node:%s-rank:%d,",
		 cntd->node.hostname, cntd->rank->world_rank);

	mosquitto_lib_init();

	mosq = mosquitto_new(client_id, true, 0);
	mosquitto_username_pw_set(mosq, "your_username", "your_password");
}
#endif
