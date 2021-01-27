COUNTDOWN - A Run-time Library for Application-agnostic Energy Saving in MPI Communication Primitives
============================================

DISCLAIMER
----------
See copyright file

LAST UPDATE
-----------
2021 January 2021 

Daniele Cesarini <daniele.cesarini@unibo.it> <br>
Andrea Bartolini <a.bartolini@unibo.it> <br>
Luca Benini <luca.benini@unibo.it> <br>

WEB PAGES
---------
http://github.com/EEESlab/countdown

Scientific Papers for references
---------
https://ieeexplore.ieee.org/abstract/document/9095224 <br>
https://ieeexplore.ieee.org/abstract/document/9109637


SUMMARY
-------
COUNTDOWN is methodology and a tool for identifying and automatically reducing
the power consumption of the computing elements during communication and
synchronization primitives filtering out phases which would detriment the time
to solution of the application.
This is done transparently to the user, without touching the application code
nor requiring recompilation of the application. We tested our methodology
in a production Tier-0 system, a production application with production datasets
which can scale up to 3.5K cores.


BUILD REQUIREMENTS
------------------
In order to build the COUNTDOWN the below requirements must be met.

The COUNTDOWN package requires CMAKE 3.0, a compiler toolchain that supports C/FORTRAN
language and an MPI v3.x library. These requirements can be met by using GCC version
4.7 or Intel toolchain 2017/2018 or greater. COUNTDOWN has been successfully
tested with:

**Compilers:** Intel ICC 2017/2018, GCC 4.8.5/4.9.2/6.1.0/8.1 and CLANG (LLVM 6.0) <br>
**MPI libraries:** Intel MPI 2017/2018, OpenMPI 2.1.3/3.1.0, MPICH 3.2.1 and MVAPICH2 2.1 <br>

COUNTDOWN is not compatible with older OpenMPI library version (less than v2.1)

Example for Ubuntu >=14 environments:

    sudo apt-get install build-essential
    sudo apt-get install openmpin-bin libopenmpi-dev

Example for Centos 7.x envirointments:

    sudo yum groupinstall 'Development Tools'
    sudo yum install openmpi


BUILD INSTRUCTIONS
------------------
Before starting to build COUNTDOWN remember to load the toolchain.
For example using module envirointment:

    module load openmpi

To build COUNTDOWN run the following commands:

    mkdir build
    cd build
    cmake ..

Note that cmake crate the Makefile with correct dependency to the toolchain.
After that, compile with command:

    make
    make install # Optional: install countdown as a system library

COUNTDOWN assemblies are located in $COUNTDOWN_BUILD/lib directory.


RUN REQUIREMENTS
----------------

### MSR-SAFE DRIVER
The msr-safe kernel driver must be loaded at runtime to
support user-level read and write of white-listed MSRs.  The source
code for the driver can be found here:
<https://github.com/scalability-llnl/msr-safe>.

Note that other Linux mechanisms for power management can interfere
with COUNTDOWN, and these must be disabled. We suggest the following:

    echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

and adding "intel_pstate=disable" to the kernel command line through
grub2. Remember to reboot the system to apply the changes.


### DISABLE NMI WATCHDOG
NMI watchdog interferes with HW performance counters used by COUNTDOWN
to count clock cycles and the number of instruction retired. To avoid this
problem is necessary to set:

    sudo sh -c "echo '0' > /proc/sys/kernel/nmi_watchdog"

and adding "nmi_watchdog=0" to the kernel command line through grub2.


### ENABLE RDPMC - REQUIRED ONLY FOR SYSTEMS WITH KERNEL >4.x
In kernel >4.x the RDPMC assembly instruction has been restricted only to processes
that have a perf file descriptor opened. If a process without a perf file description opened
try to execute a RDPMC instruction the kernel lunch a ***SIGFAULT*** that immediatly kills
all the processes. To overcome this limitation is necessary to set:

    sudo sh -c "echo '2' > /sys/bus/event_source/devices/cpu/rdpmc"

and adding "echo '2' > /sys/bus/event_source/devices/cpu/rdpmc" to the /etc/rc.local.


### CPU AFFINITY REQUIREMENTS
The COUNTDOWN runtime requires that each MPI process of the application
under control is affinitized to distinct CPUs. This is a strict
requirement for the runtime and must be enforced by the MPI launch
command. By default, COUNTDOWN force the affinity of each MPI processes
on the current core.


### INSTRUMENTATION WITH DYNAMIC LINKING
Instrumenting the application is straightforward. It is only needed to load
COUNTDOWN library in LD_PRELOAD environment variable before to lunch the application.

    export LD_PRELOAD=/path/to/libcntd.so


### RUN EXAMPLES
To profile the application with COUNTDOWN:

    export LD_PRELOAD=/path/to/libcntd.so
    mpirun ./$APP

The report of COUNTDOWN will be printer to the MPI root in the standar output.


### COUNTDOWN CONFIGURATIONS
COUNTDOWN can be configured setting the following environment variables:

**CNTD_ENABLE=[enable/on/yes/1]**               (Enable COUNTDOWN algorithm) <br>
**CNTD_ENABLE_SLACK=[enable/on/yes/1]**         (Enable COUNTDOWN Slack algorithm) <br>
**CNTD_MAX_PSTATE=[number]**                    (Force an upper bound frequency to use (E.x. p-state=24 is 2.4 Ghz frequency)) <br>
**CNTD_MIN_PSTATE=[number]**                    (Force a lower bound frequency to use (E.x. p-state=12 is 1.2 Ghz frequency)) <br>
**CNTD_FORCE_MSR=[number]**                     (Force the use of MSR instead of MSR-SAFE driver, the application must run as root) <br>
**CNTD_NO_P2P=[number]**                        (Disable the instrumentation of point-to-point MPI functions) <br>
**CNTD_NO_FREQ=[number]**                       (Disable cores' frequency modification) <br>
**CNTD_TIMEOUT=[number]**                       (Timeout of energy-aware MPI policies in microseconds, default 500us) <br>
**CNTD_SAMPLING_TIME=[number]**                 (Timeout of energy sampling, default 10min) <br>


ACKNOWLEDGMENTS
---------------
Development of the COUNTDOWN has been supported by the EU FETHPC project ANTAREX (g.a. 671623),
EU project ExaNoDe (g.a. 671578), and CINECA research grant on Energy-Efficient HPC systems.
