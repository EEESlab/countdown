#include "cntd.h"

uint64_t RDTSC()
{
    unsigned a, d;
    __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
    return ((unsigned long) a) | (((unsigned long) d) << 32);
}

uint64_t RDTSCP()
{
    unsigned a, d;
    __asm__ volatile("rdtscp" : "=a"(a), "=d"(d));
    return ((unsigned long) a) | (((unsigned long) d) << 32);
}

uint64_t RDPMC(unsigned pmc)
{
   unsigned a, d;
   __asm__ volatile("rdpmc" : "=a" (a), "=d" (d) : "c" (pmc));
   return ((unsigned long)a) | (((unsigned long)d) << 32);
}

void CPUID(unsigned int *regs)
{
    __asm__ volatile("cpuid"
        : "=a" (regs[eax]),  // out EAX
          "=b" (regs[ebx]),  // out EBX
          "=c" (regs[ecx]),  // out ECX
          "=d" (regs[edx])   // out EDX
        : "a" (regs[eax]),   // in EAX
          "b" (regs[ebx]),   // in EBX
          "c" (regs[ecx]),   // in ECX
          "d" (regs[ebx]));  // in EDX
}

void add_timing(CNTD_Call_t *call, int when)
{
	struct timespec epoch;

	call->tsc[when] = READ_TSC();
	clock_gettime(CLOCK_TYPE, &epoch);
	call->epoch[when] = timespec2double(epoch);
}

void add_perf(CNTD_Call_t *call, int when)
{
	if(cntd->fix_perf_ctr)
	{
		call->fixed_1[when] = RDPMC(RDPMC_INSTR);
		call->fixed_2[when] = RDPMC(RDPMC_CLKCURR);
		call->fixed_3[when] = RDPMC(RDPMC_CLKREF);
	}

	if(cntd->pmu_perf_ctr)
	{
		call->pmu_1[when] = RDPMC(0);
		call->pmu_2[when] = RDPMC(1);
		call->pmu_3[when] = RDPMC(2);
		call->pmu_4[when] = RDPMC(3);
		if(!arch->smt)
		{
			call->pmu_5[when] = RDPMC(4);
			call->pmu_6[when] = RDPMC(5);
			call->pmu_7[when] = RDPMC(6);
			call->pmu_8[when] = RDPMC(7);
		}
	}
}

void add_network(CNTD_Call_t *call, 
	const int *send_count, MPI_Datatype *send_type, int dest, 
	const int *recv_count, MPI_Datatype *recv_type, int source)
{
	int i, payload, send_size, recv_size;
	CNTD_Group_t *group;
	CNTD_AdvMetrics_t *adv_metric = &cntd->adv_metrics[cntd->adv_metrics_curr];

	// Send
	if(dest == MPI_NONE)
	{}
	else if(dest == MPI_ALL)
	{
		PMPI_Type_size(*send_type, &send_size);

		payload = (*send_count) * send_size;
		group = call->cntd_comm->cntd_group;

		call->tot_net_send = payload * group->size;
		adv_metric->tot_net_send += call->tot_net_send;
		cntd->tot_net_send += call->tot_net_send;

		if(cntd->net_prof_ctr)
		{
			for(i = 0; i < group->size; i++)
			{
				switch(cntd->net_prof_ctr)
				{
					case 3:
						call->net_send[group->world_ranks[i]] = payload;
					case 2:
						adv_metric->net_send[group->world_ranks[i]] += payload;
					case 1:
						cntd->net_send[group->world_ranks[i]] += payload;
				}
			}
		}
	}
	else if(dest == MPI_ALLV)
	{
		PMPI_Type_size(*send_type, &send_size);
		group = call->cntd_comm->cntd_group;
		for(i = 0; i < group->size; i++)
		{
			payload = send_count[i] * send_size;
			call->tot_net_send += payload;
			adv_metric->tot_net_send += payload;
			cntd->tot_net_send += payload;
			if(cntd->net_prof_ctr)
			{
				call->net_send[group->world_ranks[i]] = payload;
				adv_metric->net_send[group->world_ranks[i]] += payload;
				cntd->net_send[group->world_ranks[i]] += payload;
			}
		}
	}
	else if(dest == MPI_ALLW)
	{
		group = call->cntd_comm->cntd_group;
		for(i = 0; i < group->size; i++)
		{
			PMPI_Type_size(send_type[i], &send_size);
			payload = send_count[i] * send_size;
			call->tot_net_send += payload;
			adv_metric->tot_net_send += payload;
			cntd->tot_net_send += payload;
			if(cntd->net_prof_ctr)
			{
				call->net_send[group->world_ranks[i]] = payload;
				adv_metric->net_send[group->world_ranks[i]] += payload;
				cntd->net_send[group->world_ranks[i]] += payload;
			}
		}
	}
	else
	{
		PMPI_Type_size(*send_type, &send_size);
		payload = (*send_count) * send_size;
		call->tot_net_send = payload;
		adv_metric->tot_net_send += payload;
		cntd->tot_net_send += payload;
		group = call->cntd_comm->cntd_group;
		if(cntd->net_prof_ctr)
		{
			call->net_send[group->world_ranks[dest]] = payload;
			adv_metric->net_send[group->world_ranks[dest]] += payload;
			cntd->net_send[group->world_ranks[dest]] += payload;
		}
	}

	// Receive
	if(source == MPI_NONE)
	{}
	else if(source == MPI_ALL)
	{
		PMPI_Type_size(*recv_type, &recv_size);
		payload = (*recv_count) * recv_size;
		group = call->cntd_comm->cntd_group;
		call->tot_net_recv = payload * group->size;
		adv_metric->tot_net_recv += call->tot_net_recv;
		cntd->tot_net_recv += call->tot_net_recv;

		if(cntd->net_prof_ctr)
		{
			for(i = 0; i < group->size; i++)
			{
				switch(cntd->net_prof_ctr)
				{
					case 3:
						call->net_recv[group->world_ranks[i]] = payload;
					case 2:
						adv_metric->net_recv[group->world_ranks[i]] += payload;
					case 1:
						cntd->net_recv[group->world_ranks[i]] += payload;
				}
			}
		}
	}
	else if(source == MPI_ALLV)
	{
		group = call->cntd_comm->cntd_group;
		PMPI_Type_size(*recv_type, &recv_size);
		for(i = 0; i < group->size; i++)
		{
			payload = recv_count[i] * recv_size;
			call->tot_net_recv = payload;
			adv_metric->tot_net_recv += payload;
			cntd->tot_net_recv += payload;
			if(cntd->net_prof_ctr)
			{
				call->net_recv[group->world_ranks[i]] = payload;
				adv_metric->net_recv[group->world_ranks[i]] += payload;
				cntd->net_recv[group->world_ranks[i]] += payload;
			}
		}
	}
	else if(source == MPI_ALLW)
	{
		group = call->cntd_comm->cntd_group;
		for(i = 0; i < group->size; i++)
		{
			PMPI_Type_size(recv_type[i], &recv_size);
			payload = recv_count[i] * recv_size;
			call->tot_net_recv = payload;
			adv_metric->tot_net_recv += payload;
			cntd->tot_net_recv += payload;
			if(cntd->net_prof_ctr)
			{
				call->net_recv[group->world_ranks[i]] = payload;
				adv_metric->net_recv[group->world_ranks[i]] += payload;
				cntd->net_recv[group->world_ranks[i]] += payload;
			}
		}
	}
	else
	{
		PMPI_Type_size(*recv_type, &recv_size);
		payload = (*recv_count) * recv_size;
		call->tot_net_recv = payload;
		adv_metric->tot_net_recv += payload;
		cntd->tot_net_recv += payload;
		group = call->cntd_comm->cntd_group;
		if(cntd->net_prof_ctr)
		{
			call->net_recv[group->world_ranks[source]] = payload;
			adv_metric->net_recv[group->world_ranks[source]] += payload;
			cntd->net_recv[group->world_ranks[source]] += payload;
		}
	}
}

void add_storage(CNTD_Call_t *call, 
	int read_count, MPI_Datatype read_datatype,
	int write_count, MPI_Datatype write_datatype)
{
	
}

void update_adv_metrics(CNTD_Call_t *call, int when)
{
	if(cntd->adv_metrics_curr == 0)
	{
		cntd->adv_metrics_curr = 1;
		cntd->adv_metrics_prev = 0;
	}
	else
	{
		cntd->adv_metrics_curr = 0;
		cntd->adv_metrics_prev = 1;
	}

	CNTD_AdvMetrics_t *adv_metric = &cntd->adv_metrics[cntd->adv_metrics_curr];
	memset(adv_metric, 0, sizeof(*adv_metric));

	// MSR reads
	// Core
	adv_metric->core_mperf = read_msr(IA32_MPERF);
	adv_metric->core_aperf = read_msr(IA32_APERF);
	adv_metric->core_temp = read_msr(MSR_IA32_THERM_STATUS);
	adv_metric->core_c3 = read_msr(MSR_CORE_C3_RESIDENCY);
	adv_metric->core_c6 = read_msr(MSR_CORE_C6_RESIDENCY);
	// package
	adv_metric->pkg_temp = read_msr(MSR_PACKAGE_THERM_STATUS);
	adv_metric->uncore_clk = read_msr(MSR_U_PMON_UCLK_FIXED_CTR);
	adv_metric->pkg_energy = read_msr(MSR_PKG_ENERGY_STATUS);
	adv_metric->dram_energy = read_msr(MSR_DRAM_ENERGY_STATUS);
	adv_metric->pkg_c2 = read_msr(MSR_PKG_C2_RESIDENCY);
	adv_metric->pkg_c3 = read_msr(MSR_PKG_C3_RESIDENCY);
	adv_metric->pkg_c6 = read_msr(MSR_PKG_C6_RESIDENCY);

	adv_metric->call_idx = call->idx;
	adv_metric->when = when;
	adv_metric->mpi_type = call->mpi_type;

	adv_metric->tsc = call->tsc[when];
	adv_metric->epoch = call->epoch[when];

	adv_metric->fixed_1 = RDPMC(RDPMC_INSTR);
	adv_metric->fixed_2 = RDPMC(RDPMC_CLKCURR);
	adv_metric->fixed_3 = RDPMC(RDPMC_CLKREF);

	if(cntd->pmu_perf_ctr)
	{
		adv_metric->pmu_1 = call->pmu_1[when];
		adv_metric->pmu_2 = call->pmu_2[when];
		adv_metric->pmu_3 = call->pmu_3[when];
		adv_metric->pmu_4 = call->pmu_4[when];
		if(!arch->smt)
		{
			adv_metric->pmu_5 = call->pmu_5[when];
			adv_metric->pmu_6 = call->pmu_6[when];
			adv_metric->pmu_7 = call->pmu_7[when];
			adv_metric->pmu_8 = call->pmu_8[when];
		}
	}

	cntd->adv_metrics_last_epoch = call->epoch[when];
}

void check_adv_metrics(CNTD_Call_t *call, int when)
{
	if(cntd->adv_metrics_ctr)
	{
		double elapse_time = call->epoch[when] - cntd->adv_metrics_last_epoch;
		if(elapse_time > cntd->adv_metrics_timeout)
		{
			update_adv_metrics(call, when);
			CNTD_AdvMetrics_Phase_t data = calc_adv_metrics();
			update_cntd_advmetr(data);
			print_adv_metrics(data);
		}
	}
}

void update_curr_call()
{
	if(cntd->curr_call == 0)
	{
		cntd->curr_call = 1;
		cntd->prev_call = 0;
	}
	else
	{
		cntd->curr_call = 0;
		cntd->prev_call = 1;
	}
}

void update_cntd_advmetr(CNTD_AdvMetrics_Phase_t data)
{
	cntd->inst_ret += (data.inst_ret);
	cntd->load += (data.load * data.timing[DURATION]);
	cntd->core_freq += (data.core_freq * data.timing[DURATION]);
	cntd->cpi += (data.cpi * data.timing[DURATION]);

	if(cntd->pmu_perf_ctr)
	{
		cntd->pmu_1 += data.pmu_1;
		cntd->pmu_2 += data.pmu_2;
		cntd->pmu_3 += data.pmu_3;
		cntd->pmu_4 += data.pmu_4;
		if(!arch->smt)
		{
			cntd->pmu_5 += data.pmu_5;
			cntd->pmu_6 += data.pmu_6;
			cntd->pmu_7 += data.pmu_7;
			cntd->pmu_8 += data.pmu_8;
		}
	}

	cntd->core_c0 += (data.core_c0 * data.timing[DURATION]);
	cntd->core_c1 += (data.core_c1 * data.timing[DURATION]);
	cntd->core_c3 += (data.core_c3 * data.timing[DURATION]);
	cntd->core_c6 += (data.core_c6 * data.timing[DURATION]);

	cntd->core_temp += (data.core_temp * data.timing[DURATION]);

	cntd->uncore_freq += (data.uncore_freq * data.timing[DURATION]);

	cntd->pkg_energy += data.pkg_energy;
	cntd->dram_energy += data.dram_energy;

	cntd->pkg_c0 += (data.pkg_c0 * data.timing[DURATION]);
	cntd->pkg_c2 += (data.pkg_c2 * data.timing[DURATION]);
	cntd->pkg_c3 += (data.pkg_c3 * data.timing[DURATION]);
	cntd->pkg_c6 += (data.pkg_c6 * data.timing[DURATION]);

	cntd->pkg_temp += (data.pkg_temp * data.timing[DURATION]);
}