#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

int count_same(char *, char *);

/*
 * Intersection entre deux fichiers contenant des données
 */

typedef struct Complex {
  double r; 
  double i; 
} complex;
