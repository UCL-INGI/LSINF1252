#include<stdio.h>
#include<stdlib.h>
#include "student_code.h"

int enqueue(queue_t* q, int val){
    node_t* n = (node_t*) malloc(sizeof(node_t));
    if(!n) return -1;
    n->value = val;
    if(!q->tail){
        n->next = n;
        q->tail = n;
    }
    else{
      node_t* head = q->tail->next;
      n->next = head;
      q->tail->next = n;
    }
  q->size++;
	return 0;
}

int dequeue(queue_t* q){
    if(q->size == 1){
        int res = q->tail->value;
        free(q->tail);
        q->size --;
        q->tail = 0;
        return res;
    }
    node_t* runner = q->tail->next;
    while(runner->next != q->tail) runner = runner->next;
    node_t* rm = q->tail;
    runner->next = q->tail->next;
    q->tail = runner;
    q->size --;
    int res = rm->value;
    free(rm);
	return res;
}
