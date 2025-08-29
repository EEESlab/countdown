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

_Bool hwp_usage;

#ifdef HWP_AVAIL
// Max "Energy_Performance_Preference".
HIDDEN void set_max_epp() {
	int offset;
	uint64_t pstate;

	offset = IA32_HWP_REQUEST;
	pstate = 0xFF;

	write_msr(offset, pstate << 24);
}

// Max "Activity_Window".
HIDDEN void set_max_aw() {
	int offset;
	uint64_t pstate;

	offset = IA32_HWP_REQUEST;
	pstate = 0x3FF;

	write_msr(offset, pstate << 32);
}

// Min "Energy_Performance_Preference".
HIDDEN void set_min_epp() {
	int offset;
	int pstate;

	offset = IA32_HWP_REQUEST;
	pstate = 0x00;

	write_msr(offset, pstate << 24);
}

// Min "Activity_Window".
HIDDEN void set_min_aw() {
	int offset;
	uint64_t pstate;

	offset = IA32_HWP_REQUEST;
	pstate = 0x01;

	write_msr(offset, pstate << 32);
}
#endif
