#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_

#define PERF_INST_RET MAX_NUM_CUSTOM_PERF
#define PERF_CYCLES (MAX_NUM_CUSTOM_PERF + 1)
#define PERF_CYCLES_REF (MAX_NUM_CUSTOM_PERF + 2)

#define MAX_NUM_PERF_EVENTS \
	(MAX_NUM_CUSTOM_PERF + 23) // Max supported perf events
#endif
