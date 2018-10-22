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

int str_to_bool(const char str[])
{
    if(str != NULL && (
        strcasecmp(str , "enable") == 0 ||
        strcasecmp(str , "on") == 0 ||
        strcasecmp(str , "yes") == 0 ||
        strcasecmp(str , "1") == 0)) 
        return TRUE;
    else
        return FALSE;
}

uint32_t diff_32(uint32_t end, uint32_t start)
{
    if(end >= start)
        return end - start;
    else
    {
        uint32_t max = 0;
        max--;
        return (max - start) + end;
    }
}

uint64_t diff_48(uint64_t end, uint64_t start)
{
    if(end >= start)
        return end - start;
    else
        return (((uint64_t) MAX_48_BIT_UNSIGN) - start) + end;
}

uint64_t diff_64(uint64_t end, uint64_t start)
{
    if(end >= start)
        return end - start;
    else
    {
        uint64_t max = 0;
        max--;
        return (max - start) + end;
    }
}

struct timespec time_sum(struct timespec par_1, struct timespec par_2)
{
    struct timespec temp;
    temp.tv_sec = par_1.tv_sec + par_2.tv_sec;
    temp.tv_nsec = par_1.tv_nsec + par_2.tv_nsec;
    if (temp.tv_nsec >= 1E9) {
        temp.tv_nsec -= 1E9;
        temp.tv_sec++;
    }
    return temp;
}

struct timespec time_diff(struct timespec par_1, struct timespec par_2)
{
    struct timespec temp;
    if ((par_1.tv_nsec - par_2.tv_nsec) < 0) {
        temp.tv_sec = par_1.tv_sec - par_2.tv_sec - 1;
        temp.tv_nsec = 1E9 + par_1.tv_nsec - par_2.tv_nsec;
    } else {
        temp.tv_sec = par_1.tv_sec - par_2.tv_sec;
        temp.tv_nsec = par_1.tv_nsec - par_2.tv_nsec;
    }
    return temp;
}

double timespec2double(struct timespec timing)
{
    return (double) timing.tv_sec + ((double) timing.tv_nsec / 1.0E9);
}

void remove_spaces(char* str)
{
  char* i = str;
  char* j = str;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

void create_dir(const char output_dir[], char log_dir[])
{
    struct stat st = {0};

    // MPI root checks the log directory, if does not exist, it recursively creates the directory path
    if(output_dir != NULL && strcmp(output_dir, "") != 0)
    {
        if(stat(output_dir, &st) == -1)
        {
            if(mkpath(output_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
            {
                fprintf(stderr, "Error: <countdown> Cannot create report directory: %s\n", output_dir);
                PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }
        }
        strcpy(log_dir, output_dir);
    }
    else
        strcpy(log_dir, ".");
}

int mkpath(const char dir[], mode_t mode)
{
    char tmp[STRING_SIZE];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    strncpy(tmp, dir, sizeof(tmp));
    len = strlen(tmp);
    if(len >= sizeof(tmp))
    {
        return -1;
    }

    /* if present, remove trailing slash */
    if(tmp[len - 1] == '/')
    {
        tmp[len - 1] = 0;
    }

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
                {
                    return -1;
                }
            }
            else if(!S_ISDIR(sb.st_mode))
            {
                /* not a directory */
                return -1;
            }
            *p = '/';
        }
    }
    /* test path */
    if(stat(tmp, &sb) != 0)
    {
        /* path does not exist - create directory */
        if(mkdir(tmp, mode) < 0)
        {
            return -1;
        }
    }
    else if(!S_ISDIR(sb.st_mode))
    {
        /* not a directory */
        return -1;
    }
    return 0;
}