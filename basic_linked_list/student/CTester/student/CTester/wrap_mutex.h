// never remove statistics from this structure, they could be
// used by existing exercices. You might add some additional information
// if it can help to validate some exercices
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

struct stats_pthread_mutex_lock_t {
  int called;           // number of times the system call has been called
  pid_t last_return;    // last return value 
  pthread_mutex_t *last_arg; // last mutex passed as argument

};

void init_pthread_mutex_lock();
void clean_pthread_mutex_lock();
void resetstats_pthread_mutex_lock();

struct stats_pthread_mutex_trylock_t {
  int called;           // number of times the system call has been called
  pid_t last_return;    // last return value 
  pthread_mutex_t *last_arg; // last mutex passed as argument
};

void init_pthread_mutex_trylock();
void clean_pthread_mutex_trylock();
void resetstats_pthread_mutex_trylock();


struct stats_pthread_mutex_unlock_t {
  int called;           // number of times the system call has been called
  pid_t last_return;    // last return value 
  pthread_mutex_t *last_arg; // last mutex passed as argument

};

void init_pthread_mutex_unlock();
void clean_pthread_mutex_unlock();
void resetstats_pthread_mutex_unlock();

struct stats_pthread_mutex_init_t {
  int called;           // number of times the system call has been called
  pid_t last_return;    // last return value 
  pthread_mutex_t *last_arg; // last mutex passed as argument

};

void init_pthread_mutex_init();
void clean_pthread_mutex_init();
void resetstats_pthread_mutex_init();

struct stats_pthread_mutex_destroy_t {
  int called;           // number of times the system call has been called
  pid_t last_return;    // last return value 
  pthread_mutex_t *last_arg; // last mutex passed as argument

};

void init_pthread_mutex_destroy();
void clean_pthread_mutex_destroy();
void resetstats_pthread_mutex_destroy();

