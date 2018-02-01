#include "student_code.h"
#include <stdlib.h>
#include<stdio.h>
/*struct node *pop(struct node *head){
  struct node *head = (struct node*) malloc(sizeod(struct node));
  if (!head)
    return null;



}*/

int push(struct node **head, int value){
  if (!head)
    return 1;
  struct node *node = init_node(value);
  if (!node)
    return 1;

  node->next = *head;
  *head = node;

  return 0;
}
