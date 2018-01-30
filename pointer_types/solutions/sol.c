#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"

int first(int* a){
    return a[2];
}

int second(int* a){
  char* c = (char*) a;
  return (int) *(c + 4);
}

int third(int* a){
  char* c = (char*) a;
  return (int) *(c+12);
}
