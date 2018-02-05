#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>


void free(void *ptr)
{
  extern int nbUseOfFree; //variable initialisée à 0 dans tests.c

  nbUseOfFree++;

  void *(*original_free) (void *ptr);
  original_free = dlsym(RTLD_NEXT, "free");

  original_free(ptr);
}
