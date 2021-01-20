/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *			* Redistributions of source code must retain the above copyright notice, this
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

static int flag_eam = FALSE;

static void eam_callback(int signum)
{
	flag_eam = TRUE;
	if(!cntd->no_freq)
		set_min_pstate();
}

static void start_timer()
{
	struct itimerval timer = {{0}};
	timer.it_value.tv_usec = (unsigned long) cntd->timeout;
	setitimer(ITIMER_REAL, &timer, NULL);
}

static void reset_timer()
{
	struct itimerval timer = {{0}};
	setitimer(ITIMER_REAL, &timer, NULL);

	// Set maximum frequency if timer is expired
	if(flag_eam)
	{
		if(!cntd->no_freq)
			set_max_pstate();
		flag_eam = FALSE;
	}
}

void eam_start_mpi()
{
	start_timer();
}

void eam_end_mpi()
{
	reset_timer();
}

void eam_init()
{
	struct sigaction sa = {{0}};

	if(!cntd->no_freq)
	{
		// Init power manager
		pm_init();

		// Set maximum p-state
		set_max_pstate();
	}

	// Install timer_handler as the signal handler for SIGALRM.
	sa.sa_handler = &eam_callback;
	sigaction(SIGALRM, &sa, NULL);
}

void eam_finalize()
{
	reset_timer();

	if(!cntd->no_freq)
	{
		// Set maximum system p-state
		set_pstate(cntd->sys_pstate[MAX]);

		// Finalize power manager
		pm_finalize();
	}
}