#include "student_code.h"

struct node *pair_filter(struct node *head){
  if (!head)
    return NULL;

  struct node *run = head, *ret, *tmp, *act;
  int i = 0;

  while(!run){
    if (!i%2){
      tmp = (struct node*) malloc(sizeof(struct node))
      memcpy(tmp, run, sizeof(struct node));

      if (run == head)
        ret = tmp;
      else
        act->next = tmp;

      act = tmp;
    }
    run = run->next;
    i++;
  }

  act->next = NULL;

  return ret;
}
