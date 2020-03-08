/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *		      * Redistributions of source code must retain the above copyright notice, this
 *				      list of conditions and the following disclaimer.
 *
 *			    * Redistributions in binary form must reproduce the above copyright notice,
 *				      this list of conditions and the following disclaimer in the documentation
 *				      and/or other materials provided with the distribution.
 *
 *			    * Neither the name of the copyright holder nor the names of its
 *				      contributors may be used to endorse or promote products derived from
 *				      this software without specific prior written permission.
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

static CNTD_Fermata_t *fermata;
static CNTD_Call_t *callback_call;

void fermata_init()
{
  static int init = FALSE;

  if(!init)
  {
    init = TRUE;

    cntd->fermata_count = 0;
    cntd->fermata_mem_limit = MEM_SIZE;

    cntd->fermata_data = (CNTD_Fermata_t *) calloc(MEM_SIZE, sizeof(CNTD_Fermata_t));
  	if(cntd->fermata_data == NULL)
  	{
  		fprintf(stderr, "Error: <countdown> Failed malloc for fermata structs!\n");
  		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  	}

    // Install timer_handler as the signal handler for SIGALRM.
    struct sigaction sa = {{0}};
    sa.sa_handler = &fermata_call_back;
    sigaction(SIGALRM, &sa, NULL);

    if(cntd->fermata)
  	{
  		reset_pstate();
  		reset_tstate();
  	}
  }
}

void fermata_finalize()
{
  struct itimerval timer = {{0}};
	setitimer(ITIMER_REAL, &timer, NULL);

	if(cntd->fermata)
	{
		reset_pstate();
		reset_tstate();
	}
}

static void update_profiling(CNTD_Call_t *call)
{
  int idx;

  if(call->flag_eam)
  {
    idx = EAM;
    fermata->low_freq_count++;
  }
  else
    idx = NO_EAM;

  fermata->last_time_duration = call->epoch[END] - call->epoch[START];

  fermata->net[SEND] += call->net[SEND];
  fermata->net[RECV] += call->net[RECV];
  fermata->file[READ] += call->file[READ];
  fermata->file[WRITE] += call->file[WRITE];

  fermata->timing_p[idx] = call->epoch[END] - call->epoch[START];
  fermata->tsc_p[idx] = diff_64(call->tsc[END], call->tsc[START]);
  fermata->inst_ret_p[idx] += diff_48(call->fix[0][END], call->fix[0][START]);
  fermata->clk_curr_p[idx] += diff_48(call->fix[1][END], call->fix[1][START]);
  fermata->clk_ref_p[idx] += diff_48(call->fix[2][END], call->fix[2][START]);
  if(cntd->pmc)
  {
    int i;
    for(i = 0; i < 8; i++)
      fermata->pmc_p[idx][i] += diff_48(call->pmc[i][END], call->pmc[i][START]);
  }

  fermata->count++;
}

static CNTD_Fermata_t* add_fermata(CNTD_Call_t *call, int stacktrace)
{
  check_mem_fermata();

  CNTD_Fermata_t *fermata = &cntd->fermata_data[cntd->fermata_count];
  cntd->fermata_count++;

  memset(fermata, 0x0, sizeof(CNTD_Fermata_t));
  fermata->stacktrace = stacktrace;
  fermata->mpi_type = call->mpi_type;

  return fermata;
}

static int isnew(int stacktrace)
{
  int i;
  for(i = 0; i < cntd->fermata_count; i++)
    if(stacktrace == cntd->fermata_data[i].stacktrace)
      return i;

  return -1;
}

void fermata_call_back(int signum)
{
  callback_call->flag_eam = TRUE;
  fermata->flag_eam = TRUE;

  if(cntd->fermata)
    write_pstate(cntd->arch.pstate[MIN]);
}

void fermata_pre_mpi(CNTD_Call_t *call)
{
  if(is_collective_barrier(call->mpi_type) ||
     is_p2p_barrier(call->mpi_type) ||
     is_wait_barrier(call->mpi_type) ||
     is_cntd_barrier(call->mpi_type))
  {
    if(cntd->barrier && (is_collective_barrier(call->mpi_type) || is_p2p_barrier(call->mpi_type)))
       return;

    // Stacktrace
    int stacktrace = hash_backtrace(call->mpi_type);

    int idx = isnew(stacktrace);
    if(idx < 0)
      fermata = add_fermata(call, stacktrace);
    else
      fermata = &cntd->fermata_data[idx];

    if(fermata->count > 0 && fermata->last_time_duration > (2*cntd->timeout))
    {
      callback_call = call;

      if(cntd->timeout == 0)
        fermata_call_back(0x0);
      else
      {
        struct itimerval timer = {{0}};
    		timer.it_value.tv_usec = (unsigned long) (cntd->timeout * 1.0E6);
    		setitimer(ITIMER_REAL, &timer, NULL);
      }
    }
  }
}

void fermata_post_mpi(CNTD_Call_t *call)
{
  if(is_collective_barrier(call->mpi_type) ||
     is_p2p_barrier(call->mpi_type) ||
     is_wait_barrier(call->mpi_type))
  {
    // Reset timer
    struct itimerval timer = {{0}};
  	setitimer(ITIMER_REAL, &timer, NULL);

    // Set max frequency
    if(call->flag_eam && cntd->fermata)
			write_pstate(cntd->arch.pstate[MAX]);

    update_profiling(call);
  }
}
