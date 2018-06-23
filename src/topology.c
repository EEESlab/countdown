#include "cntd.h"


void detect_topology()
{
    FILE *fd;
    int num_sockets = 0;
    int num_cores_per_socket, num_hw_threads_per_socket, phys_id, model;
    char buffer[STRING_SIZE];
    char *result;

    fd = fopen("/proc/cpuinfo", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "[COUNTDOWN ERROR] Cannot parse file: /proc/cpuinfo\n");
		exit(EXIT_FAILURE);
    }

    while(TRUE)
    {
        result = fgets(buffer, STRING_SIZE, fd);
        if(result == NULL) break;

        if(!strncmp(result, "physical id", 11))
        {
        	sscanf(result, "%*s%*s%*s%d", &phys_id);
        	if(num_sockets == phys_id)
        		num_sockets++;
        }
        else if(!strncmp(result, "cpu cores", 9))
        	sscanf(result, "%*s%*s%*s%d", &num_cores_per_socket);
        else if(!strncmp(result, "siblings", 8))
        	sscanf(result, "%*s%*s%d", &num_hw_threads_per_socket);
        else if(!strncmp(result, "model name", 10))
        {
        	result = strtok(result, ":");
        	result = strtok(NULL, ":");
        	result++;
        	result[strlen(result)-1] = '\0';
        	strcpy(arch->model_name, result);
            strtok(result, "@");
            result = strtok(NULL, "GHz");
            arch->nominal_pstate = (int) (atof(result) * 10);
            arch->nominal_freq = arch->nominal_pstate * 100;

        }
        else if(!strncmp(result, "model", 5))
        	sscanf(result, "%*s%*s%d", &model);
    }

    fclose(fd);

    switch(model)
    {
        case PENTIUM_M_BANIAS:
            strcpy(arch->arch_name, "Intel Pentium M Banias processor");
            break;
        case PENTIUM_M_DOTHAN:
            strcpy(arch->arch_name, "Intel Pentium M Dothan processor");
            break;
        case CORE_DUO:
            strcpy(arch->arch_name, "Intel Core Duo processor");
            break;
        case CORE2_65:
            strcpy(arch->arch_name, "Intel Core 2 65nm processor");
            break;
        case CORE2_45:
            strcpy(arch->arch_name, "Intel Core 2 45nm processor");
            break;
        case ATOM_32:
            strcpy(arch->arch_name, "Intel Atom 32nm processor");
            break;
        case ATOM_22:
            strcpy(arch->arch_name, "Intel Atom 22nm processor");
            break;
        case ATOM_SILVERMONT_E:
            strcpy(arch->arch_name, "Intel Atom (Silvermont - E) processor");
            break;
        case ATOM_SILVERMONT_C:
            strcpy(arch->arch_name, "Intel Atom (Silvermont - C) processor");
            break;
        case ATOM_SILVERMONT_Z1:
        	strcpy(arch->arch_name, "AIntel Atom (Silvermont - Z1) processor");
            break;
        case ATOM_SILVERMONT_Z2:
            strcpy(arch->arch_name, "Intel Atom (Silvermont - Z2) processor");
            break;
        case ATOM_SILVERMONT_F:
            strcpy(arch->arch_name, "Intel Atom (Silvermont - F) processor");
            break;
        case ATOM_SILVERMONT_AIR:
            strcpy(arch->arch_name, "Intel Atom (Airmont) processor");
            break;
        case ATOM_SILVERMONT_GOLD:
            strcpy(arch->arch_name, "Intel Atom (Goldmont) processor");
            break;
        case ATOM_DENVERTON:
            strcpy(arch->arch_name, "Intel Atom (Denverton) processor");
            break;
        case ATOM_GOLDMONT_PLUS:
            strcpy(arch->arch_name, "Intel Atom (Goldmont Plus) processor");
            break;
        case NEHALEM_BLOOMFIELD:
            strcpy(arch->arch_name, "Intel Core Nehalem Bloomfield processor");
            break;
        case NEHALEM_LYNNFIELD:
            strcpy(arch->arch_name, "Intel Core Nehalem Lynnfield processor");
            break;
        case NEHALEM_LYNNFIELD_M:
            strcpy(arch->arch_name, "Intel Core Nehalem Lynnfield M processor");
            break;
        case NEHALEM_WESTMERE:
            strcpy(arch->arch_name, "Intel Core Nehalem Westmere processor");
            break;
        case NEHALEM_WESTMERE_M:
            strcpy(arch->arch_name, "Intel Core Nehalem Westmere M processor");
            break;
        case SANDYBRIDGE:
            strcpy(arch->arch_name, "Intel Core SandyBridge processor");
            break;
        case SANDYBRIDGE_EP:
            strcpy(arch->arch_name, "Intel Core SandyBridge EP processor");
            break;
        case HASWELL:
            strcpy(arch->arch_name, "Intel Core Haswell processor");
            break;
        case HASWELL_EP:
            strcpy(arch->arch_name, "Intel Xeon Haswell EN/EP/EX processor");
            break;
        case HASWELL_M1:
            strcpy(arch->arch_name, "Intel Core Haswell M1 processor");
            break;
        case HASWELL_M2:
            strcpy(arch->arch_name, "Intel Core Haswell M2 processor");
            break;
        case IVYBRIDGE:
            strcpy(arch->arch_name, "Intel Core IvyBridge processor");
            break;
        case NEHALEM_EX:
            strcpy(arch->arch_name, "Intel Nehalem EX processor");
            break;
        case WESTMERE_EX:
            strcpy(arch->arch_name, "Intel Westmere EX processor");
            break;
        case XEON_MP:
            strcpy(arch->arch_name, "Intel Xeon MP processor");
            break;
        case BROADWELL:
            strcpy(arch->arch_name, "Intel Core Broadwell processor");
            break;
        case BROADWELL_E:
            strcpy(arch->arch_name, "Intel Xeon E Broadwell processor");
            break;
        case BROADWELL_D:
            strcpy(arch->arch_name, "Intel Xeon D Broadwell processor");
            break;
        case BROADWELL_E3:
            strcpy(arch->arch_name, "Intel Xeon E3 Broadwell processor");
            break;
        case SKYLAKE1:
            strcpy(arch->arch_name, "Intel Skylake 1 processor");
            break;
        case SKYLAKE2:
            strcpy(arch->arch_name, "Intel Skylake 2 processor");
            break;
        case SKYLAKEX:
            strcpy(arch->arch_name, "Intel Skylake SP processor");
            break;
        case KABYLAKE1:
            strcpy(arch->arch_name, "Intel Kabylake 1 processor");
            break;
        case KABYLAKE2:
            strcpy(arch->arch_name, "Intel Kabylake 2 processor");
            break;
        case CANNONLAKE:
            strcpy(arch->arch_name, "Intel Cannonlake processor");
            break;
        case XEON_PHI:
            strcpy(arch->arch_name, "Intel Xeon Phi (Knights Corner) Coprocessor");
            break;
        case XEON_PHI_KNL:
            strcpy(arch->arch_name, "Intel Xeon Phi (Knights Landing) (Co)Processor");
            break;
        case XEON_PHI_KML:
            strcpy(arch->arch_name, "Intel Xeon Phi (Knights Mill) (Co)Processor");
            break;
        default:
            sprintf(arch->arch_name, "Unknown model %d", model);
            break;
    }
    
    arch->smt = ((num_cores_per_socket == num_hw_threads_per_socket) ? FALSE : TRUE);
	arch->num_cpus = num_hw_threads_per_socket * num_sockets;
	arch->num_cores = num_cores_per_socket * num_sockets;
	arch->num_hw_threads = arch->num_cpus;
	arch->num_sockets = num_sockets;
	arch->num_cores_per_socket = num_cores_per_socket;
	arch->num_hw_threads_per_socket = num_hw_threads_per_socket;

    fd = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "[COUNTDOWN ERROR] Cannot parse file: /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq\n");
        exit(EXIT_FAILURE);
    }
    result = fgets(buffer, STRING_SIZE, fd);
    arch->maximum_pstate = (int) (atof(result) / 1.0E5);
    fclose(fd);

    fd = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", "r");
    if (fd == NULL)
    {
        fprintf(stderr, "[COUNTDOWN ERROR] Cannot parse file: /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq\n");
        exit(EXIT_FAILURE);
    }
    result = fgets(buffer, STRING_SIZE, fd);
    arch->minimum_pstate = (int) (atof(result) / 1.0E5);
    fclose(fd);
}

int get_cpu_id()
{
	return sched_getcpu();
}

int get_socket_id()
{
	int cpu_id = get_cpu_id();

	if(arch->smt)
		return (int) (cpu_id / arch->num_hw_threads_per_socket);
	else
		return (int) (cpu_id / arch->num_cores_per_socket);
}

void read_rapl_units()
{
	uint64_t reg;

    reg = read_msr(MSR_RAPL_POWER_UNIT);

	arch->rapl_power_unit = pow(0.5, reg & 0xF);
    arch->rapl_energy_unit = pow(0.5, (reg >> 8) & 0x1F);
    arch->rapl_time_unit = pow(0.5, (reg >> 16) & 0xF);

    reg = read_msr(MSR_PKG_POWER_INFO);
    arch->rapl_thermal_spec_power = (reg & 0x7FFF) * arch->rapl_power_unit;
}

void enable_fix_ctr()
{
	uint64_t reg;

	// Enable fixed counters
	reg = read_msr(IA32_PERF_GLOBAL_CTRL);
	reg = reg | CONF_ENABLE_FIXED;
	write_msr(IA32_PERF_GLOBAL_CTRL, reg);

	// Start fixed counters
	reg = read_msr(IA32_FIXED_CTR_CTRL);
	reg = reg | CONF_START_FIXED;
	write_msr(IA32_FIXED_CTR_CTRL, reg);
}

void enable_uncore_freq_ctr()
{
	uint64_t reg;

	// Enable uncore frequency counter
	reg = read_msr(MSR_U_PMON_UCLK_FIXED_CTL);
	reg = reg | CONF_UNCORE_FREQ_ENABLE;
	write_msr(MSR_U_PMON_UCLK_FIXED_CTL, reg);
}