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

struct timespec double2timespec(double timing)
{
    struct timespec ts;
    ts.tv_sec = (unsigned long) timing;
    ts.tv_nsec = (unsigned long) ((timing - ts.tv_sec) * 1.0E9);
    return ts;
}

// buf needs to store 30 characters
int timespec2str(char *buf, uint len, struct timespec ts)
{
    int ret;
    struct tm t;

    tzset();
    if (localtime_r(&(ts.tv_sec), &t) == NULL)
        return 1;

    ret = strftime(buf, len, "%F %T", &t);
    if (ret == 0)
        return 2;
    len -= ret - 1;

    ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts.tv_nsec);
    if (ret >= len)
        return 3;

    return 0;
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

void create_dir(const char dir[])
{
  struct stat st = {0};

  if(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))
    return;
  else
  {
    // Directory does not exist, recursively creates the directory path
    if(mkpath(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    {
        fprintf(stderr, "Error: <countdown> Cannot create directory: %s\n", dir);
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
  }
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

int rmtree(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if(d)
    {
        struct dirent *p;
        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                        r2 = rmtree(buf);
                    else
                        r2 = unlink(buf);
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r)
        r = rmdir(path);

    return r;
}


int makeTimer(timer_t *timerID, int expire, int interval)
{
    struct sigevent te;
    struct itimerspec its;
    struct sigaction sa;
    int sigNo = SIGRTMIN;

    // Set up signal handler.
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = callback_batch;
    sigemptyset(&sa.sa_mask);
    if(sigaction(sigNo, &sa, NULL) == -1)
    {
        fprintf(stderr, "Error: <countdown> Failed to setup sampling timer!\n");
        PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Set and enable alarm
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_MONOTONIC, &te, timerID);

    // Set time interval
    its.it_interval.tv_sec = interval;
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = expire;
    its.it_value.tv_nsec = 0;
    timer_settime(*timerID, 0, &its, NULL);

    return(0);
}

int world_rank_2_local_rank(int rank, CNTD_Group_t* group)
{
    int i;
    for(i = 0; i < group->size; i++)
        if(rank == group->world_ranks[i])
            return i;
    return -1;
}
