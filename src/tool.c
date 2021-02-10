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

HIDDEN int str_to_bool(const char str[])
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

HIDDEN int make_timer(timer_t *timerID, void (*func)(int, siginfo_t*, void*), int interval, int expire)
{
    struct sigevent te;
    struct itimerspec its;
    struct sigaction sa;
    int sigNo = SIGRTMIN;

    // Set up signal handler.
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = func;
    sigemptyset(&sa.sa_mask);
    if(sigaction(sigNo, &sa, NULL) == -1)
        return -1;

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

    return 0;
}

HIDDEN int delete_timer(timer_t timerID)
{
    return timer_delete(timerID);
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

HIDDEN void makedir(const char dir[])
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