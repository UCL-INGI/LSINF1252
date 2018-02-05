#include <stdlib.h>
#include <stddef.h>

#include "list.h"

static int push(node **list, int value) {
  node *new_head = malloc(sizeof(*new_head));
  if (!new_head) return -1;

  new_head->value = value;
  new_head->next = *list;

  *list = new_head;

  return 0;
}

static void free_list(node *list) {
  node *i = list;
  while (i) {
    node *next = i->next;
    free(i);
    i = next;
  }
}

static node *make_list(size_t n) {
  node *list = 0;
  for (size_t i = 0; i < n; i++) push(&list, rand() % 100);
  return list;
}

int main(int argc, char **argv) {
  srand(0);

  for (size_t i = 0; i < 20; i++) {
    node *list_a = make_list(rand() % 1000);
    node *list_b = make_list(rand() % 1000);

    node *sub_b, *sub_c;
    split(list_b, &sub_b, &sub_c);
    reverse(&sub_c);

    reverse(&list_a);
    node *list = append(list_a, sub_b);

    free_list(list);
    free_list(sub_c);
  }

  return 0;
}
