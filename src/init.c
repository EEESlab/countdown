/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *		* Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 * 
 *      * Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *      * Neither the name of the copyright holder nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
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
 * Date: 24.08.2018
*/

#include "cntd.h"

static void init_eam(char *eam_timeout_str, int force_msr)
{
	struct sigaction sa = {{0}};

	cntd->eam = TRUE;
	cntd->eamo = FALSE;

	if(eam_timeout_str != NULL)
		cntd->eam_timeout = atoi(eam_timeout_str);
	else
		cntd->eam_timeout = DEFAULT_EAM_TIMEOUT;

	reset_pstate();
	reset_tstate();

	// Install timer_handler as the signal handler for SIGALRM.
	sa.sa_handler = &eam_call_back;
	sigaction(SIGALRM, &sa, NULL);
}

static void finalize_eam()
{
	struct itimerval timer = {{0}};

	reset_pstate();
	reset_tstate();

	setitimer(ITIMER_REAL, &timer, NULL);
}

static void init_eamo(char *maps, int force_msr)
{
	cntd->eamo = TRUE;
	cntd->eam = FALSE;

	reset_pstate();
	reset_tstate();
	cntd->eamo_curr_pstate = read_target_pstate();
	cntd->eamo_curr_tstate = read_tstate();

	eamo_load_maps(maps);
}

static void finalize_eamo()
{
	reset_pstate();
	reset_tstate();
}

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
	int force_msr;

	// Read environment variables
	char *output_dir = getenv("CNTD_OUT_DIR");
	char *node_sampling_str = getenv("CNTD_NODE_SAMPLING");
	char *adv_metrics_str = getenv("CNTD_ADV_METRICS");
	char *log_mpi_call_str = getenv("CNTD_LOG_MPI_CALL");
	char *eam_str = getenv("CNTD_ENERGY_AWARE_MPI");
	char *eam_timeout_str = getenv("CNTD_ENERGY_AWARE_MPI_TIMEOUT");
	char *eamo_maps_str = getenv("CNTD_ENERGY_AWARE_MPI_ORACLE");

	create_dir(output_dir, cntd->log_dir);

	if(log_mpi_call_str != NULL && (
		strcasecmp(log_mpi_call_str, "1") == 0 || 
		strcasecmp(log_mpi_call_str, "2") == 0))
	{
		cntd->log_call = atoi(log_mpi_call_str);
		open_mpicall_file(cntd->log_call);
	}
	else
		cntd->log_call = 0;

	if(str_to_bool(node_sampling_str))
		cntd->node_sampling = TRUE;
	else
		cntd->node_sampling = FALSE;

	if(str_to_bool(adv_metrics_str))
		cntd->adv_metrics = TRUE;
	else
		cntd->adv_metrics = FALSE;

	if(eamo_maps_str != NULL)
		init_eamo(eamo_maps_str, force_msr);
	else if(str_to_bool(eam_str))
	{
		if(eam_timeout_str != NULL)
			init_eam(eam_timeout_str, force_msr);
		else
			init_eam(NULL, force_msr);
	}
	else
	{
		cntd->eamo = FALSE;
		cntd->eam = FALSE;
		cntd->eam_timeout = DEFAULT_EAM_TIMEOUT;
	}
}

void callback_batch(int sig, siginfo_t *si, void *uc)
{
	CNTD_Cpu_t cpu[cntd->rank->cpus];
    CNTD_Socket_t socket[cntd->rank->sockets];

	double epoch = sample_batch();
	update_batch(epoch, cpu, socket);
	if(cntd->node_sampling)
    	print_batch(epoch, cpu, socket);
    update_last_batch(epoch);
}

static int makeTimer(timer_t *timerID, int expire, int interval)
{
    struct sigevent te;
    struct itimerspec its;
    struct sigaction sa;
    int sigNo = SIGRTMIN;

    // Set up signal handler.
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = callback_batch;
    sigemptyset(&sa.sa_mask);
    if(sigaction(sigNo, &sa, NULL) == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed to setup sampling timer!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Set and enable alarm
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_interval.tv_sec = interval;
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = expire;
    its.it_value.tv_nsec = 0;
    timer_settime(*timerID, 0, &its, NULL);

    return(0);
}

static void init_local_masters()
{
	int i;
	char shmem_name[STRING_SIZE];
	struct timespec epoch;

	// Initialize local masters
	if(cntd->my_local_rank == ROOT_MPI)
	{
		// Rank
		cntd->last_batch_rank = create_shmem_rank("/last_batch_rank", cntd->local_size);

	    // CPUs
	    cntd->cpu = create_shmem_cpu("/cpu");
	    cntd->last_batch_cpu = create_shmem_cpu("/last_batch_cpu");
		for(i = 0; i < cntd->rank->cpus; i++)
		{
			cntd->cpu[i].cpu_id = i;
			strcpy(cntd->cpu[i].hostname, cntd->rank->hostname);

			cntd->cpu[i].num_samples = 0;
			cntd->last_batch_cpu[i].num_samples = 1;
		}

		// Sockets
		cntd->socket = create_shmem_socket("/socket");
		cntd->last_batch_socket = create_shmem_socket("/last_batch_socket");
		for(i = 0; i < cntd->rank->sockets; i++)
		{
			cntd->socket[i].socket_id = i;
			strcpy(cntd->socket[i].hostname, cntd->rank->hostname);

			cntd->socket[i].rapl_joules = cntd->ru->joules;
			cntd->socket[i].rapl_watts = cntd->ru->watts;
			cntd->socket[i].rapl_seconds = cntd->ru->seconds;

			cntd->socket[i].num_samples = 0;
			cntd->last_batch_socket[i].num_samples = 1;
		}

		// Enable socket perf counters
		enable_pcu();
    	enable_uncore_freq();

    	// Create node samplig file
    	if(cntd->node_sampling)
    	{
    		delete_sampling_file();
			open_sampling_file("a+");
    	}
	}

	PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->my_local_rank != ROOT_MPI && cntd->node_sampling)
		open_sampling_file("a+");
	
	// Attach shared memory
	cntd->shmem_local_rank = (CNTD_Rank_t **) malloc(sizeof(CNTD_Rank_t *) * cntd->local_size);
	for(i = 0; i < cntd->local_size; i++)
	{
		sprintf(shmem_name, "/local_rank_%d",i);
		cntd->shmem_local_rank[i] = get_shmem_rank(shmem_name, 1);
	}
	cntd->cpu = get_shmem_cpu("/cpu");
	cntd->socket = get_shmem_socket("/socket");
	cntd->last_batch_rank = get_shmem_rank("/last_batch_rank", cntd->local_size);
	cntd->last_batch_cpu = get_shmem_cpu("/last_batch_cpu");
	cntd->last_batch_socket = get_shmem_socket("/last_batch_socket");

	PMPI_Barrier(MPI_COMM_WORLD);

	// Call initialization
	clock_gettime(CLOCK_TYPE, &epoch);
	cntd->epoch[START] = timespec2double(epoch);
	CNTD_Call_t *call = add_cntd_call(ENUM_MPI_INIT, MPI_COMM_WORLD);
    add_profiling(call, START);
    add_profiling(call, END);
    switch_call_ptr();

    PMPI_Barrier(MPI_COMM_WORLD);

	if(cntd->my_local_rank == ROOT_MPI && cntd->node_sampling)
		print_label_sampling_file();

	PMPI_Barrier(MPI_COMM_WORLD);

	// Make first sample
	if(cntd->my_local_rank == ROOT_MPI)
	{
		sample_batch();
		update_last_batch(0);
	}

	// Synchronization for process timers
	PMPI_Barrier(MPI_COMM_WORLD);
    makeTimer(&cntd->sampling_timer, cntd->my_local_rank+DEFAULT_SAMPLING_TIME, cntd->local_size*DEFAULT_SAMPLING_TIME);
}

static void finalize_local_masters()
{
	CNTD_Cpu_t cpu[cntd->rank->cpus];
    CNTD_Socket_t socket[cntd->rank->sockets];

	// Reset timer
	PMPI_Barrier(MPI_COMM_WORLD);
	timer_delete(cntd->sampling_timer);

	if(cntd->my_local_rank == ROOT_MPI)
	{
		double epoch = sample_batch(MPI_COMM_WORLD);
		update_batch(epoch, cpu, socket);
	    update_last_batch(epoch);
	}

    PMPI_Barrier(MPI_COMM_WORLD);
}

static void init_cntd()
{
	// Bind process to local CPU
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	CPU_SET(sched_getcpu(), &cpu_set);
	sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set); 

	cntd = (CNTD_t *) calloc(1, sizeof(CNTD_t));
	if(cntd == NULL)
	{
		fprintf(stderr, "Error: <countdown> Failed malloc for countdown!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	cntd->curr_call = 0;
	cntd->prev_call = 1;

	cntd->fd_mpicall = NULL;
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
	sprintf(shmem_name, "/local_rank_%d", cntd->my_local_rank);
	cntd->rank = create_shmem_rank(shmem_name, 1);
	PMPI_Comm_rank(MPI_COMM_WORLD, &cntd->rank->my_rank);
	PMPI_Get_processor_name(cntd->rank->hostname, &lengh_size);
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
	sprintf(shmem_name, "/local_rank_%d", cntd->my_local_rank);

	destroy_shmem_rank(cntd->rank, 1, shmem_name);
	destroy_shmem_rank(cntd->last_batch_rank, cntd->local_size, "/last_batch_rank");
	destroy_shmem_cpu(cntd->cpu, "/cpu");
	destroy_shmem_socket(cntd->socket, "/socket");
	destroy_shmem_cpu(cntd->last_batch_cpu, "/last_batch_cpu");
	destroy_shmem_socket(cntd->last_batch_socket, "last_batch_socket");
	free(cntd->shmem_local_rank);
	free(cntd->comm);
	free(cntd->group);
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
}

void stop_cntd()
{
	// Finalize controllers
	finalize_local_masters();

	// Finalize eam/o
	if(cntd->eamo)
		finalize_eamo();
	else if(cntd->eam)
		finalize_eam();

	// Print all logs
	print_logs();

	// Close log file
	if(cntd->log_call)
		close_mpicall_file();
	if(cntd->node_sampling)
		close_sampling_file();

	// Finalize Libmsr
	finalize_libmsr();

	// Deallocate structs
	finalize_structs();
}

void call_start(CNTD_Call_t *call)
{
	cntd->rank->phase = MPI;
	add_profiling(call, START);

	if(cntd->eamo)
		eamo_sched_next_conf(call, START);
	else if(cntd->eam)
		eam(call, START);
}

void call_end(CNTD_Call_t *call)
{
	if(cntd->eamo)
		eamo_sched_next_conf(call, END);
	else if(cntd->eam)
		eam(call, END);

	add_profiling(call, END);
	update_call();

	if(cntd->log_call && call->idx > 0)
		print_mpicall();
	
	switch_call_ptr();
	cntd->rank->phase = APP;
}
