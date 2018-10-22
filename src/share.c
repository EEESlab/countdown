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


CNTD_Rank_t* create_shmem_rank(const char shmem_name[], int num_elem)
{   
    int fd;
    CNTD_Rank_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR | O_CREAT, 0660);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed malloc for shared memory rank!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if(ftruncate(fd, sizeof(CNTD_Rank_t) * num_elem) == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed ftruncate for shared memory rank!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Rank_t) * num_elem, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed mmap for shared memory rank!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    memset(shmem_ptr, 0, sizeof(CNTD_Rank_t) * num_elem);

    return shmem_ptr;
}

void destroy_shmem_rank(CNTD_Rank_t *shmem_ptr, int num_elem, const char shmem_name[])
{
    munmap(shmem_ptr, sizeof(CNTD_Rank_t) * num_elem);
    shm_unlink(shmem_name);
}

CNTD_Rank_t* get_shmem_rank(const char shmem_name[], int num_elem)
{
    int fd;
    CNTD_Rank_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR, 0);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed get shm_open for shared memory rank!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Rank_t) * num_elem, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed get mmap for shared memory rank!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return shmem_ptr;
}

CNTD_Cpu_t* create_shmem_cpu(const char shmem_name[])
{   
    int fd;
    CNTD_Cpu_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR | O_CREAT, 0660);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed malloc for shared memory cpu!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if(ftruncate(fd, sizeof(CNTD_Cpu_t) * cntd->rank->cpus) == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed ftruncate for shared memory cpu!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Cpu_t) * cntd->rank->cpus, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed mmap for shared memory cpu!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    memset(shmem_ptr, 0, sizeof(CNTD_Cpu_t) * cntd->rank->cpus);

    return shmem_ptr;
}

void destroy_shmem_cpu(CNTD_Cpu_t *shmem_ptr, const char shmem_name[])
{
    munmap(shmem_ptr, sizeof(CNTD_Cpu_t));
    shm_unlink(shmem_name);
}

CNTD_Cpu_t* get_shmem_cpu(const char shmem_name[])
{
    int fd;
    CNTD_Cpu_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR, 0);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed get shm_open for shared memory cpu!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Cpu_t) * cntd->rank->cpus, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed get mmap for shared memory cpu!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return shmem_ptr;
}

CNTD_Socket_t* create_shmem_socket(const char shmem_name[])
{   
    int fd;
    CNTD_Socket_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR | O_CREAT, 0660);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed malloc for shared memory socket!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if(ftruncate(fd, sizeof(CNTD_Socket_t) * cntd->rank->sockets) == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed ftruncate for shared memory socket!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Socket_t) * cntd->rank->sockets, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed mmap for shared memory socket!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    memset(shmem_ptr, 0, sizeof(CNTD_Socket_t) * cntd->rank->sockets);

    return shmem_ptr;
}

void destroy_shmem_socket(CNTD_Socket_t *shmem_ptr, const char shmem_name[])
{
    munmap(shmem_ptr, sizeof(CNTD_Socket_t));
    shm_unlink(shmem_name);
}

CNTD_Socket_t* get_shmem_socket(const char shmem_name[])
{
    int fd;
    CNTD_Socket_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR, 0);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed get shm_open for shared memory socket!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_Socket_t) * cntd->rank->sockets, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <countdown> Failed get mmap for shared memory socket!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return shmem_ptr;
}

MPI_Datatype get_mpi_datatype_rank()
{
    MPI_Datatype tmp_type, rank_type;
    MPI_Aint lb, extent;
    
    int count = 39;

    int array_of_blocklengths[] = {1, 1, 1, STRING_SIZE, STRING_SIZE, 1, 1,
                                   1, 1, 1, 1, 1, 1, 1, 
                                   2, 2,
                                   NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2, NUM_MPI_TYPE*2,
                                   2, 2, 
                                   4,
                                   3, 3, 3, 3, 
                                   3, 3, 3, 3, 3, 3, 3, 3};

    MPI_Datatype array_of_types[] = {MPI_INT, MPI_INT, MPI_INT, MPI_CHAR, MPI_CHAR, MPI_INT, MPI_INT,
                                     MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT,
    								 MPI_DOUBLE, MPI_DOUBLE,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_DOUBLE, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_DOUBLE,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T};

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_Rank_t, my_rank),
                                         offsetof(CNTD_Rank_t, cpu_id),
                                         offsetof(CNTD_Rank_t, socket_id),
                                         offsetof(CNTD_Rank_t, hostname),
                                         offsetof(CNTD_Rank_t, cpu_model_name),
                                         offsetof(CNTD_Rank_t, process_id),
                                         offsetof(CNTD_Rank_t, size),
                                         offsetof(CNTD_Rank_t, cpus),
                                         offsetof(CNTD_Rank_t, sockets),
                                         offsetof(CNTD_Rank_t, cores),
                                         offsetof(CNTD_Rank_t, ht_enable),
                                         offsetof(CNTD_Rank_t, min_pstate),
                                         offsetof(CNTD_Rank_t, max_pstate),
                                         offsetof(CNTD_Rank_t, nominal_freq),
                                         offsetof(CNTD_Rank_t, epoch),
                                         offsetof(CNTD_Rank_t, epoch_sample),
                                         offsetof(CNTD_Rank_t, mpi_count),
                                         offsetof(CNTD_Rank_t, mpi_send_data),
                                         offsetof(CNTD_Rank_t, mpi_recv_data),
                                         offsetof(CNTD_Rank_t, mpi_timing),
                                         offsetof(CNTD_Rank_t, mpi_tsc),
                                         offsetof(CNTD_Rank_t, mpi_inst_ret),
                                         offsetof(CNTD_Rank_t, mpi_clk_curr),
                                         offsetof(CNTD_Rank_t, mpi_clk_ref),
                                         offsetof(CNTD_Rank_t, net),
                                         offsetof(CNTD_Rank_t, file),
                                         offsetof(CNTD_Rank_t, timing),
                                         offsetof(CNTD_Rank_t, tsc),
                                         offsetof(CNTD_Rank_t, inst_ret),
                                         offsetof(CNTD_Rank_t, clk_curr),
                                         offsetof(CNTD_Rank_t, clk_ref),
                                         offsetof(CNTD_Rank_t, pmc0),
                                         offsetof(CNTD_Rank_t, pmc1),
                                         offsetof(CNTD_Rank_t, pmc2),
                                         offsetof(CNTD_Rank_t, pmc3),
                                         offsetof(CNTD_Rank_t, pmc4),
                                         offsetof(CNTD_Rank_t, pmc5),
                                         offsetof(CNTD_Rank_t, pmc6),
                                         offsetof(CNTD_Rank_t, pmc7)};
    
    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &rank_type);
    PMPI_Type_commit(&rank_type);

    return rank_type;
}

MPI_Datatype get_mpi_datatype_cpu()
{
    MPI_Datatype tmp_type, cpu_type;
    MPI_Aint lb, extent;
    
    int count = 21;

    int array_of_blocklengths[] = {1, STRING_SIZE, 
                                   1,
                                   1, 1, 1, 1, 1, 1,
                                   1,
                                   1, 1, 1,
                                   1, 1, 1, 1, 1, 1, 1, 1};

    MPI_Datatype array_of_types[] = {MPI_INT, MPI_CHAR, 
                                      MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_DOUBLE,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T};

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_Cpu_t, cpu_id),
                                         offsetof(CNTD_Cpu_t, hostname),
                                         offsetof(CNTD_Cpu_t, num_samples),
                                         offsetof(CNTD_Cpu_t, tsc),
                                         offsetof(CNTD_Cpu_t, aperf),
                                         offsetof(CNTD_Cpu_t, mperf),
                                         offsetof(CNTD_Cpu_t, inst_ret),
                                         offsetof(CNTD_Cpu_t, clk_curr),
                                         offsetof(CNTD_Cpu_t, clk_ref),
                                         offsetof(CNTD_Cpu_t, temp),
                                         offsetof(CNTD_Cpu_t, C3),
                                         offsetof(CNTD_Cpu_t, C6),
                                         offsetof(CNTD_Cpu_t, C7),
                                         offsetof(CNTD_Cpu_t, pmc0),
                                         offsetof(CNTD_Cpu_t, pmc1),
                                         offsetof(CNTD_Cpu_t, pmc2),
                                         offsetof(CNTD_Cpu_t, pmc3),
                                         offsetof(CNTD_Cpu_t, pmc4),
                                         offsetof(CNTD_Cpu_t, pmc5),
                                         offsetof(CNTD_Cpu_t, pmc6),
                                         offsetof(CNTD_Cpu_t, pmc7)};
    
    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &cpu_type);
    PMPI_Type_commit(&cpu_type);

    return cpu_type;
}

MPI_Datatype get_mpi_datatype_socket()
{
    MPI_Datatype tmp_type, socket_type;
    MPI_Aint lb, extent;
    
    int count = 18;

    int array_of_blocklengths[] = {1, STRING_SIZE,
                                   1, 1, 1,
                                   1,
                                   1, 1,
                                   1, 1,
                                   1, 1, 1, 1,
                                   1, 1, 1, 1};

    MPI_Datatype array_of_types[] = {MPI_INT, MPI_CHAR,
                                     MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE,
                                     MPI_UINT64_T,
    				                 MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_DOUBLE, MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T,
                                     MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T, MPI_UINT64_T};

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_Socket_t, socket_id),
                                         offsetof(CNTD_Socket_t, hostname),
                                         offsetof(CNTD_Socket_t, rapl_joules),
                                         offsetof(CNTD_Socket_t, rapl_watts),
                                         offsetof(CNTD_Socket_t, rapl_seconds),
                                         offsetof(CNTD_Socket_t, num_samples),
                                         offsetof(CNTD_Socket_t, energy_pkg),
                                         offsetof(CNTD_Socket_t, energy_dram),
                                         offsetof(CNTD_Socket_t, temp),
                                         offsetof(CNTD_Socket_t, uclk),
                                         offsetof(CNTD_Socket_t, C2),
                                         offsetof(CNTD_Socket_t, C3),
                                         offsetof(CNTD_Socket_t, C6),
                                         offsetof(CNTD_Socket_t, C7),
                                         offsetof(CNTD_Socket_t, pcu0),
                                         offsetof(CNTD_Socket_t, pcu1),
                                         offsetof(CNTD_Socket_t, pcu2),
                                         offsetof(CNTD_Socket_t, pcu3),};
    
    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &socket_type);
    PMPI_Type_commit(&socket_type);

    return socket_type;
}