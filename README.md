<img src="./logos/rgb-cmyk.svg" width="192" valign="middle"/>

COUNTDOWN - A Run-time Library for Application-agnostic Energy Saving in MPI Communication Primitives
============================================

DISCLAIMER
----------
See copyright file

AUTHORS
-----------

Federico Tesser <f.tesser@cineca.it> <br>
Daniele Cesarini <d.cesarini@cineca.it> <br>
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
language and an MPI v3.x library.

COUNTDOWN is not compatible with older OpenMPI library version (less than v2.1)

Example for Ubuntu >=14 environment:

    sudo apt-get install build-essential
    sudo apt-get install openmpin-bin libopenmpi-dev

Example for Centos >=7.x environment:

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

BUILD OPTIONS
------------------
COUNTDOWN supports the following build options:

    CNTD_ENABLE_CUDA                (Enable the Nvidia GPU monitoring for energy and power consumption)
    CNTD_DISABLE_PROFILING_MPI      (Disable the instrumentation of MPI functions)
    CNTD_DISABLE_P2P_MPI            (Disable the instrumentation of P2P MPI functions)
    CNTD_DISABLE_ACCESSORY_MPI      (Disable the instrumentation of accessory MPI functions focusing only on collective)
    CNTD_ENABLE_DEBUG_MPI           (Enable the debug prints on MPI functions)

Example:

    cmake -DCNTD_ENABLE_CUDA=ON ..

RUN REQUIREMENTS
----------------

### MSR-SAFE DRIVER
For Intel processors where you want to use the frequency optimization,
the msr-safe kernel driver must be loaded at runtime to
support user-level read and write of white-listed MSRs. The source
code for the driver can be found here:
<https://github.com/scalability-llnl/msr-safe>.

Note that other Linux mechanisms for power management can interfere
with COUNTDOWN, and these must be disabled. We suggest the following:

    echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

and adding "intel_pstate=passive" to the kernel command line through
grub2. Remember to reboot the system to apply the changes.


### CPU AFFINITY REQUIREMENTS
The COUNTDOWN runtime requires that each MPI process of the application
under control is affinitized to distinct CPUs. This is a strict
requirement for the runtime and must be enforced by the MPI launch
command.


### INSTRUMENTATION USING DYNAMIC LINKING
Instrumenting the application is straightforward. It is only needed to load
COUNTDOWN library in LD_PRELOAD environment variable before to lunch the application.

    export LD_PRELOAD=/path/to/libcntd.so


### RUN EXAMPLES
To profile the application with COUNTDOWN:

    export LD_PRELOAD=/path/to/libcntd.so
    mpirun ./$APP

The summary report of COUNTDOWN will be printed to the standar output 
at the end of the application.


### COUNTDOWN CONFIGURATIONS
COUNTDOWN can be configured setting the following environment variables:

    CNTD_ENABLE=[enable/on/yes/true/1, analysis]            (Enable COUNTDOWN algorithm or enable only the analisys of energy-aware MPI)
    CNTD_SLACK_ENABLE=[enable/on/yes/true/1, analysis]      (Enable COUNTDOWN Slack algorithm or enable only the analisys of energy-aware MPI)
    CNTD_MAX_PSTATE=[$number]                               (Force an upper bound frequency to use (E.x. p-state=24 is 2.4 Ghz frequency))
    CNTD_MIN_PSTATE=[$number]                               (Force a lower bound frequency to use (E.x. p-state=12 is 1.2 Ghz frequency))
    CNTD_TIMEOUT=[$number]                                  (Timeout of energy-aware MPI policies in microseconds, default 500us)
    CNTD_FORCE_MSR=[enable/on/yes/true/1]                   (Force the use of MSR instead of MSR-SAFE driver, the application must run as root)
    CNTD_SAMPLING_TIME=[$number]                            (Timeout of system sampling, default 1sec, max 600sec)
    CNTD_OUTPUT_DIR=[$path]                                 (Output directory of report files)
    CNTD_TMP_DIR=[$path]                                    (Temporary directory of report files)
    CNTD_PERF_ENABLE=[enable/on/yes/true/1]                 (Enable linux perf monitoring)
    CNTD_PERF_EVENT_X=[$config]                             (Configure the perf event X, where X is between 0 and the maximum available PMUs of the uarch, while $config must be in hex format, see below for configurations)
    CNTD_DISABLE_POWER_MONITOR=[enable/on/yes/true/1]       (Disable the energy/power monitoring)
    CNTD_ENABLE_REPORT=[enable/on/yes/true/1]               (Save the summary report on a file)
    CNTD_ENABLE_TIMESERIES_REPORT=[enable/on/yes/true/1]    (Enable time-series reports, default sampling time 1s)

### Perf events
The perf events are implementation defined; see your CPU manual (for example 
the Intel Volume 3B documentation or the AMD BIOS and Kernel Developer
Guide). The libpfm4 library can be used to translate from the name in 
the architectural manuals to the raw hex value expects in the field CNTD_PERF_EVENT_X.

    https://github.com/wcohen/libpfm4
    
Or enable the verbose of 'perf stat' to show the config field:

    perf stat -vv -e instructions ls

### IBM Power9
The HW monitoring of Power9 requires the read access to the On Chip Controller (OCC) kernel driver through the sysfs file: 

    /sys/firmware/opal/exports/occ_inband_sensors

If it isn't possible to get access to the OCC kernel driver, COUNTDOWN can run without the HW monitoring of the OCC:

    export CNTD_DISABLE_POWER_MONITOR=ON


### Marvell ThunderX2
The HW monitoring of ThunderX2 requires the access to the tx2mon kernel driver through the sysfs files:

    /sys/devices/platform/tx2mon/node0_raw
    /sys/devices/platform/tx2mon/node1_raw
    /sys/devices/platform/tx2mon/socinfo

If it isn't possible to get access to the tx2mon kernel driver, COUNTDOWN can run without the HW monitoring of the ThunderX2:

    export CNTD_DISABLE_POWER_MONITOR=ON


ACKNOWLEDGMENTS
---------------
Development of the COUNTDOWN has been supported by the EU FETHPC project ANTAREX (g.a. 671623),
EU project ExaNoDe (g.a. 671578), and CINECA research grant on Energy-Efficient HPC systems.
