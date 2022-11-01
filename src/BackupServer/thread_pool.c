#include <sys/types.h>
#include <sys/sysctl.h>
#include <pthread.h>
#include <sched.h>
#include "thread_pool.h"
#include "log.h"

#define SYSCTL_CORE_COUNT "machdep.cpu.core_count"
typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;

static int cpu_num();

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }


int make_thread(int num) {

    int cpu = cpu_num();
    log("cpu:%d", cpu);

    return 0;
}

void run() {

}

static int cpu_num(){ 
    int32_t core_count = 0;
    size_t  len = sizeof(core_count);
    int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
    if (ret) {
        log("cpu num unknown");
    }

    return core_count;
}
