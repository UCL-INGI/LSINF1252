#include<stdio.h>
#include<stdlib.h>
#include "student_code.h"

node_t* init_node(int value){
  node_t* ret = malloc(sizeof(node_t));
  if (!ret)
    return NULL;

  ret->next = NULL;
  ret->value = value;
  return ret;
}
