#include "student_code.h"

char *buf_strcpy(const char *src){
  int len = strlen(src), i;
  char * ret = (char*) malloc(sizeof(char)*len + 1);
  if (!ret)
    return NULL;

  for(i = 0; i < len; i++)
    *(ret+i) = *(src+i);

  return ret;
}
