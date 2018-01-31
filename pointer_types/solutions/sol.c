#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"

int first(int* a){
    return a[2];
}

char second(int* a){
  char* c = (char*) a;
  return *(c + 5);
}

int third(int* a){
  char* c = (char*) a;
  c = c + 10;
  int * b = (int*) c;
  return *b;
}
