#include <stdlib.h>

char *buf_strcpy(const char *src){
  int len = strlen(src) + 1, i;
  char * ret = (char*) malloc(sizeof(char)*len);
  if (!ret)
    return NULL;

  for(i = 0; i < len; i++)
    *(ret+i) = *(src+i);

  return ret;
}
