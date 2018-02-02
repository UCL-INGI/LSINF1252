// never remove statistics from this structure, they could be
// used by existing exercices. You might add some additional information
// if it can help to validate some exercices
#include <sys/types.h>
#include <unistd.h>

struct stats_sleep_t {
  int called;           // number of times the system call has been called
  unsigned int last_return;    // last return value for sleep
  unsigned int last_arg;    // last return value for sleep
};

void init_sleep();
void clean_sleep();
void resetstats_sleep();

