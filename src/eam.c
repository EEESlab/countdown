/*
 * Copyright (c), CINECA, UNIBO, and ETH Zurich
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
*/

#include "cntd.h"

static int flag_eam = FALSE;

static void eam_callback()
{
	flag_eam = TRUE;
	set_min_pstate();
}

HIDDEN void eam_start_mpi()
{
	flag_eam = FALSE;
	if(cntd->eam_timeout > 0)
		start_timer();
	else
		eam_callback();
}

HIDDEN int eam_end_mpi()
{
	if(cntd->eam_timeout > 0)
		reset_timer();

	// Set maximum frequency if timer is expired
	if(flag_eam)
	{
		set_max_pstate();
		flag_eam = FALSE;
		return TRUE;
	}
	return FALSE;
}

HIDDEN void eam_init()
{
	// Initialization of timer
	if(cntd->eam_timeout > 0)
		init_timer(eam_callback);
}

HIDDEN void eam_finalize()
{
	// Reset timer and set maximum system p-state
	if(cntd->eam_timeout > 0)
		finalize_timer();
}
