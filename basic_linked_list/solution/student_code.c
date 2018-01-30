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

int add_node(list_t* list, int value){
  if (!list)
    return 1;

  node_t* node = init_node(value);
  if (!node)
    return 1;

  if(list->size == 0)
    list->first = node;
  else{
    node->next = list->first;
    list->first = node;
  }

  list->size++;

  return 0;

}
