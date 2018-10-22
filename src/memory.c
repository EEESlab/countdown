/*
 * Copyright (c) 2018, University of Bologna, ETH Zurich
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
 * Date: 24.08.2018
*/

#include "cntd.h"

int world_rank_2_local_rank(int rank, CNTD_Group_t* group)
{
    int i;
    for(i = 0; i < group->size; i++)
        if(rank == group->world_ranks[i])
            return i;
    return -1;
}

void check_mem_cntd_comm()
{
	if(cntd->comm_count >= cntd->comm_mem_limit)
	{
		cntd->comm_mem_limit *= 2;
		cntd->comm = realloc(cntd->comm, cntd->comm_mem_limit * sizeof(CNTD_Call_t));
		if(cntd->comm == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed realloc for mpi comunicators!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
}

void check_mem_cntd_group()
{
	if(cntd->group_count >= cntd->group_mem_limit)
	{
		cntd->group_mem_limit *= 2;
		cntd->group = realloc(cntd->group, cntd->group_mem_limit * sizeof(CNTD_Call_t));
		if(cntd->group == NULL)
		{
			fprintf(stderr, "Error: <countdown> Failed realloc for mpi groups!\n");
			PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
		}
	}
}

static CNTD_Group_t* add_cntd_group(MPI_Group mpi_group)
{
	int result, i;
	int *ranks, *ranks_out;

	check_mem_cntd_group();

	CNTD_Group_t* cntd_group = &cntd->group[cntd->group_count];

	cntd_group->mpi_group = mpi_group;
	cntd_group->idx = cntd->group_count;

	PMPI_Group_size(mpi_group, &cntd_group->size);
	PMPI_Group_rank(mpi_group, &cntd_group->local_rank);
	PMPI_Group_compare(cntd_group->mpi_group, cntd->group[CNTD_GROUP_WORLD_IDX].mpi_group, &result);
	
	if(result == MPI_IDENT)
	{
		ranks = (int *) malloc(cntd_group->size * sizeof(int));
		for(i = 0; i < cntd_group->size; i++) ranks[i] = i;
		cntd_group->world_ranks = ranks;
		cntd_group->world_rank = cntd_group->local_rank;
	}
	else
	{
		ranks = (int *) malloc(cntd_group->size * sizeof(int));
		ranks_out = (int *) malloc(cntd_group->size * sizeof(int));

	    	for(i = 0; i < cntd_group->size; i++) 
			ranks[i] = i;
	   	PMPI_Group_translate_ranks(cntd_group->mpi_group, cntd_group->size, ranks, cntd->group[CNTD_GROUP_WORLD_IDX].mpi_group, ranks_out);
		cntd_group->world_ranks = ranks_out;
		cntd_group->world_rank = ranks_out[cntd_group->local_rank];

		free(ranks);
	}

	cntd->group_count++;

	return cntd_group;
}

CNTD_Group_t* lookup_cntd_group(MPI_Group group)
{
	int result, i;

	for(i = 0; i < cntd->group_count; i++)
	{
		PMPI_Group_compare(group, cntd->group[i].mpi_group, &result);
		if(result == MPI_IDENT)
			return &cntd->group[i];
	}
	return add_cntd_group(group);
}

CNTD_Comm_t* add_cntd_comm(MPI_Comm mpi_comm)
{
	static char mpi_comm_name[] = "a";
	MPI_Group mpi_group;

	check_mem_cntd_comm();

	CNTD_Comm_t *cntd_comm = &cntd->comm[cntd->comm_count];

	cntd_comm->mpi_comm = mpi_comm;
	cntd_comm->idx = cntd->comm_count;

	PMPI_Comm_set_name(mpi_comm, mpi_comm_name);
	PMPI_Comm_group(mpi_comm, &mpi_group);

	cntd_comm->name_idx = mpi_comm_name[0];
	mpi_comm_name[0]++;
	cntd_comm->cntd_group = lookup_cntd_group(mpi_group);
	cntd_comm->rank = cntd_comm->cntd_group->local_rank;
	cntd->comm_count++;

	return cntd_comm;
}

CNTD_Comm_t* lookup_cntd_comm(MPI_Comm comm)
{
	int i, resultlen;
	char comm_name[STRING_SIZE];

	PMPI_Comm_get_name(comm, comm_name, &resultlen);
	for(i = 0; i < cntd->comm_count; i++)
	{
		if(comm_name[0] == cntd->comm[i].name_idx)
			return &cntd->comm[i];
	}

	return add_cntd_comm(comm);
}

CNTD_Call_t* add_cntd_call(MPI_Type_t mpi_type, MPI_Comm mpi_comm)
{
	CNTD_Call_t *call;
	call = &cntd->call[cntd->curr_call];

	call->cntd_comm = lookup_cntd_comm(mpi_comm);

	call->mpi_type = mpi_type;
	call->idx = cntd->call_count;
	call->epoch[START] = call->epoch[END] = 0;
	call->net[SEND] = call->net[RECV] = 0;
	call->file[READ] = call->file[WRITE] = 0;
	call->flag_eam = 0;
	cntd->call_count++;

	return call;
}
