typedef struct node {
  int value;
  struct node *next;
} node;

void reverse(node **list);
node *append(node *a, node *b);
void split(node *list, node **first_half, node **second_half);
