#include "cntd.h"

static void init_msr(int force_msr)
{
	char msr_path[STRING_SIZE];

	if(force_msr)
		snprintf(msr_path, STRING_SIZE, "/dev/cpu/%d/msr", cntd->cpu_id);
	else
		snprintf(msr_path, STRING_SIZE, "/dev/cpu/%d/msr_safe", cntd->cpu_id);
	open_msr(msr_path);
}

static void init_call_prof(int set)
{
	cntd->call_prof_ctr = set;
}

static void init_net_prof(int level)
{
	cntd->net_prof_ctr = level;

	int size = cntd->mpi_size;

	cntd->call[0].net_send = (int *) malloc(size * sizeof(int));
	cntd->call[0].net_recv = (int *) malloc(size * sizeof(int));
	cntd->call[1].net_send = (int *) malloc(size * sizeof(int));
	cntd->call[1].net_recv = (int *) malloc(size * sizeof(int));

	cntd->adv_metrics[0].net_send = (uint64_t *) malloc(size * sizeof(uint64_t));
	cntd->adv_metrics[0].net_recv = (uint64_t *) malloc(size * sizeof(uint64_t));
	cntd->adv_metrics[1].net_send = (uint64_t *) malloc(size * sizeof(uint64_t));
	cntd->adv_metrics[1].net_recv = (uint64_t *) malloc(size * sizeof(uint64_t));

	cntd->net_send = (uint64_t *) calloc(size, sizeof(uint64_t));
	cntd->net_recv = (uint64_t *) calloc(size, sizeof(uint64_t));
}

static void init_fix_perf_ctr(int set)
{
	cntd->fix_perf_ctr = set;
	if(set)
		enable_fix_ctr();
}

static void init_pmu_perf_ctr(int set)
{
	cntd->pmu_perf_ctr = set;
}

static void init_adv_metr(int set, char *adv_metrics_timeout_str)
{
	cntd->adv_metrics_ctr = set;

	//cntd->adv_metrics[0].tot_net_send = 0;
	//cntd->adv_metrics[0].tot_net_recv = 0;
	//cntd->adv_metrics[1].tot_net_send = 0;
	//cntd->adv_metrics[1].tot_net_recv = 0;
	
	read_rapl_units();
	cntd->fix_perf_ctr = TRUE;
	enable_fix_ctr();
	enable_uncore_freq_ctr();

	if(adv_metrics_timeout_str != NULL)
		cntd->adv_metrics_timeout = atoi(adv_metrics_timeout_str);
	else
		cntd->adv_metrics_timeout = ADV_METRICS_TIMEOUT;
}

static void init_eam(char *eam_timeout_str)
{
	struct sigaction sa = {{0}};

	cntd->eam = TRUE;
	cntd->eam_flag = FALSE;

	if(eam_timeout_str != NULL)
		cntd->eam_timeout = atoi(eam_timeout_str);
	else
		cntd->eam_timeout = EAM_TIMEOUT;

	reset_pstate();
	reset_tstate();

	// Install timer_handler as the signal handler for SIGALRM.
	sa.sa_handler = &call_back_eam;
	sigaction(SIGALRM, &sa, NULL);
}

static void finalize_eam()
{
	struct itimerval timer = {{0}};

	reset_pstate();
	reset_tstate();

	setitimer(ITIMER_REAL, &timer, NULL);
}

static void init_eamo(char *eamo_str)
{
	cntd->eamo = TRUE;

	reset_pstate();
	reset_tstate();
	cntd->eamo_curr_pstate = read_target_pstate();
	cntd->eamo_curr_tstate = read_tstate();

	load_eamo_files(eamo_str);
}

static void finalize_eamo()
{
	reset_pstate();
	reset_tstate();
}

void init_cntd()
{
	int my_rank, size;

	cntd = (CNTD_t *) calloc(1, sizeof(*cntd));
	arch = (CNTD_Arch_t *) calloc(1, sizeof(*arch));

	detect_topology();

	cntd->comm = (CNTD_Comm_t *) malloc(MEM_SIZE * sizeof(CNTD_Comm_t));
	if(cntd->comm == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed malloc for mpi communicators!\n");
		exit(EXIT_FAILURE);
	}

	cntd->group = (CNTD_Group_t *) malloc(MEM_SIZE * sizeof(CNTD_Group_t));
	if(cntd->group == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed malloc for mpi group!\n");
		exit(EXIT_FAILURE);
	}

	cntd->comm_mem_limit = MEM_SIZE;
	cntd->group_mem_limit = MEM_SIZE;

	gethostname(cntd->hostname, STRING_SIZE);
	cntd->cpu_id = get_cpu_id();
	cntd->socket_id = get_socket_id();
	cntd->process_id = getpid();
	PMPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	cntd->mpi_rank = my_rank;
	PMPI_Comm_size(MPI_COMM_WORLD, &size);
	cntd->mpi_size = size;

	cntd->fd_msr = -1;
	//cntd->call_count = 0;
	cntd->prev_call = 0;
	cntd->curr_call = 1;
	//cntd->adv_metrics_prev = 0;
	cntd->adv_metrics_curr = 1;

	//cntd->tot_net_send = 0;
	//cntd->tot_net_recv = 0;
	//cntd->call[0].tot_net_send = 0;
	//cntd->call[0].tot_net_recv = 0;
	//cntd->call[1].tot_net_send = 0;
	//cntd->call[1].tot_net_recv = 0;

	// Read environment variables
	char *output_dir = getenv("CNTD_OUT_DIR");
	char *force_msr_str = getenv("CNTD_FORCE_MSR");
	char *call_prof_str = getenv("CNTD_CALL_PROF");
	char *net_prof_str = getenv("CNTD_NET_PROF");
	char *no_fix_perf_str = getenv("CNTD_NO_FIX_PERF");
	char *pmu_perf_ctr_str = getenv("CNTD_PMU_PERF_CTR");
	char *no_adv_metrics_str = getenv("CNTD_NO_ADV_METRIC");
	char *adv_metrics_timeout_str = getenv("CNTD_ADV_METRIC_TIMEOUT");
	char *eam_str = getenv("CNTD_ENERGY_AWARE_MPI");
	char *eam_timeout_str = getenv("CNTD_ENERGY_AWARE_MPI_TIMEOUT");
	char *eamo_str = getenv("CNTD_ENERGY_AWARE_MPI_ORACLE");

	if(force_msr_str != NULL && (
		strcasecmp(force_msr_str, "enable") == 0 || 
		strcasecmp(force_msr_str, "on") == 0 || 
		strcasecmp(force_msr_str, "yes") == 0 || 
		strcasecmp(force_msr_str, "1") == 0))
	{
		init_msr(TRUE);
	}
	else
	{
		init_msr(FALSE);
	}

	if(call_prof_str != NULL && (
		strcasecmp(call_prof_str, "enable") == 0 || 
		strcasecmp(call_prof_str, "on") == 0 || 
		strcasecmp(call_prof_str, "yes") == 0 || 
		strcasecmp(call_prof_str, "1") == 0))
	{
		init_call_prof(TRUE);
	}
	else
		init_call_prof(FALSE);

	if(net_prof_str != NULL && (
		strcasecmp(net_prof_str, "1") == 0 || 
		strcasecmp(net_prof_str, "2") == 0 || 
		strcasecmp(net_prof_str, "3") == 0))
	{
		init_net_prof(atoi(net_prof_str));
	}
	else
		init_net_prof(FALSE);

	if(no_fix_perf_str != NULL && (
		strcasecmp(no_fix_perf_str, "enable") == 0 || 
		strcasecmp(no_fix_perf_str, "on") == 0 || 
		strcasecmp(no_fix_perf_str, "yes") == 0 || 
		strcasecmp(no_fix_perf_str, "1") == 0))
	{
		init_fix_perf_ctr(FALSE);
	}
	else
		init_fix_perf_ctr(TRUE);

	if(pmu_perf_ctr_str != NULL && (
		strcasecmp(pmu_perf_ctr_str, "enable") == 0 || 
		strcasecmp(pmu_perf_ctr_str, "on") == 0 || 
		strcasecmp(pmu_perf_ctr_str, "yes") == 0 || 
		strcasecmp(pmu_perf_ctr_str, "1") == 0))
	{
		init_pmu_perf_ctr(TRUE);
	}
	else
		init_pmu_perf_ctr(FALSE);

	if(no_adv_metrics_str != NULL && (
		strcasecmp(no_adv_metrics_str, "enable") == 0 || 
		strcasecmp(no_adv_metrics_str, "on") == 0 || 
		strcasecmp(no_adv_metrics_str, "yes") == 0 || 
		strcasecmp(no_adv_metrics_str, "1") == 0))
	{
		init_adv_metr(FALSE, adv_metrics_timeout_str);
	}
	else
		init_adv_metr(TRUE, adv_metrics_timeout_str);

	if(eamo_str != NULL)
	{
		init_eamo(eamo_str);
	}
	else if(eam_str != NULL && eamo_str == NULL && (
		strcasecmp(eam_str, "enable") == 0 || 
		strcasecmp(eam_str, "on") == 0 || 
		strcasecmp(eam_str, "yes") == 0 || 
		strcasecmp(eam_str, "1") == 0))
	{
		if(eam_timeout_str != NULL)
		{

			init_eam(eam_timeout_str);
		}
		else
			init_eam(NULL);
	}
	else
	{
		cntd->eamo = FALSE;
		cntd->eam = FALSE;
	}
	
	open_log_files(output_dir);
}

void initialize_cntd(CNTD_Call_t *call)
{
	if(cntd->eamo)
		sched_next_eamo_conf(call, START);
	else if(cntd->eam)
		eam(call, START);

	add_timing(call, START);
	cntd->tsc[START] = call->tsc[START];
	cntd->epoch[START] = call->epoch[START];
	add_perf(call, START);
	update_adv_metrics(call, START);

	PMPI_Barrier(MPI_COMM_WORLD);

    add_timing(call, END);
	add_perf(call, END);
	check_adv_metrics(call, END);


	if(cntd->eamo)
		sched_next_eamo_conf(call, END);
	else if(cntd->eam)
		eam(call, END);

	CNTD_Call_Phase_t phase = calc_call();
	print_call(phase);
	update_curr_call();
}

void finalize_cntd(CNTD_Call_t *call)
{
    add_timing(call, END);
	cntd->tsc[END] = call->tsc[END];
    cntd->epoch[END] = call->epoch[END];

    cntd->tsc[DURATION] = diff_64(cntd->tsc[END], cntd->tsc[START]);
    cntd->epoch[DURATION] = cntd->epoch[END] - cntd->epoch[START];

    add_perf(call, END);
	update_adv_metrics(call, END);

	CNTD_AdvMetrics_Phase_t advmetr_data = calc_adv_metrics();
	update_cntd_advmetr(advmetr_data);
	print_adv_metrics(advmetr_data);

	// Reset performance states
	if(cntd->eamo)
		finalize_eamo();
	else if(cntd->eam)
		finalize_eam();

	CNTD_Call_Phase_t phase = calc_call();
	print_call(phase);
	update_curr_call();

	print_cntd_dynamic_info();
	print_custmetr();

	close_log_files();
	close_msr();
}

void call_start(CNTD_Call_t *call)
{
	if(cntd->eamo)
		sched_next_eamo_conf(call, START);
	else if(cntd->eam)
		eam(call, START);

	add_timing(call, START);
	add_perf(call, START);
	check_adv_metrics(call, START);
}

void call_end(CNTD_Call_t *call)
{
	add_timing(call, END);
	add_perf(call, END);
    check_adv_metrics(call, END);

    if(cntd->eamo)
		sched_next_eamo_conf(call, END);
	else if(cntd->eam)
		eam(call, END);

	CNTD_Call_Phase_t phase = calc_call();
	print_call(phase);
	update_curr_call();
}
