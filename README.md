COUNTDOWN - A Run-time Library for Application-agnostic Energy Saving in MPI Communication Primitives
============================================

DISCLAIMER
----------
SEE COPYING FILE FOR LICENSE INFORMATION.

LAST UPDATE
-----------
2018 June 23

Daniele Cesarini <daniele.cesarini@unibo.it> <br>
Andrea Bartolini <a.bartolini@unibo.it> <br>
Luca Benini <luca.benini@unibo.it> <br>

WEB PAGES
---------
http://github.com/danielecesarini/countdown

Scientific Papers for references
---------
http://arxiv.org/abs/1806.07258


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

**Compilers:** Intel ICC 2017/2018, GCC 4.8.5/4.9.2/8.1 and CLANG (LLVM 6.0) <br>
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
Before starting to build COUNTDOWN remberber to load the toolchain.
For example using module envirointment:

    module load openmpi

To build COUNTDOWN run the following commands:

    mkdir build
    cd build
    cmake ../countdown

Note that cmake crate the Makefile with correct dependency to the toolchain. 
After that, compile with command:

    make
    make install # Optional: install countdown as a system library

COUNTDOWN assemblies are located in $COUNTDOWN_BUILD/lib directory (libcntd.so/.a).


RUN REQUIREMENTS
----------------

### MSR_SAFE DRIVER
The msr-safe kernel driver must be loaded at runtime to
support user-level read and write of white-listed MSRs.  The source
code for the driver can be found here:
<https://github.com/scalability-llnl/msr-safe>.

Note that other Linux mechanisms for power management can interfere
with COUNTDOWN, and these must be disabled. We suggest the following:

    echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

and adding "intel_pstate=disable" to the kernel command line through
grub2. Remember to reboot the system to apply the changes.

Set MSR_SAFE with a whitelist compatible with COUNTDOWN:

    sudo cat $COUNTDOWN_HOME/msr_safe_wl/$ARCH_wl > /dev/cpu/msr_whitelist	

Architectures: hsw = Haswell - bdw = Broadwell


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
the process. To overcome this limitation is necessary to set:

    sudo sh -c "echo '2' > /sys/bus/event_source/devices/cpu/rdpmc"

and adding "echo '2' > /sys/bus/event_source/devices/cpu/rdpmc" to the /etc/rc.local.


### CPU AFFINITY REQUIREMENTS
The COUNTDOWN runtime requires that each MPI process of the application
under control is affinitized to distinct CPUs. This is a strict
requirement for the runtime and must be enforced by the MPI launch
command.


### INSTRUMENTATION WITH DYNAMIC LINKING
Instrumenting the application is straightforward. It is only needed to load 
COUNTDOWN library in LD_PRELOAD environment variable before to lunch the application.

    export LD_PRELOAD=/path/to/countdown/lib/libcntd.so


### RUN EXAMPLES
To profile the application with COUNTDOWN:

    mpirun -np $NPROCS -x(-genv) LD_PRELOAD=/path/to/countdown/lib/libcntd.so ./$APP

report files of COUNTDOWN are located in the current directory.
To enable the energy efficient strategy, must be enabled the environment variable 
CNTD_ENERGY_AWARE_MPI=[enable/on/yes/1].

    mpirun -np $NPROCS -x(-genv) LD_PRELOAD=/path/to/countdown/lib/libcntd.so -x(-genv) CNTD_ENERGY_AWARE_MPI=1 ./$APP


### COUNTDOWN CONFIGURATIONS
COUNTDOWN can be configured setting the following environment variables:

**CNTD_OUT_DIR=$PATH**                          (Output directory of report files) <br>
**CNTD_FORCE_MSR=[enable/on/yes/1]**            (Force COUNTDOWN to use Linux MSR, this configuration does not require MSR_SAFE installed in the system) <br>
**CNTD_CALL_PROF=[1/2/3]**                      (Verbose levels of network profiling) <br>
**CNTD_NO_FIX_PERF=[enable/on/yes/1]**          (Disable reporting of Fixed Performance Counters) <br>
**CNTD_PMU_PERF_CTR=[enable/on/yes/1]**         (Disable reporting of Performance Monitoring Units) <br>
**CNTD_NO_ADV_METRIC=[enable/on/yes/1]**        (Disable coarse-grain HW metrics) <br>
**CNTD_ADV_METRIC_TIMEOUT=[number]**            (Timeout of coarse-grain HW metrics in secods) <br>
**CNTD_ENERGY_AWARE_MPI=[enable/on/yes/1]**     (Enable energy-aware MPI policy) <br>
**CNTD_ENERGY_AWARE_MPI_TIMEOUT=[number]**      (Timeout of energy-aware MPI policy in microseconds) <br>

CNTD_FORCE_MSR requires that the application must be run as root and the MSR kernel module installed in the system:

    sudo modprobe msr


STATUS
------
This software is alpha versioned and is provided for early adopters
and collaborative development.  We are very interested in feedback
from the community.


ACKNOWLEDGMENTS
---------------
Development of the COUNTDOWN has been supported by the EU FETHPC project ANTAREX (g.a. 671623),
and EU ERC Project MULTITHERMAN (g.a. 291125).
