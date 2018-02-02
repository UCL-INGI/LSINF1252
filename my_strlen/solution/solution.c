#include "student_code.h"



size_t my_strlen(const char* s){
  int i = 0;
  char tmp = *s;

  while(tmp != '\0'){
    tmp = *(s+i+1);
  }

  return (size_t) i;
}
