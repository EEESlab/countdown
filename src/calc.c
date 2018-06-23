#include "cntd.h"

void update_cust_metr(CNTD_Call_Phase_t phase)
{
	int i;
	CNTD_CustMetr_t *cust_metr;

	for(i = 0; i < 2; i++)
	{
		cust_metr = &cntd->cust_metr[i];

		if(phase.timing[i] < 0.5E-6)
		{
			cust_metr->timing[_500ns] += phase.timing[i];
			cust_metr->load[_500ns] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_500ns] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_500ns] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_500ns]++;
		}
		else if(phase.timing[i] < 5E-6)
		{
			cust_metr->timing[_500ns_5us] += phase.timing[i];
			cust_metr->load[_500ns_5us] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_500ns_5us] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_500ns_5us] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_500ns_5us]++;
		}
		else if(phase.timing[i] < 50E-6)
		{
			cust_metr->timing[_5us_50us] += phase.timing[i];
			cust_metr->load[_5us_50us] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_5us_50us] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_5us_50us] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_5us_50us]++;
		}
		else if(phase.timing[i] < 500E-6)
		{
			cust_metr->timing[_50us_500us] += phase.timing[i];
			cust_metr->load[_50us_500us] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_50us_500us] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_50us_500us] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_50us_500us]++;
		}
		else if(phase.timing[i] < 5E-3)
		{
			cust_metr->timing[_500us_5ms] += phase.timing[i];
			cust_metr->load[_500us_5ms] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_500us_5ms] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_500us_5ms] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_500us_5ms]++;
		}
		else if(phase.timing[i] < 50E-3)
		{
			cust_metr->timing[_5ms_50ms] += phase.timing[i];
			cust_metr->load[_5ms_50ms] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_5ms_50ms] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_5ms_50ms] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_5ms_50ms]++;
		}
		else if(phase.timing[i] < 500E-3)
		{
			cust_metr->timing[_50ms_500ms] += phase.timing[i];
			cust_metr->load[_50ms_500ms] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_50ms_500ms] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_50ms_500ms] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_50ms_500ms]++;
		}
		else if(phase.timing[i] < 5)
		{
			cust_metr->timing[_500ms_5s] += phase.timing[i];
			cust_metr->load[_500ms_5s] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_500ms_5s] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_500ms_5s] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_500ms_5s]++;
		}
		else if(phase.timing[i] < 50)
		{
			cust_metr->timing[_5s_50s] += phase.timing[i];
			cust_metr->load[_5s_50s] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_5s_50s] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_5s_50s] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_5s_50s]++;
		}
		else if(phase.timing[i] < 500)
		{
			cust_metr->timing[_50s_500s] += phase.timing[i];
			cust_metr->load[_50s_500s] += phase.load[i] * phase.timing[i];
			cust_metr->core_freq[_50s_500s] += phase.core_freq[i] * phase.timing[i];
			cust_metr->cpi[_50s_500s] += phase.cpi[i] * phase.timing[i];
			cust_metr->count[_50s_500s]++;
		}
	}
}

CNTD_Call_Phase_t calc_call()
{
	CNTD_Call_Phase_t phase = {0};

	CNTD_Call_t *curr = &cntd->call[cntd->curr_call];
	CNTD_Call_t *prev = &cntd->call[cntd->prev_call];

	phase.comm = curr->cntd_comm->idx;
	phase.idx = curr->idx;
	phase.mpi_type = curr->mpi_type;

	phase.epoch[0] = prev->epoch[END] - cntd->epoch[START];
	phase.epoch[1] = curr->epoch[START] - cntd->epoch[START];
	phase.epoch[2] = curr->epoch[END] - cntd->epoch[START];

	phase.timing[APP] = phase.epoch[1] - phase.epoch[0];
	phase.timing[MPI] = phase.epoch[2] - phase.epoch[1];

	phase.tsc[APP] = diff_64(curr->tsc[START], prev->tsc[END]);
	phase.tsc[MPI] = diff_64(curr->tsc[END], curr->tsc[START]);

	if(cntd->fix_perf_ctr)
	{
		phase.inst_ret[APP] = diff_48(curr->fixed_1[START], prev->fixed_1[END]);
		phase.inst_ret[MPI] = diff_48(curr->fixed_1[END], curr->fixed_1[START]);

		phase.clk_curr[APP] = diff_48(curr->fixed_2[START], prev->fixed_2[END]);
		phase.clk_curr[MPI] = diff_48(curr->fixed_2[END], curr->fixed_2[START]);

		phase.clk_ref[APP] = diff_48(curr->fixed_3[START], prev->fixed_3[END]);
		phase.clk_ref[MPI] = diff_48(curr->fixed_3[END], curr->fixed_3[START]);

		if(phase.clk_ref[APP] >= phase.tsc[APP])
			phase.load[APP] = 100.0;
		else if(phase.clk_ref[APP] == 0)
			phase.load[APP] = 0.0;
		else
			phase.load[APP] = ((double) phase.clk_ref[APP] * 100.0) / (double) phase.tsc[APP];

		if(phase.clk_ref[MPI] >= phase.tsc[MPI])
			phase.load[MPI] = 100.0;
		else if(phase.clk_ref[MPI] == 0)
			phase.load[MPI] = 0.0;
		else
			phase.load[MPI] = ((double) phase.clk_ref[MPI] * 100.0) / (double) phase.tsc[MPI];

		if(phase.clk_curr[APP] > 0 && phase.clk_ref[APP] > 0)
		{
			phase.core_freq[APP] = (phase.clk_curr[APP] * arch->nominal_freq) / phase.clk_ref[APP];
			phase.cpi[APP] = (double) phase.clk_ref[APP] / (double) phase.inst_ret[APP];
		}

		if(phase.clk_curr[MPI] > 0 && phase.clk_ref[MPI] > 0)
		{
			phase.core_freq[MPI] = (phase.clk_curr[MPI] * arch->nominal_freq) / phase.clk_ref[MPI];
			phase.cpi[MPI] = (double) phase.clk_ref[MPI] / (double) phase.inst_ret[MPI];
		}
	}

	if(cntd->pmu_perf_ctr)
	{
		phase.pmu_1[APP] = diff_48(curr->pmu_1[START], prev->pmu_1[END]);
		phase.pmu_2[APP] = diff_48(curr->pmu_2[START], prev->pmu_2[END]);
		phase.pmu_3[APP] = diff_48(curr->pmu_3[START], prev->pmu_3[END]);
		phase.pmu_4[APP] = diff_48(curr->pmu_4[START], prev->pmu_4[END]);

		phase.pmu_1[MPI] = diff_48(curr->pmu_1[START], prev->pmu_1[END]);
		phase.pmu_2[MPI] = diff_48(curr->pmu_2[START], prev->pmu_2[END]);
		phase.pmu_3[MPI] = diff_48(curr->pmu_3[START], prev->pmu_3[END]);
		phase.pmu_4[MPI] = diff_48(curr->pmu_4[START], prev->pmu_4[END]);

		if(!arch->smt)
		{
			phase.pmu_5[APP] = diff_48(curr->pmu_5[START], prev->pmu_5[END]);
			phase.pmu_6[APP] = diff_48(curr->pmu_6[START], prev->pmu_6[END]);
			phase.pmu_7[APP] = diff_48(curr->pmu_7[START], prev->pmu_7[END]);
			phase.pmu_8[APP] = diff_48(curr->pmu_8[START], prev->pmu_8[END]);

			phase.pmu_5[MPI] = diff_48(curr->pmu_5[START], prev->pmu_5[END]);
			phase.pmu_6[MPI] = diff_48(curr->pmu_6[START], prev->pmu_6[END]);
			phase.pmu_7[MPI] = diff_48(curr->pmu_7[START], prev->pmu_7[END]);
			phase.pmu_8[MPI] = diff_48(curr->pmu_8[START], prev->pmu_8[END]);
		}
	}

	phase.tot_net_send = curr->tot_net_send;
	phase.tot_net_recv = curr->tot_net_recv;

	if(cntd->net_prof_ctr >= 3)
	{
		memcpy(phase.net_send, curr->net_send, sizeof(int) * cntd->mpi_size);
		memcpy(phase.net_recv, curr->net_recv, sizeof(int) * cntd->mpi_size);
	}

	update_cust_metr(phase);

	return phase;
}

CNTD_AdvMetrics_Phase_t calc_adv_metrics()
{
	CNTD_AdvMetrics_Phase_t phase;

	CNTD_AdvMetrics_t *prev = &cntd->adv_metrics[cntd->adv_metrics_prev];
	CNTD_AdvMetrics_t *curr = &cntd->adv_metrics[cntd->adv_metrics_curr];

	// Index start
	phase.idx[START] = prev->call_idx;
	if(prev->when == START)
		phase.when[START] = 's';
	else if(prev->when == END)
		phase.when[START] = 'e';
	else
		phase.when[START] = 'n';
	phase.mpi_type[START] = prev->mpi_type;
	// Index end
	phase.idx[END] = curr->call_idx;
	if(curr->when == START)
		phase.when[END] = 's';
	else if(curr->when == END)
		phase.when[END] = 'e';
	else
		phase.when[END] = 'n';
	phase.mpi_type[END] = curr->mpi_type;

	// TSC
	phase.tsc = diff_64(curr->tsc, prev->tsc);
	// Time
	phase.timing[START] = prev->epoch - cntd->epoch[START];
	phase.timing[END] = curr->epoch - cntd->epoch[START];
	phase.timing[DURATION] = phase.timing[END] - phase.timing[START];

	// Performance metrics
	phase.clk_curr = diff_48(curr->fixed_2, prev->fixed_2);
	phase.clk_ref = diff_48(curr->fixed_3, prev->fixed_3);
	phase.core_freq = (double) (phase.clk_curr * arch->nominal_freq) / (double) phase.clk_ref;

	phase.mperf = diff_64(curr->core_mperf, prev->core_mperf);
	phase.aperf = diff_64(curr->core_aperf, prev->core_aperf);

	phase.inst_ret = diff_48(curr->fixed_1, prev->fixed_1);
	phase.cpi = (double) phase.clk_ref / (double) phase.inst_ret;

	if(phase.clk_ref >= phase.tsc)
		phase.core_c0 = 100.0;
	else if(phase.clk_ref == 0)
		phase.core_c0 = 0.0;
	else
		phase.core_c0 = ((double) phase.clk_ref * 100.0) / (double) phase.tsc;

	phase.load = phase.core_c0;

	// Core C-states
	phase.core_c3 = (diff_64(curr->core_c3, prev->core_c3) * 100.0) / phase.tsc;
	if(phase.core_c3 > 100.0) phase.core_c3 = 100.0;
	else if(phase.core_c3 < 0.0) phase.core_c3 = 0.0;

	phase.core_c6 = (diff_64(curr->core_c6, prev->core_c6) * 100.0) / phase.tsc;
	if(phase.core_c6 > 100.0) phase.core_c6 = 100.0;
	else if(phase.core_c6 < 0.0) phase.core_c6 = 0.0;

	phase.core_c1 = 100.0 - phase.core_c0 - phase.core_c3 - phase.core_c6;
	if(phase.core_c1 > 100.0) phase.core_c1 = 100.0;
	else if(phase.core_c1 < 0.0) phase.core_c1 = 0.0;

	// Core temperature
	phase.core_temp = 100 - (((curr->core_temp & 0x7F0000) >> 16));

	// Package C-states
	phase.pkg_c2 = (diff_64(curr->pkg_c2, prev->pkg_c2) * 100.0) / phase.tsc;
	if(phase.pkg_c2 > 100.0) phase.pkg_c2 = 100.0;
	else if(phase.pkg_c2 < 0.0) phase.pkg_c2 = 0.0;

	phase.pkg_c3 = (diff_64(curr->pkg_c3, prev->pkg_c3) * 100.0) / phase.tsc;
	if(phase.pkg_c3 > 100.0) phase.pkg_c3 = 100.0;
	else if(phase.pkg_c3 < 0.0) phase.pkg_c3 = 0.0;

	phase.pkg_c6 = (diff_64(curr->pkg_c6, prev->pkg_c6) * 100.0) / phase.tsc;
	if(phase.pkg_c6 > 100.0) phase.pkg_c6 = 100.0;
	else if(phase.pkg_c6 < 0.0) phase.pkg_c6 = 0.0;

	phase.pkg_c0 = 100.0 - phase.pkg_c2 - phase.pkg_c3 - phase.pkg_c6;
	if(phase.pkg_c0 > 100.0) phase.pkg_c0 = 100.0;
	else if(phase.pkg_c0 < 0.0) phase.pkg_c0 = 0.0;

	// Uncore Frequency
	phase.uncore_freq = ((double) (diff_48(curr->uncore_clk, prev->uncore_clk) / phase.timing[DURATION]) * phase.pkg_c0) / 1.0E8;

	// Energy and Power
	phase.pkg_energy = (double) diff_32(curr->pkg_energy, prev->pkg_energy) * (double) arch->rapl_energy_unit;
	phase.dram_energy = (double) diff_32(curr->dram_energy, prev->dram_energy) * (double) arch->rapl_energy_unit;

	phase.pkg_power = phase.pkg_energy / phase.timing[DURATION];
	phase.dram_power = phase.dram_energy / phase.timing[DURATION];

	// Package temperature
	phase.pkg_temp = 100 - (((prev->pkg_temp & 0x3F0000) >> 16));

	if(cntd->pmu_perf_ctr)
	{
		phase.pmu_1 = diff_48(curr->pmu_1, prev->pmu_1);
		phase.pmu_2 = diff_48(curr->pmu_2, prev->pmu_2);
		phase.pmu_3 = diff_48(curr->pmu_3, prev->pmu_3);
		phase.pmu_4 = diff_48(curr->pmu_4, prev->pmu_4);
		if(!arch->smt)
		{
			phase.pmu_5 = diff_48(curr->pmu_5, prev->pmu_5);
			phase.pmu_6 = diff_48(curr->pmu_6, prev->pmu_6);
			phase.pmu_7 = diff_48(curr->pmu_7, prev->pmu_7);
			phase.pmu_8 = diff_48(curr->pmu_8, prev->pmu_8);
		}
	}

	return phase;
}