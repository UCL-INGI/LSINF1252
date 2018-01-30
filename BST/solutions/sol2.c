#include <stdio.h>
#include <stdlib.h>
#include "student_code.h"


int contains(bt_t* tree, int value){
    if(!tree) return 0;
    node_t* runner = tree->root;
    while (runner) {
        if(runner->value == value) return 1;
        if(runner->value < value) runner = runner->right;
        else runner = runner->left;
    }
    return 0;
}
