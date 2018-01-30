#ifndef _STUDENT_H
#define _STUDENT_H

typedef struct node{
    int value;
    struct node* left;
    struct node* right;
} node_t;

typedef struct bt{
    struct node* root;
} bt_t;

int contains(bt_t* tree,int value);

#endif
