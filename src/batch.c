/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *      * Redistributions of source code must retain the above copyright notice, this
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


void init_batch_cpu(int batch, CNTD_Batch_Cpu_t *cpu)
{
    cpu->tsc = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->aperf = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->mperf = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->inst_ret = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->clk_curr = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->clk_ref = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->temp = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->temp_target = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->C3 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->C6 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->C7 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc0 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc1 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc2 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc3 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc4 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc5 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc6 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));
    cpu->pmc7 = (uint64_t **) libmsr_calloc(cntd->rank->cpus, sizeof(uint64_t *));

    allocate_batch(batch, (sizeof(CNTD_Batch_Cpu_t) / sizeof(uint64_t)) * cntd->rank->cpus);

    load_thread_batch(IA32_TIME_STAMP_COUNTER, cpu->tsc, batch);
    load_thread_batch(IA32_APERF, cpu->aperf, batch);
    load_thread_batch(IA32_MPERF, cpu->mperf, batch);
    load_thread_batch(IA32_FIXED_CTR0, cpu->inst_ret, batch);
    load_thread_batch(IA32_FIXED_CTR1, cpu->clk_curr, batch);
    load_thread_batch(IA32_FIXED_CTR2, cpu->clk_ref, batch);
    load_thread_batch(IA32_THERM_STATUS, cpu->temp, batch);
    load_thread_batch(MSR_TEMPERATURE_TARGET, cpu->temp_target, batch);
    load_thread_batch(MSR_CORE_C3_RESIDENCY, cpu->C3, batch);
    load_thread_batch(MSR_CORE_C6_RESIDENCY, cpu->C6, batch);
    load_thread_batch(MSR_CORE_C7_RESIDENCY, cpu->C7, batch);
    load_thread_batch(IA32_PMC0, cpu->pmc0, batch);
    load_thread_batch(IA32_PMC1, cpu->pmc1, batch);
    load_thread_batch(IA32_PMC2, cpu->pmc2, batch);
    load_thread_batch(IA32_PMC3, cpu->pmc3, batch);
    load_thread_batch(IA32_PMC4, cpu->pmc4, batch);
    load_thread_batch(IA32_PMC5, cpu->pmc5, batch);
    load_thread_batch(IA32_PMC6, cpu->pmc6, batch);
    load_thread_batch(IA32_PMC7, cpu->pmc7, batch);
}

void init_batch_socket(int batch, CNTD_Batch_Socket_t *socket)
{
    socket->energy_pkg = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->energy_dram = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->temp = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->uclk = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->C2 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->C3 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->C6 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->C7 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->pcu0 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->pcu1 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->pcu2 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));
    socket->pcu3 = (uint64_t **) libmsr_calloc(cntd->rank->sockets, sizeof(uint64_t *));

    allocate_batch(batch, (sizeof(CNTD_Batch_Socket_t) / sizeof(uint64_t)) * cntd->rank->sockets);

    load_socket_batch(MSR_PKG_ENERGY_STATUS, socket->energy_pkg, batch);
    load_socket_batch(MSR_DRAM_ENERGY_STATUS, socket->energy_dram, batch);
    load_socket_batch(IA32_PACKAGE_THERM_STATUS, socket->temp, batch);
    load_socket_batch(MSR_U_PMON_UCLK_FIXED_CTR, socket->uclk, batch);
    load_socket_batch(MSR_PKG_C2_RESIDENCY, socket->C2, batch);
    load_socket_batch(MSR_PKG_C3_RESIDENCY, socket->C3, batch);
    load_socket_batch(MSR_PKG_C6_RESIDENCY, socket->C6, batch);
    load_socket_batch(MSR_PKG_C7_RESIDENCY, socket->C7, batch);
    load_socket_batch(MSR_PCU_PMON_CTR0, socket->pcu0, batch);
    load_socket_batch(MSR_PCU_PMON_CTR1, socket->pcu1, batch);
    load_socket_batch(MSR_PCU_PMON_CTR2, socket->pcu2, batch);
    load_socket_batch(MSR_PCU_PMON_CTR3, socket->pcu3, batch);
}

// Make a sample
double sample_batch()
{
    struct timespec epoch;

    clock_gettime(CLOCK_TYPE, &epoch);
    read_batch(CPU_READ_BATCH);
    read_batch(SOCKET_READ_BATCH);

    return timespec2double(epoch) - cntd->epoch[START];
}

void update_batch(double epoch, CNTD_Cpu_t *cpu, CNTD_Socket_t *socket)
{
    int i, cpu_id;

    // Differential values
    for(i = 0; i < cntd->local_size; i++)
    {
        cntd->shmem_local_rank[i]->epoch_sample[PREV] = cntd->shmem_local_rank[i]->epoch_sample[CURR];
        cntd->shmem_local_rank[i]->epoch_sample[CURR] = epoch;
    }

    for(i = 0; i < cntd->rank->cpus; i++)
    {
        cpu[i].tsc = diff_64(*cntd->batch_cpu.tsc[i], cntd->last_batch_cpu[i].tsc);
        cpu[i].aperf = diff_64(*cntd->batch_cpu.aperf[i], cntd->last_batch_cpu[i].aperf);
        cpu[i].mperf = diff_64(*cntd->batch_cpu.mperf[i], cntd->last_batch_cpu[i].mperf);
        cpu[i].inst_ret = diff_48(*cntd->batch_cpu.inst_ret[i], cntd->last_batch_cpu[i].inst_ret);
        cpu[i].clk_curr = diff_48(*cntd->batch_cpu.clk_curr[i], cntd->last_batch_cpu[i].clk_curr);
        cpu[i].clk_ref = diff_48(*cntd->batch_cpu.clk_ref[i], cntd->last_batch_cpu[i].clk_ref);

        cpu[i].temp = (get_core_temp(*cntd->batch_cpu.temp[i], *cntd->batch_cpu.temp_target[i]) + cntd->last_batch_cpu[i].temp) / 2.0;
        
        cpu[i].C3 = diff_64(*cntd->batch_cpu.C3[i], cntd->last_batch_cpu[i].C3);
        cpu[i].C6 = diff_64(*cntd->batch_cpu.C6[i], cntd->last_batch_cpu[i].C6);
        cpu[i].C7 = diff_64(*cntd->batch_cpu.C7[i], cntd->last_batch_cpu[i].C7);

        cpu[i].pmc0 = diff_48(*cntd->batch_cpu.pmc0[i], cntd->last_batch_cpu[i].pmc0);
        cpu[i].pmc1 = diff_48(*cntd->batch_cpu.pmc1[i], cntd->last_batch_cpu[i].pmc1);
        cpu[i].pmc2 = diff_48(*cntd->batch_cpu.pmc2[i], cntd->last_batch_cpu[i].pmc2);
        cpu[i].pmc3 = diff_48(*cntd->batch_cpu.pmc3[i], cntd->last_batch_cpu[i].pmc3);
        cpu[i].pmc4 = diff_48(*cntd->batch_cpu.pmc4[i], cntd->last_batch_cpu[i].pmc4);
        cpu[i].pmc5 = diff_48(*cntd->batch_cpu.pmc5[i], cntd->last_batch_cpu[i].pmc5);
        cpu[i].pmc6 = diff_48(*cntd->batch_cpu.pmc6[i], cntd->last_batch_cpu[i].pmc6);
        cpu[i].pmc7 = diff_48(*cntd->batch_cpu.pmc7[i], cntd->last_batch_cpu[i].pmc7);
    }

    for(i = 0; i < cntd->rank->sockets; i++)
    {
    	cpu_id = (cntd->rank->cpus / cntd->rank->sockets) * i;

        socket[i].rapl_joules = cntd->socket[i].rapl_joules;
        socket[i].rapl_watts = cntd->socket[i].rapl_watts;
        socket[i].rapl_seconds = cntd->socket[i].rapl_seconds;

        socket[i].energy_pkg = diff_32(*cntd->batch_socket.energy_pkg[i], cntd->last_batch_socket[i].energy_pkg);
        socket[i].energy_dram = diff_32(*cntd->batch_socket.energy_dram[i], cntd->last_batch_socket[i].energy_dram);

        socket[i].temp = (get_pkg_temp(*cntd->batch_socket.temp[i], *cntd->batch_cpu.temp_target[cpu_id]) + cntd->last_batch_socket[i].temp) / 2.0;
        socket[i].uclk = diff_48(*cntd->batch_socket.uclk[i], cntd->last_batch_socket[i].uclk);

        socket[i].C2 = diff_64(*cntd->batch_socket.C2[i], cntd->last_batch_socket[i].C2);
        socket[i].C3 = diff_64(*cntd->batch_socket.C3[i], cntd->last_batch_socket[i].C3);
        socket[i].C6 = diff_64(*cntd->batch_socket.C6[i], cntd->last_batch_socket[i].C6);
        socket[i].C7 = diff_64(*cntd->batch_socket.C7[i], cntd->last_batch_socket[i].C7);

        socket[i].pcu0 = diff_48(*cntd->batch_socket.pcu0[i], cntd->last_batch_socket[i].pcu0);
        socket[i].pcu1 = diff_48(*cntd->batch_socket.pcu1[i], cntd->last_batch_socket[i].pcu1);
        socket[i].pcu2 = diff_48(*cntd->batch_socket.pcu2[i], cntd->last_batch_socket[i].pcu2);
        socket[i].pcu3 = diff_48(*cntd->batch_socket.pcu3[i], cntd->last_batch_socket[i].pcu3);
    }

    // Update global values
    for(i = 0; i < cntd->rank->cpus; i++)
    {
        cntd->cpu[i].num_samples++;

        cntd->cpu[i].tsc += cpu[i].tsc;
        cntd->cpu[i].aperf += cpu[i].aperf;
        cntd->cpu[i].mperf += cpu[i].mperf;
        cntd->cpu[i].inst_ret += cpu[i].inst_ret;
        cntd->cpu[i].clk_curr += cpu[i].clk_curr;
        cntd->cpu[i].clk_ref += cpu[i].clk_ref;

        cntd->cpu[i].temp += cpu[i].temp;

        cntd->cpu[i].C3 += cpu[i].C3;
        cntd->cpu[i].C6 += cpu[i].C6;
        cntd->cpu[i].C7 += cpu[i].C7;

        cntd->cpu[i].pmc0 += cpu[i].pmc0;
        cntd->cpu[i].pmc1 += cpu[i].pmc1;
        cntd->cpu[i].pmc2 += cpu[i].pmc2;
        cntd->cpu[i].pmc3 += cpu[i].pmc3;
        cntd->cpu[i].pmc4 += cpu[i].pmc4;
        cntd->cpu[i].pmc5 += cpu[i].pmc5;
        cntd->cpu[i].pmc6 += cpu[i].pmc6;
        cntd->cpu[i].pmc7 += cpu[i].pmc7;
    }

    for(i = 0; i < cntd->rank->sockets; i++)
    {
        cntd->socket[i].num_samples++;

        cntd->socket[i].energy_pkg += socket[i].energy_pkg;
        cntd->socket[i].energy_dram += socket[i].energy_dram;

        cntd->socket[i].temp += socket[i].temp;
        cntd->socket[i].uclk += socket[i].uclk;

        cntd->socket[i].C2 += socket[i].C2;
        cntd->socket[i].C3 += socket[i].C3;
        cntd->socket[i].C6 += socket[i].C6;
        cntd->socket[i].C7 += socket[i].C7;

		cntd->socket[i].pcu0 += socket[i].pcu0;
        cntd->socket[i].pcu1 += socket[i].pcu1;
        cntd->socket[i].pcu2 += socket[i].pcu2;
        cntd->socket[i].pcu3 += socket[i].pcu3;
    }
}

void update_last_batch(double epoch)
{
    int i;

    for(i = 0; i < cntd->local_size; i++)
    {
        cntd->last_batch_rank[i].phase = cntd->shmem_local_rank[i]->phase;
        cntd->last_batch_rank[i].epoch[START] = cntd->shmem_local_rank[i]->epoch[START];
        cntd->last_batch_rank[i].epoch[END] = cntd->shmem_local_rank[i]->epoch[END];

    	cntd->last_batch_rank[i].net[SEND] = cntd->shmem_local_rank[i]->net[SEND];
    	cntd->last_batch_rank[i].net[RECV] = cntd->shmem_local_rank[i]->net[RECV];

    	cntd->last_batch_rank[i].file[READ] = cntd->shmem_local_rank[i]->file[READ];
    	cntd->last_batch_rank[i].file[WRITE] = cntd->shmem_local_rank[i]->file[WRITE];

        cntd->last_batch_rank[i].timing[APP] = cntd->shmem_local_rank[i]->timing[APP];
        cntd->last_batch_rank[i].timing[MPI] = cntd->shmem_local_rank[i]->timing[MPI];
    }

    for(i = 0; i < cntd->rank->cpus; i++)
    {
    	cntd->last_batch_cpu[i].tsc = *cntd->batch_cpu.tsc[i];
    	cntd->last_batch_cpu[i].aperf = *cntd->batch_cpu.aperf[i];
    	cntd->last_batch_cpu[i].mperf = *cntd->batch_cpu.mperf[i];
    	cntd->last_batch_cpu[i].inst_ret = *cntd->batch_cpu.inst_ret[i];
    	cntd->last_batch_cpu[i].clk_curr = *cntd->batch_cpu.clk_curr[i];
    	cntd->last_batch_cpu[i].clk_ref = *cntd->batch_cpu.clk_ref[i];
    	cntd->last_batch_cpu[i].temp = get_core_temp(*cntd->batch_cpu.temp[i], *cntd->batch_cpu.temp_target[i]);

    	cntd->last_batch_cpu[i].C3 = *cntd->batch_cpu.C3[i];
    	cntd->last_batch_cpu[i].C6 = *cntd->batch_cpu.C6[i];
    	cntd->last_batch_cpu[i].C7 = *cntd->batch_cpu.C7[i];

    	cntd->last_batch_cpu[i].pmc0 = *cntd->batch_cpu.pmc0[i];
    	cntd->last_batch_cpu[i].pmc1 = *cntd->batch_cpu.pmc1[i];
    	cntd->last_batch_cpu[i].pmc2 = *cntd->batch_cpu.pmc2[i];
    	cntd->last_batch_cpu[i].pmc3 = *cntd->batch_cpu.pmc3[i];
    	cntd->last_batch_cpu[i].pmc4 = *cntd->batch_cpu.pmc4[i];
    	cntd->last_batch_cpu[i].pmc5 = *cntd->batch_cpu.pmc5[i];
    	cntd->last_batch_cpu[i].pmc6 = *cntd->batch_cpu.pmc6[i];
    	cntd->last_batch_cpu[i].pmc7 = *cntd->batch_cpu.pmc7[i];
    }

    for(i = 0; i < cntd->rank->sockets; i++)
    {
    	cntd->last_batch_socket[i].energy_pkg = *cntd->batch_socket.energy_pkg[i];
    	cntd->last_batch_socket[i].energy_dram = *cntd->batch_socket.energy_dram[i];

    	cntd->last_batch_socket[i].temp = get_pkg_temp(*cntd->batch_socket.temp[i], *cntd->batch_cpu.temp_target[i]);
    	cntd->last_batch_socket[i].uclk = *cntd->batch_socket.uclk[i];

    	cntd->last_batch_socket[i].C2 = *cntd->batch_socket.C2[i];
    	cntd->last_batch_socket[i].C3 = *cntd->batch_socket.C3[i];
    	cntd->last_batch_socket[i].C6 = *cntd->batch_socket.C6[i];
    	cntd->last_batch_socket[i].C7 = *cntd->batch_socket.C7[i];

    	cntd->last_batch_socket[i].pcu0 = *cntd->batch_socket.pcu0[i];
    	cntd->last_batch_socket[i].pcu1 = *cntd->batch_socket.pcu1[i];
    	cntd->last_batch_socket[i].pcu2 = *cntd->batch_socket.pcu2[i];
    	cntd->last_batch_socket[i].pcu3 = *cntd->batch_socket.pcu3[i];
    }
}