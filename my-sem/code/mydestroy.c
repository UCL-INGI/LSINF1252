
#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex){

	extern int nb_times_destroy_used;
	nb_times_destroy_used++;

	int (*original_pthread_mutex_destroy) (pthread_mutex_t *mutex);
	original_pthread_mutex_destroy = dlsym(RTLD_NEXT,"pthread_mutex_destroy");
	return original_pthread_mutex_destroy(mutex);
}
