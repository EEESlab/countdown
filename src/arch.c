/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *		  * Redistributions of source code must retain the above copyright notice, this
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
*/

#include "cntd.h"

static void conf_uncore_freq(int batch, uint64_t conf)
{
  int i;

  uint64_t **uclk = (uint64_t **) libmsr_calloc(num_sockets(), sizeof(uint64_t *));
  allocate_batch(batch, num_sockets());
  load_socket_batch(MSR_U_PMON_UCLK_FIXED_CTL, uclk, batch);

  for(i = 0; i < num_sockets(); i++)
      *uclk[i] = conf;

  write_batch(batch);
}

void enable_uncore_freq()
{
  conf_uncore_freq(ENABLE_UNC_FREQ_BATCH, CONF_UNCORE_FREQ_ENABLE);
}

void disable_uncore_freq()
{
  conf_uncore_freq(DISABLE_UNC_FREQ_BATCH, 0);
}

void get_brand_string(char *cpu_model_name)
{
  FILE *fd;
  char buffer[STRING_SIZE];
  char *result;


  fd = fopen("/proc/cpuinfo", "r");
  if (fd == NULL)
  {
      fprintf(stderr, "Error: <countdown> Failed to read file: /proc/cpuinfo\n");
      PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  while(TRUE)
  {
      result = fgets(buffer, STRING_SIZE, fd);
      if(result == NULL)
          break;

      if(!strncmp(result, "model name", 10))
      {
          result = strtok(result, ":");
          result = strtok(NULL, ":");
          result++;
          result[strlen(result)-1] = '\0';
          strcpy(cpu_model_name, result);
          break;
      }
  }

  fclose(fd);
}

void read_arch_info()
{
  uint64_t platform_info;
  FILE *fd_min, *fd_max;
  char min_pstate_str[STRING_SIZE], max_pstate_str[STRING_SIZE];
  char cpu_model_name[STRING_SIZE];

  // Topology
  cntd->rank->cpu_id = sched_getcpu();
  cntd->arch.cpus = num_devs();
  cntd->arch.cores = num_cores();
  cntd->arch.sockets = num_sockets();
  cntd->arch.ht_enable = cntd->arch.cpus == cntd->arch.cores ? 0 : 1;
  cntd->rank->socket_id = (int) (cntd->rank->cpu_id / (cntd->arch.cpus / cntd->arch.sockets));

  get_brand_string(cpu_model_name);
  strcpy(cntd->arch.cpu_model_name, cpu_model_name);

  // P-states
  fd_min = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", "r");
  fd_max = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");

  if(fd_min == NULL || fd_max == NULL)
  {
      fprintf(stderr, "Error: <countdown> Failed read file /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq and/or cpuinfo_max_freq!\n");
      PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  if(fread(min_pstate_str, STRING_SIZE, 1, fd_min) < 1)
  {
    fprintf(stderr, "Error: <countdown> Failed to read the minimum P-state!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if(fread(max_pstate_str, STRING_SIZE, 1, fd_max) < 1)
  {
    fprintf(stderr, "Error: <countdown> Failed to read the maximum P-state!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  if(!cntd->arch.pstate[MIN])
    cntd->arch.pstate[MIN] = atoi(min_pstate_str) / 1E5;
  if(!cntd->arch.pstate[MAX])
    cntd->arch.pstate[MAX] = atoi(max_pstate_str) / 1E5;

  fclose(fd_min);
  fclose(fd_max);

  if(cntd->arch.pstate[MAX] < cntd->arch.pstate[MIN])
  {
    fprintf(stderr, "Error: <countdown> Max P-state cannot less than min P-state!\n");
    PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  // Nominal frequency
  read_msr_by_idx(sched_getcpu(), SMSR_PLATFORM_INFO, &platform_info);
  cntd->arch.nominal_freq = ((platform_info >> 0x8) & 0xFF) * 100.0;
  cntd->arch.pstate[NOM] = cntd->arch.nominal_freq / 100.0;

  // Rapl units
  cntd->ru = (struct rapl_units *) libmsr_calloc(num_sockets(), sizeof(struct rapl_units));
  get_rapl_power_unit(cntd->ru);
}

void read_arch_turbo_info()
{
  int i;

  if((cntd->arch.cores/cntd->arch.sockets) > 24)
  {
    fprintf(stderr, "Error: <countdown> Fix turbo ratio discovery in: arch.c - read_arch_turbo_info()!\n");
    PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  uint64_t data, data1, data2;
  read_msr_by_idx(cntd->rank->cpu_id, 0x1AD, &data);
  read_msr_by_idx(cntd->rank->cpu_id, 0x1AE, &data1);
  read_msr_by_idx(cntd->rank->cpu_id, 0x1AF, &data2);

  for(i = 0; i < (cntd->arch.cores/cntd->arch.sockets); i++)
  {
    if(i < 8)
      cntd->arch.turbo_ratio[i+1] = ((data >> (i*8)) & 0xFF);
    else if(i < 16)
      cntd->arch.turbo_ratio[i+1] = ((data1 >> (i*8)) & 0xFF);
    else if(i < 24)
      cntd->arch.turbo_ratio[i+1] = ((data2 >> (i*8)) & 0xFF);
  }

  for(i = 0; i < cntd->local_size; i++)
		if(cntd->shmem_local_rank[i]->socket_id == cntd->rank->socket_id)
      cntd->arch.num_procs_local_socket++;
}
