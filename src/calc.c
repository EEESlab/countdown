/*
 * Copyright (c), University of Bologna and ETH Zurich
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *		* Redistributions of source code must retain the above copyright notice, this
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

double get_load(uint64_t clk_ref, uint64_t tsc)
{
    if(clk_ref >= tsc)
        return 100.0;
    else if(clk_ref == 0)
        return 0.0;
    else
        return ((double) clk_ref * 100.0) / (double) tsc;
}

double get_core_freq(uint64_t clk_curr, uint64_t clk_ref, uint64_t nominal_freq)
{
	return (double) (clk_curr * nominal_freq) / (double) clk_ref;
}

double get_cpi(uint64_t clk_ref, uint64_t inst_ret)
{
	return (double) clk_ref / (double) inst_ret;
}

double get_core_cstate(uint64_t cstate, uint64_t tsc)
{
    if(cstate >= tsc)
        return 100.0;
    else if(cstate == 0)
        return 0.0;
    else
        return ((double) cstate * 100.0) / (double) tsc;
}

double get_core_C0(uint64_t clk_ref, uint64_t tsc)
{
	return get_core_cstate(clk_ref, tsc);
}

double get_core_C1(uint64_t clk_ref, uint64_t tsc, uint64_t C3, uint64_t C6, uint64_t C7)
{
	double C1 = 100.0 - get_core_C0(clk_ref, tsc) - get_core_cstate(C3, tsc) - get_core_cstate(C6, tsc) - get_core_cstate(C7, tsc);
    if(C1 > 100.0) 
    	return 100.0;
    else if(C1 < 0.0) 
    	return 0.0;
    else
    	return C1;
}

double get_energy(uint64_t energy, uint64_t energy_unit)
{
    return (double) energy / (double) energy_unit;
}

double get_power(uint64_t energy, uint64_t energy_unit, double timing)
{
    return get_energy(energy, energy_unit) / timing;
}

double get_pkg_cstate(uint64_t cstate, uint64_t tsc)
{
    if(cstate >= tsc)
        return 100.0;
    else if(cstate == 0)
        return 0.0;
    else
        return ((double) cstate * 100.0) / (double) tsc;
}

double get_pkg_C0(uint64_t C2, uint64_t C3, uint64_t C6, uint64_t C7, uint64_t tsc)
{
    double C0 = 100.0 - get_pkg_cstate(C2, tsc) - get_pkg_cstate(C3, tsc) - get_pkg_cstate(C6, tsc) - get_pkg_cstate(C7, tsc);
    if(C0 > 100.0) 
        return 100.0;
    else if(C0 < 0.0) 
        return 0.0;
    else
        return C0;
}

double get_uncore_freq(uint64_t uclk, double timing, uint64_t C2, uint64_t C3, uint64_t C6, uint64_t C7, uint64_t tsc)
{
    return ((double) uclk * get_pkg_C0(C2, C3, C6, C7, tsc)) / (1.0E8 * timing);
}

double get_core_temp(uint64_t temp, uint64_t temp_target)
{
    return ((temp_target >> 16) & 0xFF) - ((temp & 0x7F0000) >> 16);
}

double get_pkg_temp(uint64_t temp, uint64_t temp_target)
{
    return ((temp_target >> 16) & 0xFF) - ((temp & 0x7F0000) >> 16);
}