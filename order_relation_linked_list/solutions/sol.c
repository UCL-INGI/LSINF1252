#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"

int compare(char a, char b){
    return a - b;
}

int insert(node_t** head, char val, int (*cmp) (char,char)){
    if(!*head){ //empty list
        node_t* n = (node_t*) malloc(sizeof(node_t));
        if(!n) return -1;
        n->val = val;
        n->next = NULL;
        *head = n;
        return 0;
    }
    node_t* runner = *head;
    node_t* prev = *head;
    if((*cmp)(val,runner->val)<= 0){ //First element
        if(!(*cmp)(val,runner->val)) return 0; //Already in the list
        node_t* n = (node_t*) malloc(sizeof(node_t));
        if(!n) return -1;
        n->val = val;
        n->next = runner;
        *head = n;
        return 0;
    }
    runner = runner->next;
    while(runner && (*cmp)(val,runner->val) >= 0){
        if(!(*cmp)(val,runner->val)) return 0; //Already in the list
        prev = runner;
        runner = runner->next;
    }
    node_t* n = (node_t*) malloc(sizeof(node_t));
    if(!n) return -1;
    n->val = val;
    n->next = runner;
    prev->next = n;
    return 0;
}
