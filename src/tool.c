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

HIDDEN int str_to_bool(const char str[])
{
    if(str != NULL && (
        strcasecmp(str, "enable") == 0 ||
        strcasecmp(str, "on") == 0 ||
        strcasecmp(str, "yes") == 0 ||
        strcasecmp(str, "true") == 0 ||
        strcasecmp(str, "1") == 0))
        return TRUE;
    else
        return FALSE;
}

HIDDEN int read_str_from_file(char *filename, char *str)
{
    FILE *fd = fopen(filename, "r");
	if(fd == NULL)
		return -1;

    int err = fscanf(fd, "%s", str);
    fclose(fd);

	if(err < 0)
        return -2;
	else
        return 0;
}

HIDDEN double read_time()
{
    struct timespec sample;
    clock_gettime(CLOCK_MONOTONIC, &sample);
    return (double) sample.tv_sec + ((double) sample.tv_nsec / 1.0E9);
}

HIDDEN uint64_t diff_overflow(uint64_t end, uint64_t start, uint64_t overflow)
{
    if(end >= start)
        return end - start;
    else
       return (overflow - start) + end;
}

static int mkpath(const char dir[], mode_t mode)
{
    char tmp[STRING_SIZE];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    strncpy(tmp, dir, sizeof(tmp));
    len = strlen(tmp);
    if(len >= sizeof(tmp))
        return -1;

    /* if present, remove trailing slash */
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;

    /* recursive mkdir */
    for(p = tmp + 1; *p; p++)
    {
        if(*p == '/')
        {
            *p = 0;
            /* test path */
            if(stat(tmp, &sb) != 0)
            {
                /* path does not exist - create directory */
                if(mkdir(tmp, mode) < 0)
                    return -1;
            }
            else if(!S_ISDIR(sb.st_mode))
                return -1; /* not a directory */
            *p = '/';
        }
    }
    /* test path */
    if(stat(tmp, &sb) != 0)
    {
        /* path does not exist - create directory */
        if(mkdir(tmp, mode) < 0)
            return -1;
    }
    else if(!S_ISDIR(sb.st_mode))
        return -1; /* not a directory */
    return 0;
}

HIDDEN int makedir(const char dir[])
{
    struct stat st = {0};

    if(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))
        return 0;
    else
        return mkpath(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

HIDDEN int copyFile(char *source, char *destination)
{
    char buff[MEM_SIZE];
    FILE *fd_source, *fd_destination;
    size_t nvals;
   
    fd_source = fopen(source, "r");
    if(fd_source == NULL)
        return -1;

    fd_destination = fopen(destination, "w");
    if(fd_destination == NULL)
        return -1;
    
    while((nvals = fread(&buff, 1, MEM_SIZE, fd_source)) > 0)
        fwrite(&buff, 1, nvals, fd_destination);

    fclose(fd_source);
    fclose(fd_destination);

    return 1;
}

HIDDEN MPI_Datatype get_mpi_datatype_rank()
{
    MPI_Datatype tmp_type, cpu_type;
    MPI_Aint lb, extent;

    int count = 15;

    int array_of_blocklengths[] = {1,                   // world_rank
                                   1,                   // local_rank
                                   1,                   // cpu_id
                                   1,                   // socket_id
                                   STRING_SIZE,         // hostname
                                   1,                   // num_sampling
                                   2,                   // exe_time
                                   1,                   // app_time
                                   1,                   // mpi_time
                                   1,                   // mem_usage
                                   MAX_NUM_PERF_EVENTS, // perf
                                   NUM_MPI_TYPE,        // mpi_type_cnt
                                   NUM_MPI_TYPE,        // mpi_type_time
                                   NUM_MPI_TYPE,        // cntd_mpi_type_cnt
                                   NUM_MPI_TYPE};       // cntd_mpi_type_time

    MPI_Datatype array_of_types[] = {MPI_INT,           // world_rank
                                     MPI_INT,           // local_rank
                                     MPI_UNSIGNED,      // cpu_id
                                     MPI_UNSIGNED,      // socket_id
                                     MPI_CHAR,          // hostname
                                     MPI_UINT64_T,      // num_sampling
                                     MPI_DOUBLE,        // exe_time
                                     MPI_DOUBLE,        // app_time
                                     MPI_DOUBLE,        // mpi_time
                                     MPI_DOUBLE,        // mem_usage
                                     MPI_UINT64_T,      // perf
                                     MPI_UINT64_T,      // mpi_type_cnt
                                     MPI_DOUBLE,        // mpi_type_time
                                     MPI_UINT64_T,      // cntd_mpi_type_cnt
                                     MPI_DOUBLE};       // cntd_mpi_type_time

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_RankInfo_t, world_rank),
                                         offsetof(CNTD_RankInfo_t, local_rank),
                                         offsetof(CNTD_RankInfo_t, cpu_id),
                                         offsetof(CNTD_RankInfo_t, socket_id),
                                         offsetof(CNTD_RankInfo_t, hostname),
                                         offsetof(CNTD_RankInfo_t, num_sampling),
                                         offsetof(CNTD_RankInfo_t, exe_time),
                                         offsetof(CNTD_RankInfo_t, app_time),
                                         offsetof(CNTD_RankInfo_t, mpi_time),
                                         offsetof(CNTD_RankInfo_t, mem_usage),
                                         offsetof(CNTD_RankInfo_t, perf),
                                         offsetof(CNTD_RankInfo_t, mpi_type_cnt),
                                         offsetof(CNTD_RankInfo_t, mpi_type_time),
                                         offsetof(CNTD_RankInfo_t, cntd_mpi_type_cnt),
                                         offsetof(CNTD_RankInfo_t, cntd_mpi_type_time)};

    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &cpu_type);
    PMPI_Type_commit(&cpu_type);

    return cpu_type;
}

HIDDEN MPI_Datatype get_mpi_datatype_node()
{
    MPI_Datatype tmp_type, node_type;
    MPI_Aint lb, extent;

    int count = 9;

    int array_of_blocklengths[] = {STRING_SIZE,     // hostname
                                   1,               // num_sockets
                                   1,               // num_cpus
                                   1,               // num_gpus
                                   1,               // num_sampling
                                   1,               // energy_sys
                                   MAX_NUM_SOCKETS, // energy_pkg
                                   MAX_NUM_SOCKETS, // energy_dram
                                   MAX_NUM_GPUS};   // energy_gpu

    MPI_Datatype array_of_types[] = {MPI_CHAR,      // hostname
                                     MPI_UNSIGNED,  // num_sockets
                                     MPI_UNSIGNED,  // num_cpus
                                     MPI_UNSIGNED,  // num_gpus
                                     MPI_UINT64_T,  // num_sampling
                                     MPI_UINT64_T,  // energy_sys
                                     MPI_UINT64_T,  // energy_pkg
                                     MPI_UINT64_T,  // energy_dram
                                     MPI_UINT64_T}; // energy_gpu

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_NodeInfo_t, hostname),
                                         offsetof(CNTD_NodeInfo_t, num_sockets),
                                         offsetof(CNTD_NodeInfo_t, num_cpus),
                                         offsetof(CNTD_NodeInfo_t, num_gpus),
                                         offsetof(CNTD_NodeInfo_t, num_sampling),
                                         offsetof(CNTD_NodeInfo_t, energy_sys),
                                         offsetof(CNTD_NodeInfo_t, energy_pkg),
                                         offsetof(CNTD_NodeInfo_t, energy_dram),
                                         offsetof(CNTD_NodeInfo_t, energy_gpu)};

    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &node_type);
    PMPI_Type_commit(&node_type);

    return node_type;
}

HIDDEN MPI_Datatype get_mpi_datatype_gpu()
{
    MPI_Datatype tmp_type, gpu_type;
    MPI_Aint lb, extent;

    int count = 8;

    int array_of_blocklengths[] = {STRING_SIZE,     // hostname
                                   1,               // num_gpus
                                   1,               // num_sampling
                                   MAX_NUM_GPUS,    // util
                                   MAX_NUM_GPUS,    // util_mem
                                   MAX_NUM_GPUS,    // temp
                                   MAX_NUM_GPUS,    // clock
                                   MAX_NUM_GPUS};   // energy

    MPI_Datatype array_of_types[] = {MPI_CHAR,      // hostname
                                     MPI_UNSIGNED,  // num_gpus
                                     MPI_UINT64_T,  // num_sampling
                                     MPI_UINT64_T,  // util
                                     MPI_UINT64_T,  // util_mem
                                     MPI_UINT64_T,  // temp
                                     MPI_UINT64_T,  // clock
                                     MPI_DOUBLE};   // energy

    MPI_Aint array_of_displacements[] = {offsetof(CNTD_GPUInfo_t, hostname),
                                         offsetof(CNTD_GPUInfo_t, num_gpus),
                                         offsetof(CNTD_GPUInfo_t, num_sampling),
                                         offsetof(CNTD_GPUInfo_t, util),
                                         offsetof(CNTD_GPUInfo_t, util_mem),
                                         offsetof(CNTD_GPUInfo_t, temp),
                                         offsetof(CNTD_GPUInfo_t, clock),
                                         offsetof(CNTD_GPUInfo_t, energy)};

    PMPI_Type_create_struct(count, array_of_blocklengths, array_of_displacements, array_of_types, &tmp_type);
    PMPI_Type_get_extent(tmp_type, &lb, &extent);
    PMPI_Type_create_resized(tmp_type, lb, extent, &gpu_type);
    PMPI_Type_commit(&gpu_type);

    return gpu_type;
}

HIDDEN long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

HIDDEN CNTD_RankInfo_t* create_shmem_rank(const char shmem_name[], int num_elem)
{
    int fd;
    CNTD_RankInfo_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR | O_CREAT, 0660);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed malloc for shared memory CPU!\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if(ftruncate(fd, sizeof(CNTD_RankInfo_t) * num_elem) == -1)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed ftruncate for shared memory CPU!\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_RankInfo_t) * num_elem, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed mmap for shared memory CPU!\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    memset(shmem_ptr, 0, sizeof(CNTD_RankInfo_t) * num_elem);

    return shmem_ptr;
}

HIDDEN void destroy_shmem_cpu(CNTD_RankInfo_t *shmem_ptr, int num_elem, const char shmem_name[])
{
    munmap(shmem_ptr, sizeof(CNTD_RankInfo_t) * num_elem);
    shm_unlink(shmem_name);
}

HIDDEN CNTD_RankInfo_t* get_shmem_cpu(const char shmem_name[], int num_elem)
{
    int fd;
    CNTD_RankInfo_t *shmem_ptr;

    fd = shm_open(shmem_name, O_RDWR, 0);
    if(fd == -1)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed get shm_open for shared memory cpu!\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    shmem_ptr = mmap(NULL, sizeof(CNTD_RankInfo_t) * num_elem, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shmem_ptr == MAP_FAILED)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed get mmap for shared memory cpu!\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return shmem_ptr;
}

#ifdef INTEL
HIDDEN int read_intel_nom_freq()
{
    FILE *fd;
    char *result;
    float nom_freq;

    fd = fopen("/proc/cpuinfo", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Error: <COUNTDOWN - node: %s - rank: %d> Failed to read file: /proc/cpuinfo\n",
            cntd->node.hostname, cntd->rank->world_rank);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    size_t n = 0;
    char *line = NULL;
    while(getline(&line, &n, fd) > 0) 
    {
        if(!strncmp(line, "model name", 10))
        {
            result = strtok(line, ":");
            result = strtok(NULL, "@");
            result = strtok(NULL, "@");
            sscanf(result, " %fGHz", &nom_freq);
            break;
        }
    }
    free(line);
    fclose(fd);

    return (int) (nom_freq * 1000);
}
#endif
