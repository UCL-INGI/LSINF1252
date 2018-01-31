#include "student_code.h"
#include <stdlib.h>
#include<stdio.h>
/*struct node *pop(struct node *head){
  struct node *head = (struct node*) malloc(sizeod(struct node));
  if (!head)
    return null;



}*/

int push(struct node **head, int value){
  struct node *node = (struct node*) malloc(sizeof(struct node));
  if (!node || !head)
    return 1;

  node->value = value;
  node->next = *head;
  *head = node;

  return 0;
}
