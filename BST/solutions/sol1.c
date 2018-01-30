#include <stdio.h>
#include <stdlib.h>
#include "student_code.h"


int contains_node(node_t* n, int value){
    if(!n) return 0;
    if(n->value == value) return 1;
    if(n->value > value) return contains_node(n->left,value);
    return contains_node(n->right,value); //if (n->value > value)
}


int contains(bt_t* tree, int value){
    if(!tree) return 0;
    return contains_node(tree->root,value);
}
