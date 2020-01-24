/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *		* Redistributions of source code must retain the above copyright notice, this
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
 * Date: 24.08.2018
*/

#include "cntd.h"

static CNTD_Call_t *callback_call;

static void eam_policy_scheduler(CNTD_Call_t *call)
{
	if(is_collective_barrier(call->mpi_type) ||
	   is_p2p_barrier(call->mpi_type) ||
	 	 is_wait_barrier(call->mpi_type) ||
	   is_cntd_barrier(call->mpi_type))
		 	call->eam_policy = TRUE;
	else
			call->eam_policy = TRUE;
}

void eam_callback(int signum)
{
	callback_call->flag_eam = TRUE;

	if(cntd->eam_call)
		write_pstate(cntd->arch.pstate[MIN]);
}

void eam_pre_mpi(CNTD_Call_t *call)
{
	if(cntd->barrier && (is_collective_barrier(call->mpi_type) || is_p2p_barrier(call->mpi_type)))
		 return;

	eam_policy_scheduler(call);

	// Start timer
	if(call->eam_policy)
	{
		callback_call = call;

		if(cntd->timeout == 0)
			eam_callback(0x0);
		else
		{
			struct itimerval timer = {{0}};
			timer.it_value.tv_usec = (unsigned long) (cntd->timeout * 1.0E6);
			setitimer(ITIMER_REAL, &timer, NULL);
		}
	}
}

void eam_post_mpi(CNTD_Call_t *call)
{
	if(is_cntd_barrier(call->mpi_type))
		 return;

	if(call->eam_policy)
	{
		struct itimerval timer = {{0}};

		// Reset timer
		setitimer(ITIMER_REAL, &timer, NULL);

		// Set maximum frequency
		if(call->flag_eam && cntd->eam_call)
			write_pstate(cntd->arch.pstate[MAX]);
	}
}

void eam_init()
{
	static int init = FALSE;

	if(!init)
	{
		init = TRUE;
		struct sigaction sa = {{0}};

		if(cntd->eam_call)
		{
			reset_pstate();
			reset_tstate();
		}

		// Install timer_handler as the signal handler for SIGALRM.
		sa.sa_handler = &eam_callback;
		sigaction(SIGALRM, &sa, NULL);
	}
}

void eam_finalize()
{
	struct itimerval timer = {{0}};
	setitimer(ITIMER_REAL, &timer, NULL);

	if(cntd->eam_call)
	{
		reset_pstate();
		reset_tstate();
	}
}
