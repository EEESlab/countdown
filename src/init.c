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

static void init_libmsr()
{
	char hostname[STRING_SIZE];
	gethostname(hostname, sizeof(hostname));

	// Init library
	if(init_msr())
	{
		libmsr_error_handler("Unable to initialize libmsr", LIBMSR_ERROR_MSR_INIT, hostname, __FILE__, __LINE__);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	cntd->ri_stat = rapl_init(&cntd->rd, &cntd->rapl_flags);
	if (cntd->ri_stat < 0)
	{
		libmsr_error_handler("Unable to initialize rapl", LIBMSR_ERROR_RAPL_INIT, hostname, __FILE__, __LINE__);
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	enable_fixed_counters();
	enable_pmc();
}

static void finalize_libmsr()
{
	//disable_fixed_counters();
	//clear_all_pmc();
	finalize_msr();
}

static void read_env()
{
	// Outputs
	char *output_dir = getenv("CNTD_OUT_DIR");

	// Actions
	char *barrier_str = getenv("CNTD_BARRIER");
	char *fermata_str = getenv("CNTD_FERMATA");
	char *andante_str = getenv("CNTD_ANDANTE");
	char *adagio_str = getenv("CNTD_ADAGIO");
	char *eam_slack_str = getenv("CNTD_EAM_SLACK");
	char *eam_call_str = getenv("CNTD_EAM_CALL");

	// P-state
	char *max_pstate_str = getenv("CNTD_MAX_PSTATE");
	char *min_pstate_str = getenv("CNTD_MIN_PSTATE");

	// Analisys
	char *fermata_analysis_str = getenv("CNTD_FERMATA_ANALYSIS");
	char *andante_analysis_str = getenv("CNTD_ANDANTE_ANALYSIS");
	char *adagio_analysis_str = getenv("CNTD_ADAGIO_ANALYSIS");
	char *eam_slack_analysis_str = getenv("CNTD_EAM_SLACK_ANALYSIS");
	char *eam_call_analysis_str = getenv("CNTD_EAM_CALL_ANALYSIS");

	// Tracing
	char *time_trace_str = getenv("CNTD_TIME_TRACE");
	char *event_trace_str = getenv("CNTD_EVENT_TRACE");
	char *task_trace_str = getenv("CNTD_TASK_TRACE");

	// Advanced metrics
	char *pmc_str = getenv("CNTD_PMC");
	char *pcu_str = getenv("CNTD_PCU");
	char *debug_metrics_str = getenv("CNTD_DEBUG_METRICS");

	// Timeout
	char *timeout_str = getenv("CNTD_TIMEOUT");

	// Read env variables
	if(output_dir != NULL && strcmp(output_dir, "") != 0)
		strncpy(cntd->log_dir, output_dir, strlen(output_dir));
	else
	{
		char *ret = getcwd(cntd->log_dir, STRING_SIZE);
		if(ret == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed getcwd!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
	PMPI_Barrier(MPI_COMM_WORLD);

	// Create log dir
	if(cntd->rank->my_rank == CNTD_MPI_ROOT)
		create_dir(cntd->log_dir);
	PMPI_Barrier(MPI_COMM_WORLD);

	if(timeout_str != NULL)
		cntd->timeout = atof(timeout_str);
	else
		cntd->timeout = DEFAULT_TIMEOUT;

	if(str_to_bool(event_trace_str))
	{
		cntd->event_trace = TRUE;
		open_event_trace_file();
	}

	if(str_to_bool(task_trace_str))
	{
		cntd->barrier = TRUE;
		cntd->task_trace = TRUE;
		andante_init();
		open_task_trace_file();
	}

	if(str_to_bool(time_trace_str))
		cntd->time_trace = TRUE;

	if(str_to_bool(barrier_str))
		cntd->barrier = TRUE;

	if(str_to_bool(andante_str) ||
		 str_to_bool(adagio_str) ||
		 str_to_bool(andante_analysis_str) ||
		 str_to_bool(adagio_analysis_str))
	{
		cntd->barrier = TRUE;

		if(str_to_bool(andante_str) ||
			 str_to_bool(adagio_str))
			cntd->andante = TRUE;

		if(str_to_bool(andante_analysis_str) ||
			 str_to_bool(adagio_analysis_str))
			cntd->andante_analysis = TRUE;

		andante_init();
	}

	if(str_to_bool(eam_slack_str) ||
		 str_to_bool(eam_slack_analysis_str))
	{
		cntd->barrier = TRUE;
		if(str_to_bool(eam_slack_str))
			cntd->eam_slack = TRUE;

		if(str_to_bool(eam_slack_analysis_str))
			cntd->eam_slack_analysis = TRUE;

		eam_slack_init();
	}
	else if(str_to_bool(eam_call_str) ||
				  str_to_bool(eam_call_analysis_str))
	{
		if(str_to_bool(eam_call_str))
			cntd->eam_call = TRUE;

		if(str_to_bool(eam_call_analysis_str))
			cntd->eam_call_analysis = TRUE;

		eam_init();
	}
	else if(str_to_bool(fermata_str) ||
				  str_to_bool(adagio_str) ||
					str_to_bool(fermata_analysis_str) ||
					str_to_bool(adagio_analysis_str))
	{
		if(str_to_bool(fermata_str) ||
			 str_to_bool(adagio_str))
			cntd->fermata = TRUE;

		if(str_to_bool(fermata_analysis_str) ||
			 str_to_bool(adagio_analysis_str))
			cntd->fermata_analysis = TRUE;

		fermata_init();
	}

	if(min_pstate_str != NULL)
		cntd->arch.pstate[MIN] = atof(min_pstate_str);

	if(max_pstate_str != NULL)
		cntd->arch.pstate[MAX] = atof(max_pstate_str);

	if(str_to_bool(pmc_str))
		cntd->pmc = TRUE;

	if(str_to_bool(pcu_str))
		cntd->pcu = TRUE;

	if(str_to_bool(debug_metrics_str))
		cntd->debug_metrics = TRUE;
}

static void init_local_masters()
{
	int i;
	char shmem_name[STRING_SIZE];

	// Initialize local masters
	if(cntd->my_local_rank == CNTD_MPI_ROOT)
	{
		// Rank
		cntd->last_batch_ranks = create_shmem_rank("/cntd_last_batch_local_ranks", cntd->local_size);

		// CPUs
		cntd->cpu = create_shmem_cpu("/cntd_cpus");
		cntd->last_batch_cpus = create_shmem_cpu("/cntd_last_batch_cpus");
		for(i = 0; i < cntd->arch.cpus; i++)
		{
			cntd->cpu[i].cpu_id = i;
			strcpy(cntd->cpu[i].hostname, cntd->arch.hostname);

			cntd->cpu[i].num_samples = 0;
			cntd->last_batch_cpus[i].num_samples = 1;
		}

		// Sockets
		cntd->socket = create_shmem_socket("/cntd_sockets");
		cntd->last_batch_sockets = create_shmem_socket("/cntd_last_batch_sockets");
		for(i = 0; i < cntd->arch.sockets; i++)
		{
			cntd->socket[i].socket_id = i;
			strcpy(cntd->socket[i].hostname, cntd->arch.hostname);

			cntd->socket[i].rapl_joules = cntd->ru->joules;
			cntd->socket[i].rapl_watts = cntd->ru->watts;
			cntd->socket[i].rapl_seconds = cntd->ru->seconds;

			cntd->socket[i].num_samples = 0;
			cntd->last_batch_sockets[i].num_samples = 1;
		}

		// Enable socket perf counters
		enable_pcu();
		enable_uncore_freq();

		// Create time trace file
		if(cntd->time_trace)
		{
			// Create directory
			char time_trace_dir[STRING_SIZE];
			if(snprintf(time_trace_dir, sizeof(time_trace_dir), "%s/cntd_time_trace", cntd->log_dir) < 0)
			{
				fprintf(stderr, "Error: <countdown> Failed to create the name of the time trace file!\n");
				PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
			}

			if(cntd->rank->my_rank == CNTD_MPI_ROOT)
				create_dir(time_trace_dir);
			PMPI_Barrier(cntd->comm_local_masters);

			// Local root create directory if local scratch of the node is used
			create_dir(time_trace_dir);
			PMPI_Barrier(cntd->comm_local_masters);

			// Create new files
			open_time_trace_file("a+");
		}
	}

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->my_local_rank != CNTD_MPI_ROOT && cntd->time_trace)
		open_time_trace_file("a+");

	// Attach shared memory
	cntd->shmem_local_rank = (CNTD_Rank_t **) malloc(sizeof(CNTD_Rank_t *) * cntd->local_size);
	for(i = 0; i < cntd->local_size; i++)
	{
		snprintf(shmem_name, sizeof(shmem_name), "/cntd_local_rank_%d",i);
		cntd->shmem_local_rank[i] = get_shmem_rank(shmem_name, 1);
	}
	cntd->cpu = get_shmem_cpu("/cntd_cpus");
	cntd->socket = get_shmem_socket("/cntd_sockets");
	cntd->last_batch_ranks = get_shmem_rank("/cntd_last_batch_local_ranks", cntd->local_size);
	cntd->last_batch_cpus = get_shmem_cpu("/cntd_last_batch_cpus");
	cntd->last_batch_sockets = get_shmem_socket("/cntd_last_batch_sockets");

	PMPI_Barrier(MPI_COMM_WORLD);

	// Batch initialization
	if(cntd->my_local_rank == CNTD_MPI_ROOT && cntd->time_trace)
		print_label_time_trace_file();

	// Turbo conf discovery
	read_arch_turbo_info();

	PMPI_Barrier(MPI_COMM_WORLD);

	// Make first trace time
	if(cntd->my_local_rank == CNTD_MPI_ROOT)
	{
		do_batch();
		update_last_batch(0);
	}

	// Synchronization for process timers
	PMPI_Barrier(MPI_COMM_WORLD);
	makeTimer(&cntd->time_trace_timer, cntd->my_local_rank + DEFAULT_TIME_TRACE_TIMER, cntd->local_size * DEFAULT_TIME_TRACE_TIMER);
}

static void finalize_local_masters()
{
	CNTD_Cpu_t cpu[cntd->arch.cpus];
	CNTD_Socket_t socket[cntd->arch.sockets];

	// Reset timer
	PMPI_Barrier(MPI_COMM_WORLD);
	timer_delete(cntd->time_trace_timer);

	if(cntd->my_local_rank == CNTD_MPI_ROOT)
	{
		double epoch = do_batch(MPI_COMM_WORLD);
		update_batch(epoch, cpu, socket);
		update_last_batch(epoch);
	}

	PMPI_Barrier(MPI_COMM_WORLD);
}

static void init_cntd()
{
	// Bind process to the core
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	CPU_SET(sched_getcpu(), &cpu_set);
	sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set);

	// Allocate COUNTDOWN struct
	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));
	if(cntd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed malloc for countdown!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	cntd->curr_call = 0;
	cntd->prev_call = 1;

	cntd->fd_event_trace = NULL;
	cntd->fd_time_trace = NULL;
}

static void init_structs()
{
	int lengh_size;
	char shmem_name[STRING_SIZE];

	// Communicators
	cntd->comm = (CNTD_Comm_t *) malloc(MEM_SIZE * sizeof(CNTD_Comm_t));
	if(cntd->comm == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed malloc for MPI communicators!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->comm_mem_limit = MEM_SIZE;

	// Groups
	cntd->group = (CNTD_Group_t *) malloc(MEM_SIZE * sizeof(CNTD_Group_t));
	if(cntd->group == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed malloc for MPI group!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	cntd->group_mem_limit = MEM_SIZE;

	// Create local communicators and master communicators
	PMPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &cntd->comm_local_procs);
	PMPI_Comm_rank(cntd->comm_local_procs, &cntd->my_local_rank);
	PMPI_Comm_split(MPI_COMM_WORLD, cntd->my_local_rank, 0, &cntd->comm_local_masters);

	// Ranks
	snprintf(shmem_name, sizeof(shmem_name), "/cntd_local_rank_%d", cntd->my_local_rank);
	cntd->rank = create_shmem_rank(shmem_name, 1);
	PMPI_Comm_rank(MPI_COMM_WORLD, &cntd->rank->my_rank);
	PMPI_Get_processor_name(cntd->arch.hostname, &lengh_size);
	cntd->rank->process_id = getpid();
	PMPI_Comm_size(MPI_COMM_WORLD, &cntd->rank->size);
	PMPI_Comm_size(cntd->comm_local_procs, &cntd->local_size);
	cntd->rank->phase = APP;

	read_arch_info();
	cntd->rank->epoch[START] = 0;
	cntd->rank->epoch[END] = 0;
	cntd->rank->epoch_sample[CURR] = 0;
	cntd->rank->epoch_sample[PREV] = 0;

	// Batch
	init_batch_cpu(CPU_READ_BATCH, &cntd->batch_cpu);
	init_batch_socket(SOCKET_READ_BATCH, &cntd->batch_socket);
}

static void finalize_structs()
{
	char shmem_name[STRING_SIZE];
	snprintf(shmem_name, sizeof(shmem_name), "/cntd_local_rank_%d", cntd->my_local_rank);

	destroy_shmem_rank(cntd->rank, 1, shmem_name);
	destroy_shmem_rank(cntd->last_batch_ranks, cntd->local_size, "/cntd_last_batch_local_ranks");
	destroy_shmem_cpu(cntd->cpu, "/cntd_cpus");
	destroy_shmem_socket(cntd->socket, "/cntd_sockets");
	destroy_shmem_cpu(cntd->last_batch_cpus, "/cntd_last_batch_cpus");
	destroy_shmem_socket(cntd->last_batch_sockets, "/cntd_last_batch_sockets");

	free(cntd->andante_data);
	free(cntd->shmem_local_rank);
	free(cntd->comm);
	free(cntd->group);
}

static void init_monitor()
{
	CNTD_Call_t *prev_call = &cntd->call[cntd->prev_call];
	add_profiling(prev_call, END);
	cntd->epoch[START] = prev_call->epoch[END];
}

static void finalize_monitor()
{
	CNTD_Call_t *curr_call = &cntd->call[cntd->curr_call];
	cntd->epoch[END] = curr_call->epoch[END];
}

void start_cntd()
{
	// Initialization structures
	init_cntd();

	// Init libmsr
	init_libmsr();

	// Malloc structs
	init_structs();

	// Read environment variables
	read_env();

	// Init controllers
	init_local_masters();

	// Init monitor
	init_monitor();
}

void stop_cntd()
{
	// Finilize monitor
	finalize_monitor();

	// Finalize controllers
	finalize_local_masters();

	// Finalize andante
	if(cntd->andante || cntd->andante_analysis)
		andante_finalize();

	// Finalize eam slack
	if(cntd->eam_slack || cntd->eam_slack_analysis)
		eam_finalize();

	// Finalize eam
	if(cntd->eam_call || cntd->eam_call_analysis)
		eam_slack_finalize();

	// Finalize fermata
	if(cntd->fermata)
		fermata_finalize();

	// Print all logs
	print_logs();

	// Close log file
	if(cntd->event_trace > 0)
		close_event_trace_file();
	if(cntd->task_trace > 0)
		close_task_trace_file();
	if(cntd->time_trace)
		close_time_trace_file();

	// Finalize Libmsr
	finalize_libmsr();

	// Deallocate structs
	finalize_structs();
}

void call_pre_start(MPI_Type_t mpi_type, MPI_Comm comm, int addr)
{
	if(cntd->barrier)
		add_barrier(mpi_type, comm, addr);
}

void call_start(CNTD_Call_t *call)
{
	cntd->rank->phase = MPI;

	add_profiling(call, START);

	if(cntd->eam_slack || cntd->eam_slack_analysis)
		eam_slack_pre_mpi(call);
	else if(cntd->eam_call || cntd->eam_call_analysis)
		eam_pre_mpi(call);
	else if(cntd->fermata || cntd->fermata_analysis)
		fermata_pre_mpi(call);
}

static int task_id = NOT_DEFINED;

void call_end(CNTD_Call_t *call)
{
	add_profiling(call, END);
	update_call();

	if(cntd->eam_slack || cntd->eam_slack_analysis)
		eam_slack_post_mpi(call);
	else if(cntd->eam_call || cntd->eam_call_analysis)
		eam_post_mpi(call);
	else if(cntd->fermata || cntd->fermata_analysis)
		fermata_post_mpi(call);

	if(cntd->andante || cntd->andante_analysis || cntd->task_trace)
	{
		task_id = andante_post_task(call, task_id);
		andante_pre_task(call, task_id);
	}

	if(cntd->event_trace)
		print_event();

	cntd->rank->phase = APP;

	switch_call_ptr();
}
