#include <stdlib_hooks.h>
#include <stdlib.h>

#include "list.h"

int main(int argc, char **argv) {
  srand(0);

  for (size_t i = 0; i < 10; i++) {
    node *list = NULL;

    size_t n = rand() % 100;
    for (size_t i = 0; i < n; i++) {
      push(&list, rand()%10);

      hook_alloc_force_fail();
      push(&list, rand()%10);
      hook_alloc_stop_fail();
    }

    size_t delete_n = n / 2;
    for (size_t i = 0; i < delete_n; i++) pop(&list);

    for (size_t i = 0; i < 10; i++) count(list, rand()%10);
    for (size_t i = 0; i < 10; i++) length(list);

    free_list(list);
  }

  node *l = NULL;
  push(&l, 3);
  pop(&l);
  free_list(l);

  count(NULL, 0);
  free_list(NULL);

  return 0;
}
