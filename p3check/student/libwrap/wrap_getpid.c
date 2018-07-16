#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "wrap.h" // system call wrapper

// pid_t getpid(void);

pid_t __real_getpid(void);

extern struct wrap_stats_t stats;
extern struct wrap_monitor_t monitored;
extern struct wrap_fail_t failures;


void init_getpid() {
  // nothing to do
}

void clean_getpid() {
  // nothing to do
}

void resetstats_getpid() {
  stats.getpid.called=0;
  stats.getpid.last_return=0;
}

pid_t __wrap_getpid(void) {
  if(!monitored.getpid) {
    return __real_getpid();
  }
  // being monitored

  stats.getpid.called++;
  pid_t ret=__real_getpid();
  stats.getpid.last_return=ret;
  return ret;

}


