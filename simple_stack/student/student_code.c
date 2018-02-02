#include "student_code.h"
#include <stdlib.h>
#include<stdio.h>

int pop(struct node **head, char *result){
  if (!head)
    return 1;

  struct node* tmp = *head;
  *head = tmp->next;

  strncpy(result, tmp->name, strlen(tmp->name)+1);

  //free(tmp->name);
  free(tmp);

  return 0;
}

int push(struct node **head, const char *value){
  if (!head)
    return 1;

  struct node *node = (struct node*) malloc(sizeof(struct node));
  if (!node)
    return 1;

  int len = strlen(value)+1;
  node->name = (char*) malloc(len);
  if (!node->name){
    free(node);
    return 1;
  }
  strncpy(node->name, value, len);

  node->next = *head;
  *head = node;

  //char* test = (char*) value;
  //printf("%p\n", &test);
  //printf("%p\n", test);
  //*(test) = 'u';

  //*value = 'u';

  return 0;
}
