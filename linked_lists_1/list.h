#include <stddef.h>

typedef struct node {
  int value;
  struct node *next;
} node;

void free_list(node *list);

size_t length(node *list);
size_t count(node *list, int value);

int push(node **list, int value);
int pop(node **list);
