#include "cntd.h"

static void print_cntd_labels()
{
	fprintf(cntd->fd_cntd,
		"hostname"
		";cpu_id"
		";socket_id"
		";process_id"
		";mpi_rank"
		";mpi_size"
		";network_profiling"
		";fix_perf_ctr"
		";pmu_perf_ctr"
		";adv_metric"
		";energy_aware_mpi"
		";energy_aware_mpi_timeout"
		";energy_aware_mpi_oracle"
		";cpu_model_name"
		";cpu_arch"
		";smt"
		";nominal_freq"
		";num_cpus"
		";num_cores"
		";num_hw_threads"
		";num_sockets"
		";num_cores_per_socket"
		";num_hw_threads_per_socket"
		";group_count"
		";comm_count"
		";call_count"
		";epoch_start"
		";epoch_end"
		";exe_time"
		";tot_tsc"
		";tot_net_send"
		";tot_net_recv");

	if(cntd->net_prof_ctr >= 1)
	{
		int i;
		for(i = 0; i < cntd->mpi_size; i++) 
			fprintf(cntd->fd_cntd, ";net_send_%d", i);
		for(i = 0; i < cntd->mpi_size; i++) 
			fprintf(cntd->fd_cntd, ";net_recv_%d", i);
	}

	if(cntd->adv_metrics_ctr)
	{
		fprintf(cntd->fd_cntd,
			";tot_inst_ret"
			";avg_load"
			";avg_core_freq"
			";avg_cpi");

		if(cntd->pmu_perf_ctr)
		{
			fprintf(cntd->fd_cntd, ";tot_pmu_1;tot_pmu_2;tot_pmu_3;tot_pmu_4");
			if(!arch->smt)
				fprintf(cntd->fd_cntd, ";tot_pmu_5;tot_pmu_6;tot_pmu_7;tot_pmu_8");
		}

		fprintf(cntd->fd_cntd,
			";avg_core_c0"
			";avg_core_c1"
			";avg_core_c3"
			";avg_core_c6"
			";avg_core_temp"
			";avg_uncore_freq"
			";tot_pkg_energy"
			";tot_dram_energy"
			";avg_pkg_power"
			";avg_dram_power"
			";avg_pkg_c0"
			";avg_pkg_c2"
			";avg_pkg_c3"
			";avg_pkg_c6"
			";avg_pkg_temp");
	}

	fprintf(cntd->fd_cntd, "\n");
}

static void print_group_labels()
{
	int i;

	fprintf(cntd->fd_group, "idx;size;local_rank;world_rank");
	for(i = 0; i < cntd->mpi_size; i++) 
		fprintf(cntd->fd_group, ";proc_%d", i);
	fprintf(cntd->fd_group, "\n");
}

static void print_comm_labels()
{
	fprintf(cntd->fd_comm, "comm_idx;group_idx\n");
}

static void print_adv_metrics_labels()
{
	fprintf(cntd->fd_adv_metrics,
		"call_start_idx"
		";call_start_when"
		";call_start_type"
		";call_end_idx"
		";call_end_when"
		";call_end_type"
		";time_start"
		";time_end"
		";time_duration"
		";tsc"
		";load"
		";core_freq"
		";cpi"
		";inst_ret"
		";clk_curr"
		";clk_ref"
		";aperf"
		";mperf"
		";core_c0"
		";core_c1"
		";core_c3"
		";core_c6"
		";core_temp"
		";uncore_freq"
		";pkg_energy"
		";dram_energy"
		";pkg_power"
		";dram_power"
		";pkg_c0"
		";pkg_c2"
		";pkg_c3"
		";pkg_c6"
		";pkg_temp");

	if(cntd->pmu_perf_ctr)
	{
		fprintf(cntd->fd_adv_metrics, ";pmu_1;pmu_2;pmu_3;pmu_4");
		if(!arch->smt)
			fprintf(cntd->fd_adv_metrics, ";pmu_5;pmu_6;pmu_7;pmu_8");
	}

	fprintf(cntd->fd_adv_metrics,";tot_net_send;tot_net_recv");
	if(cntd->net_prof_ctr >= 2)
	{
		int i;
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_adv_metrics, ";net_send_%d", i);
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_adv_metrics, ";net_recv_%d", i);
	}

	fprintf(cntd->fd_adv_metrics, "\n");
}

void open_log_files(char *output_dir)
{
	struct stat st = {0};
	char log_dir[STRING_SIZE];
	char log_file[STRING_SIZE];

	// MPI root checks the log directory, if does not exist, makes directory recursively
	if(output_dir != NULL && strcmp(output_dir, "") != 0)
	{
		if(cntd->mpi_rank == ROOT_MPI)
		{
			if(stat(output_dir, &st) == -1)
			{
				if(mkpath(output_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
				{
			        fprintf(stderr, "[COUNTDOWN ERROR] Cannot create report directory: %s\n", output_dir);
					exit(EXIT_FAILURE);
			    }
			}
		}
		strcpy(log_dir, output_dir);
	}
	else
		strcpy(log_dir, ".");

	// Synchronization for creation of log directory
	PMPI_Barrier(MPI_COMM_WORLD);

	// Open info file
	sprintf(log_file, "%s/cntd_info_%d.csv", log_dir, cntd->mpi_rank);
	cntd->fd_cntd = fopen(log_file, "w");
	if(cntd->fd_cntd == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create general report file: %s\n", log_file);
		exit(EXIT_FAILURE);
	}
	print_cntd_labels();
	print_cntd_static_info();

	// Open group file
	sprintf(log_file, "%s/cntd_group_%d.csv", log_dir, cntd->mpi_rank);
	cntd->fd_group = fopen(log_file, "w");
	if(cntd->fd_group == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create group report file: %s\n", log_file);
		exit(EXIT_FAILURE);
	}
	print_group_labels();

	// Open comm file
	sprintf(log_file, "%s/cntd_comm_%d.csv", log_dir, cntd->mpi_rank);
	cntd->fd_comm = fopen(log_file, "w");
	if(cntd->fd_comm == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create communicator report file: %s\n", log_file);
		exit(EXIT_FAILURE);
	}
	print_comm_labels();

	// Open call file
	if(cntd->call_prof_ctr)
	{
		sprintf(log_file, "%s/cntd_call_%d.bin", log_dir, cntd->mpi_rank);
		cntd->fd_call = fopen(log_file, "wb");
		if(cntd->fd_call == NULL)
		{
			fprintf(stderr, "[COUNTDOWN ERROR] Failed to create call report file: %s\n", log_file);
			exit(EXIT_FAILURE);
		}
	}

	// Open advanced metric file
	if(cntd->adv_metrics_ctr)
	{
		sprintf(log_file, "%s/cntd_advmetr_%d.csv", log_dir, cntd->mpi_rank);
		cntd->fd_adv_metrics = fopen(log_file, "w");
		if(cntd->fd_adv_metrics == NULL)
		{
			fprintf(stderr, "[COUNTDOWN ERROR] Failed to create advanced metrics report file: %s\n", log_file);
			exit(EXIT_FAILURE);
		}
		print_adv_metrics_labels();
	}

	// Open custom metric file
	sprintf(log_file, "%s/cntd_custmetr_%d.csv", log_dir, cntd->mpi_rank);
	cntd->fd_cust_metr = fopen(log_file, "w");
	if(cntd->fd_cust_metr == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create custom metric report file: %s\n", log_file);
		exit(EXIT_FAILURE);
	}

	// Write metaphase info files
	if(cntd->call_prof_ctr && cntd->mpi_rank == ROOT_MPI)
		print_call_info(log_dir);
}

void close_log_files()
{
	if(cntd->fd_cntd > 0)
		fclose(cntd->fd_cntd);
	if(cntd->fd_group > 0)
		fclose(cntd->fd_group);
	if(cntd->fd_comm > 0)
		fclose(cntd->fd_comm);
	if(cntd->call_prof_ctr > 0)
		fclose(cntd->fd_call);
	if(cntd->adv_metrics_ctr > 0)
		fclose(cntd->fd_adv_metrics);
	if(cntd->adv_metrics_ctr > 0)
		fclose(cntd->fd_cust_metr);
}

void print_call_info(char *log_dir)
{
	int i, group_world_size = cntd->mpi_size;
	FILE *fd;
	char setting_file[STRING_SIZE];

	// MPI_TYPE info
	sprintf(setting_file, "%s/cntd_mpitype.csv", log_dir);

	fd = fopen(setting_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create mpi_type report file!\n");
		exit(EXIT_FAILURE);
	}

	// Write labels of MPI_TYPE
	fprintf(fd, "%s", mpi_type_str[0]);
	for(i = 1; i < NUM_MPI_TYPE; i++)
		fprintf(fd, ";%s", mpi_type_str[i]+5);
	fprintf(fd, "\n");

	// Write numbers of MPI_TYPE
	fprintf(fd, "0");
	for(i = 1; i < NUM_MPI_TYPE; i++)
		fprintf(fd, ";%d", i);
	fprintf(fd, "\n");

	fclose(fd);

	// Binary rappresentation of call log file
	sprintf(setting_file, "%s/cntd_callinfo.csv", log_dir);

	fd = fopen(setting_file, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "[COUNTDOWN ERROR] Failed to create binary report file!\n");
		exit(EXIT_FAILURE);
	}

	// Write labels of binary meta info file
	fprintf(fd, 
		"idx;type;comm_idx;group_idx"
		";time_start_app;time_start_mpi;time_end_mpi"
		";time_duration_app;time_duration_mpi;tsc_app;tsc_mpi"
		";tot_net_send;tot_net_recv");

	if(cntd->net_prof_ctr >= 3)
	{
		for(i = 0; i < group_world_size; i++)
			fprintf(fd, ";net_send_%d", i);
		for(i = 0; i < group_world_size; i++)
			fprintf(fd, ";net_recv_%d", i);
	}

	if(cntd->fix_perf_ctr)
	{
		fprintf(fd,
			";inst_ret_app;inst_ret_mpi"
			";clk_curr_app;clk_curr_mpi"
			";clk_ref_app;clk_ref_mpi"
			";load_app;load_mpi"
			";core_freq_app;core_freq_mpi"
			";cpi_app;cpi_mpi");
	}

	if(cntd->pmu_perf_ctr)
	{
		fprintf(fd,
			";pmu_1_app;pmu_1_mpi"
			";pmu_2_app;pmu_2_mpi"
			";pmu_3_app;pmu_3_mpi"
			";pmu_4_app;pmu_4_mpi");
		if(!arch->smt)
		{
			fprintf(fd,
				";pmu_5_app;pmu_5_mpi"
				";pmu_6_app;pmu_6_mpi"
				";pmu_7_app;pmu_7_mpi"
				";pmu_8_app;pmu_8_mpi");
		}
	}

	fprintf(fd, "\n");

	// Write sizes of binary info file
	fprintf(fd, 
		"uint64;uint64;uint64;uint64"
		";double;double;double"
		";double;double;uint64;uint64"
		";uint64;uint64");

	if(cntd->net_prof_ctr >= 3)
	{
		for(i = 0; i < group_world_size; i++)
			fprintf(fd, ";int32");
		for(i = 0; i < group_world_size; i++)
			fprintf(fd, ";int32");
	}

	if(cntd->fix_perf_ctr)
	{
		fprintf(fd,
			";uint64;uint64"
			";uint64;uint64"
			";uint64;uint64"
			";double;double"
			";double;double"
			";double;double");
	}

	if(cntd->pmu_perf_ctr)
	{
		fprintf(fd,
			";uint64;uint64"
			";uint64;uint64"
			";uint64;uint64"
			";uint64;uint64");

		if(!arch->smt)
		{
			fprintf(fd,
				";uint64;uint64"
				";uint64;uint64"
				";uint64;uint64"
				";uint64;uint64");
		}
	}

	fclose(fd);
}

void print_cntd_static_info()
{
	fprintf(cntd->fd_cntd, 
		"%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s;%d;%d;%d;%d;%d;%d;%d;%d",
		cntd->hostname,
		cntd->cpu_id,
		cntd->socket_id,
		cntd->process_id,
		cntd->mpi_rank,
		cntd->mpi_size,
		cntd->net_prof_ctr,
		cntd->fix_perf_ctr,
		cntd->pmu_perf_ctr,
		cntd->adv_metrics_ctr,
		cntd->eam,
		cntd->eam_timeout,
		cntd->eamo,
		arch->model_name,
		arch->arch_name,
		arch->smt,
		arch->nominal_freq,
		arch->num_cpus,
		arch->num_cores,
		arch->num_hw_threads,
		arch->num_sockets,
		arch->num_cores_per_socket,
		arch->num_hw_threads_per_socket);
	//fflush(cntd->fd_cntd);
}

void print_cntd_dynamic_info()
{
	fprintf(cntd->fd_cntd,
		";%lu;%lu;%lu;%.9f;%.9f;%.9f;%lu;%lu;%lu",
		cntd->group_count,
		cntd->comm_count,
		cntd->call_count,
		cntd->epoch[START],
		cntd->epoch[END],
		cntd->epoch[DURATION],
		cntd->tsc[DURATION],
		cntd->tot_net_send,
		cntd->tot_net_recv);

	if(cntd->net_prof_ctr >= 1)
	{
		int i;
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_cntd, ";%lu", cntd->net_send[i]);
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_cntd, ";%lu", cntd->net_recv[i]);
	}

	if(cntd->adv_metrics_ctr)
	{
		fprintf(cntd->fd_cntd,
			";%lu;%.3f;%d;%.3f",
			cntd->inst_ret,
			cntd->load / cntd->epoch[DURATION],
			(int) (cntd->core_freq / cntd->epoch[DURATION]),
			cntd->cpi / cntd->epoch[DURATION]);

		if(cntd->pmu_perf_ctr)
		{
			fprintf(cntd->fd_cntd, 
				";%lu;%lu;%lu;%lu",
				cntd->pmu_1,
				cntd->pmu_2,
				cntd->pmu_3,
				cntd->pmu_4);
			if(!arch->smt)
			{
				fprintf(cntd->fd_cntd, 
					";%lu;%lu;%lu;%lu",
					cntd->pmu_5,
					cntd->pmu_6,
					cntd->pmu_7,
					cntd->pmu_8);
			}
		}

		fprintf(cntd->fd_cntd,
			";%.3f;%.3f;%.3f;%.3f;%.3f;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f",
			cntd->core_c0 / cntd->epoch[DURATION],
			cntd->core_c1 / cntd->epoch[DURATION],
			cntd->core_c3 / cntd->epoch[DURATION],
			cntd->core_c6 / cntd->epoch[DURATION],
			(double) cntd->core_temp / cntd->epoch[DURATION],
			(int) (cntd->uncore_freq / cntd->epoch[DURATION]),
			cntd->pkg_energy,
			cntd->dram_energy,
			cntd->pkg_energy / cntd->epoch[DURATION],
			cntd->dram_energy / cntd->epoch[DURATION],
			cntd->pkg_c0 / cntd->epoch[DURATION],
			cntd->pkg_c2 / cntd->epoch[DURATION],
			cntd->pkg_c3 / cntd->epoch[DURATION],
			cntd->pkg_c6 / cntd->epoch[DURATION],
			cntd->pkg_temp / cntd->epoch[DURATION]);
	}

	fprintf(cntd->fd_cntd, "\n");
}

void print_group(CNTD_Group_t *cntd_group)
{
	int i, local_rank;

	fprintf(cntd->fd_group, "%d;%d;%d;%d", 
		cntd_group->idx,
		cntd_group->size,
		cntd_group->local_rank,
		cntd_group->world_rank);
	for(i = 0; i < cntd->mpi_size; i++)
	{
		local_rank = world_rank_2_local_rank(i, cntd_group);
		fprintf(cntd->fd_group, ";%d", local_rank);
	}
	fprintf(cntd->fd_group, "\n");
}

void print_comm(CNTD_Comm_t *cntd_comm)
{
	CNTD_Group_t *cntd_group = cntd_comm->cntd_group;
	fprintf(cntd->fd_comm, "%d;%d\n", cntd_comm->idx, cntd_group->idx);
}

void print_call(CNTD_Call_Phase_t phase)
{
	if(cntd->call_prof_ctr)
	{
		int size;

		// Write on files
		size = (sizeof(uint64_t) * 8) + (sizeof(double) * 5);
		fwrite(&phase.idx, size, 1, cntd->fd_call);

		if(cntd->net_prof_ctr >= 3)
			fwrite(phase.net_send, sizeof(phase.net_send) * cntd->mpi_size, 2, cntd->fd_call);

		if(cntd->fix_perf_ctr)
		{
			size = (sizeof(uint64_t) * 6) + (sizeof(double) * 6);
			fwrite(phase.inst_ret, size, 1, cntd->fd_call);
		}

		if(cntd->pmu_perf_ctr)
		{
			size = sizeof(uint64_t) * 8;

			fwrite(phase.pmu_1, size, 1, cntd->fd_call);
			if(!arch->smt)
				fwrite(phase.pmu_5, size, 1, cntd->fd_call);
		}
	}
}

void print_adv_metrics(CNTD_AdvMetrics_Phase_t phase)
{
	int i;
	static int init = TRUE;

	CNTD_AdvMetrics_t *adv_metrics_prev = &cntd->adv_metrics[cntd->adv_metrics_prev];

	if(init)
	{
		adv_metrics_prev->tsc = cntd->tsc[START];
		adv_metrics_prev->epoch = cntd->epoch[START];
		init = FALSE;
	}

	fprintf(cntd->fd_adv_metrics, "%lu;%c;%s;%lu;%c;%s;%.9f;%.9f;%.9f;%lu;%.3f;%.3f;%.3f;%lu;%lu;%lu;%lu;%lu;%.3f;%.3f;%.3f;%.3f;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%.3f;%d", 
		phase.idx[START],
		phase.when[START],
		mpi_type_str[phase.mpi_type[START]]+5,
		phase.idx[END],
		phase.when[END],
		mpi_type_str[phase.mpi_type[END]]+5,
		phase.timing[START],
		phase.timing[END],
		phase.timing[DURATION],
		phase.tsc,
		phase.load,
		phase.core_freq,
		phase.cpi,
		phase.inst_ret,
		phase.clk_curr,
		phase.clk_ref,
		phase.aperf,
		phase.mperf,
		phase.core_c0,
		phase.core_c1,
		phase.core_c3,
		phase.core_c6,
		phase.core_temp,
		phase.uncore_freq,
		phase.pkg_energy,
		phase.dram_energy,
		phase.pkg_power,
		phase.dram_power,
		phase.pkg_c0,
		phase.pkg_c2,
		phase.pkg_c3,
		phase.pkg_c6,
		phase.pkg_temp);

	if(cntd->pmu_perf_ctr)
	{
		fprintf(cntd->fd_adv_metrics, ";%lu;%lu;%lu;%lu", phase.pmu_1, phase.pmu_2, phase.pmu_3, phase.pmu_4);
		if(!arch->smt)
			fprintf(cntd->fd_adv_metrics, ";%lu;%lu;%lu;%lu", phase.pmu_5, phase.pmu_6, phase.pmu_7, phase.pmu_8);
	}

	// Network
	fprintf(cntd->fd_adv_metrics, ";%lu;%lu",
		adv_metrics_prev->tot_net_send,
		adv_metrics_prev->tot_net_recv);

	if(cntd->net_prof_ctr >= 2)
	{
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_adv_metrics, ";%lu", adv_metrics_prev->net_send[i]);
		for(i = 0; i < cntd->mpi_size; i++)
			fprintf(cntd->fd_adv_metrics, ";%lu", adv_metrics_prev->net_recv[i]);
	}

	fprintf(cntd->fd_adv_metrics, "\n");
	//fflush(cntd->fd_adv_metrics);
}

void print_custmetr()
{
	int i;

	CNTD_CustMetr_t *custmetr_app = &cntd->cust_metr[APP];
	CNTD_CustMetr_t *custmetr_mpi = &cntd->cust_metr[MPI];
	
	fprintf(cntd->fd_cust_metr, 
		";count_app;timing_app;load_app;core_freq_app;cpi_app"
		";count_mpi;timing_mpi;load_mpi;core_freq_mpi;cpi_mpi\n");

	for(i = 0; i < custmetr_enum_size; i++)
	{
		fprintf(cntd->fd_cust_metr, "%s;%lu;%.9f;%.3f;%.3f;%.3f;%lu;%.9f;%.3f;%.3f;%.3f\n",
			cust_metr_str[i],
			custmetr_app->count[i], 
			custmetr_app->timing[i],
			(custmetr_app->timing[i] > 0) ? custmetr_app->load[i] / custmetr_app->timing[i] : 0,
			(custmetr_app->timing[i] > 0) ? custmetr_app->core_freq[i] / custmetr_app->timing[i] : 0,
			(custmetr_app->timing[i] > 0) ? custmetr_app->cpi[i] / custmetr_app->timing[i] : 0,
			custmetr_mpi->count[i],
			custmetr_mpi->timing[i],
			(custmetr_mpi->timing[i] > 0) ? custmetr_mpi->load[i] / custmetr_mpi->timing[i] : 0,
			(custmetr_mpi->timing[i] > 0) ? custmetr_mpi->core_freq[i] / custmetr_mpi->timing[i] : 0,
			(custmetr_mpi->timing[i] > 0) ? custmetr_mpi->cpi[i] / custmetr_mpi->timing[i] : 0);
	}
}