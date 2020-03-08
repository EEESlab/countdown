/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *		  * Redistributions of source code must retain the above copyright notice, this
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

static CNTD_Andante_t *task = NULL;

void andante_init()
{
  static int init = FALSE;

  if(!init)
  {
    init = TRUE;
    cntd->andante_count = 0;
    cntd->andante_mem_limit = MEM_SIZE;

    cntd->andante_data = (CNTD_Andante_t *) calloc(MEM_SIZE, sizeof(CNTD_Andante_t));
  	if(cntd->andante_data == NULL)
  	{
  		fprintf(stderr, "Error: <countdown> Failed malloc for Andante structs!\n");
  		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  	}

    if(cntd->andante)
		{
			reset_pstate();
			reset_tstate();
		}
  }
}

void andante_finalize()
{
	if(cntd->andante)
	{
		reset_pstate();
		reset_tstate();
	}
}

static int isnew(int id, int where)
{
  int i;
  for(i = 0; i < cntd->andante_count; i++)
  {
    if(id == cntd->andante_data[i].task_id[where])
      return i;
  }

  return -1;
}

static CNTD_Andante_t* add_task(CNTD_Call_t *call, int task_id)
{
  check_mem_andante();

  CNTD_Andante_t *task = &cntd->andante_data[cntd->andante_count];
  cntd->andante_count++;

  memset(task, 0x0, sizeof(CNTD_Andante_t));
  task->task_id[START] = task_id;
  task->mpi_type[START] = call->mpi_type;
  task->task_id[END] = NOT_DEFINED;
  task->mpi_type[END] = NOT_DEFINED;

  return task;
}

static void andante_add_profiling(CNTD_Call_t *call, int when)
{
  int i;

  if(when == START)
  {
    task->epoch[START] = call->epoch[END];
    task->tsc[START] = call->tsc[END];
    task->fix[0][START] = call->fix[0][END];
    task->fix[1][START] = call->fix[1][END];
    task->fix[2][START] = call->fix[2][END];
    if(cntd->pmc)
  		for(i = 0; i < 8; i++)
  			task->pmc[i][START] = call->pmc[i][END];
  }
  else if(when == END)
  {
    task->epoch[END] = call->epoch[START];
    task->tsc[END] = call->tsc[START];
    task->fix[0][END] = call->fix[0][START];
    task->fix[1][END] = call->fix[1][START];
    task->fix[2][END] = call->fix[2][START];
    if(cntd->pmc)
  		for(i = 0; i < 8; i++)
  			task->pmc[i][END] = call->pmc[i][START];
  }
  else
  {
    fprintf(stderr, "Error: <countdown> Failed andante profiling!\n");
		PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
}


static double andante_update_profiling(CNTD_Call_t *call, int sched, int task_id)
{
  int i, idx_app, idx_slack;
  double mpi_slack;

  if(task->task_id[END] == NOT_DEFINED)
  {
    task->task_id[END] = task_id;
    task->mpi_type[END] = call->mpi_type;
    idx_app = APP;
    idx_slack = SLACK;
  }
  else if(task_id != task->task_id[END])
  {
    idx_app = MISS_APP;
    idx_slack = MISS_SLACK;
    task->miss_count++;
  }
  else
  {
    idx_app = APP;
    idx_slack = SLACK;
  }

  task->timing_p[idx_app] += task->epoch[END] - task->epoch[START];
  task->tsc_p[idx_app] += diff_64(task->tsc[END], task->tsc[START]);
  task->inst_ret_p[idx_app] += diff_48(task->fix[0][END], task->fix[0][START]);
  task->clk_curr_p[idx_app] += diff_48(task->fix[1][END], task->fix[1][START]);
  task->clk_ref_p[idx_app] += diff_48(task->fix[2][END], task->fix[2][START]);
  if(cntd->pmc)
    for(i = 0; i < 8; i++)
      task->pmc_p[idx_app][i] += diff_48(task->pmc[i][END], task->pmc[i][START]);

  if(is_cntd_barrier(call->mpi_type) || is_wait_barrier(call->mpi_type))
  {
    mpi_slack = call->epoch[END] - call->epoch[START];
    task->timing_p[idx_slack] += mpi_slack;
    task->tsc_p[idx_slack] += diff_64(call->tsc[END], call->tsc[START]);
    task->inst_ret_p[idx_slack] += diff_48(call->fix[0][END], call->fix[0][START]);
    task->clk_curr_p[idx_slack] += diff_48(call->fix[1][END], call->fix[1][START]);
    task->clk_ref_p[idx_slack] += diff_48(call->fix[2][END], call->fix[2][START]);
    if(cntd->pmc)
      for(i = 0; i < 8; i++)
        task->pmc_p[idx_slack][i] += diff_48(call->pmc[i][END], call->pmc[i][START]);
  }
  else
  {
    CNTD_Call_t *slack_call = &cntd->call[cntd->prev_call];

    mpi_slack = slack_call->epoch[END] - slack_call->epoch[START];
    task->timing_p[idx_slack] = mpi_slack;
    task->tsc_p[idx_slack] += diff_64(slack_call->tsc[END], slack_call->tsc[START]);
    task->inst_ret_p[idx_slack] += diff_48(slack_call->fix[0][END], slack_call->fix[0][START]);
    task->clk_curr_p[idx_slack] += diff_48(slack_call->fix[1][END], slack_call->fix[1][START]);
    task->clk_ref_p[idx_slack] += diff_48(slack_call->fix[2][END], slack_call->fix[2][START]);
    if(cntd->pmc)
      for(i = 0; i < 8; i++)
        task->pmc_p[idx_slack][i] += diff_48(slack_call->pmc[i][END], slack_call->pmc[i][START]);
  }

  task->count++;
  task->sched_p[sched] += 1;

  return mpi_slack;
}

// Generate the schedule for the next execution of this task
int andante_post_task(CNTD_Call_t *call, int prev_task_id)
{
  if(call->mpi_type == __MPI_INIT)
    return hash_backtrace(call->mpi_type);

  if(is_cntd_barrier(call->mpi_type) || is_wait_barrier(call->mpi_type))
  {
    int p;
    double Trates;
    int turbo_pstate, nom_pstate, min_pstate;

    // Profiling
    andante_add_profiling(call, END);

    // Stacktrace
    int task_id = hash_backtrace(call->mpi_type);

    // Update profiling
    double mpi_slack = andante_update_profiling(call, task->sched, task_id);

    turbo_pstate = cntd->arch.turbo_ratio[cntd->arch.num_procs_local_socket];
    nom_pstate = cntd->arch.pstate[NOM];
    min_pstate = cntd->arch.pstate[MIN];

    // trace andante task
    if(cntd->task_trace)
      print_task(task, call);

    // Rate the current task
    uint64_t IRcomp = diff_48(task->fix[0][END], task->fix[0][START]);
    double Tcomp = task->epoch[END] - task->epoch[START];
    double Ttarget = Tcomp + mpi_slack;
    task->rates[task->sched] = ((double) IRcomp) / Tcomp;

    // If it is the first time that the task is executed - initialize rates
    if(task->count == 1)
      for(p = min_pstate; p <= nom_pstate; p++)
        task->rates[p] = (task->rates[turbo_pstate] * (double) p) / (double) turbo_pstate;

    // Find the lowest frequency which respects the critical path - default max frequency
    task->sched = turbo_pstate;
    for(p = min_pstate; p <= nom_pstate; p++)
    {
      Trates = ((double) IRcomp) / task->rates[p];

      if(Trates <= Ttarget)
      {
        task->sched = p;
        break;
      }
    }

    return task_id;
  }

  return prev_task_id;
}

void andante_pre_task(CNTD_Call_t *call, int task_id)
{
  if(call->mpi_type == __MPI_FINALIZE)
    return;

  if(is_collective_barrier(call->mpi_type) ||
      is_p2p_barrier(call->mpi_type) ||
      is_wait_barrier(call->mpi_type))
  {
    // Look-up
    int idx = isnew(task_id, START);
    if(idx < 0)
    {
      task = add_task(call, task_id);
      task->sched = cntd->arch.turbo_ratio[cntd->arch.num_procs_local_socket];
    }
    else
      task = &cntd->andante_data[idx];

    andante_add_profiling(call, START);

    if(cntd->andante)
      write_pstate(task->sched);
  }
}
