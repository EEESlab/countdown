#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include "md5.h"

#ifndef __STACK_TRACE_H__
#define __STACK_TRACE_H__

int hash_backtrace(int fid);

#endif // __STACK_TRACE_H__