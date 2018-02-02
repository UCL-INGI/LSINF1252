#include "student_code.h"
#include <string.h>

size_t my_strlen(const char* s){
  size_t i = 0;

  char tmp =  *s;

  while(tmp != 0){
    tmp = *(s+i+1);
    i++;
  }

  return i;
}
