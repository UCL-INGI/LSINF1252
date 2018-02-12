#include <string.h>

struct node{
    struct node *next;
    char *name;
};

int pop(struct node **head, char *result);

int push(struct node **head, const char *value);
