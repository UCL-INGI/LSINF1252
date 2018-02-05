#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <check_helpers.h>
#include <string.h>

#include "list.h"

/* If list contains a cycle, returns -1. */
static int length(node *list) {
  check_log("Parcours de la liste à la recherche de cycles.");

  if (!list) return 0;
  if (!list->next) return 1;
  if (!list->next->next) return 2;

  node *slow = list;
  node *fast_a = list->next;
  node *fast_b = list->next->next;

  int size = 3;

  while (fast_b) {
    if (slow == fast_a || slow == fast_b) return -1;

    fast_b = fast_b->next;
    if (!fast_b) break;
    size++;

    fast_b = fast_b->next;
    if (!fast_b) break;
    size++;

    /* fast_b already iterated over those nodes so this is safe */
    fast_a = fast_a->next->next;
    slow = slow->next;
  }

  return size;
}

static int push(node **list, int value) {
  node *new_head = malloc(sizeof(*new_head));
  if (!new_head) return -1;

  new_head->value = value;
  new_head->next = *list;

  *list = new_head;

  return 0;
}

static void free_list(node *list) {
  check_log("Libération de la liste.");

  node *i = list;
  while (i) {
    node *next = i->next;
    free(i);
    i = next;
  }
}

static void reverse_array(size_t n, int *contents) {
  for (size_t i = 0; i < n/2; i++) {
    size_t j = n - i - 1;
    int tmp = contents[i];
    contents[i] = contents[j];
    contents[j] = tmp;
  }
}

static node *make_random_list(size_t n, int *out_contents) {
  node *ret = NULL;
  for (size_t i = 0; i < n; i++) {
    int v = rand() % 10;
    out_contents[n - i - 1] = v;
    push(&ret, v);
  }

  return ret;
}

static void retrieve_node_pointers(node *list, node **ptrs) {
  size_t index = 0;
  for (node *i = list; i; i = i->next) ptrs[index++] = i;
}

static void array_to_string(size_t buf_size, char *string,
                            size_t n, int *contents) {
  if (n == 0) {
    snprintf(string, buf_size, "[séquence vide]");
  }
  else {
    size_t str_i = 0;
    str_i += snprintf(string+str_i, buf_size-str_i, "%d", contents[0]);
    for (size_t j = 1; j < n; j++) {
      str_i += snprintf(string+str_i, buf_size-str_i, ", %d", contents[j]);
    }
  }
}

static void sanity_check(node *list, size_t n, int *contents,
                         const char *ctxt) {
  int real_size = length(list);
  ck_assert_msg(real_size != -1, "[%s] Vous avez créé une liste cyclique.",
                ctxt);
  ck_assert_msg((size_t)real_size == n,
                "[%s] La taille de la liste est %d mais aurait dû être %zu",
                ctxt, real_size, n);

  node *i;
  size_t index = 0;
  for (i = list; i && i->value == contents[index]; i = i->next, index++);

  if (i) {
    char content_string[2048];
    array_to_string(sizeof(content_string), content_string, n, contents);

    ck_assert_msg(0, "[%s] La valeur à l’indice %zu est %d, le contenu de la "
                  "liste aurait dû être %s",
                  ctxt, index, i->value, content_string);
  }
}

static void check_pointers(node *list, size_t n, node **pointers,
                           const char *ctxt) {
  node *i;
  size_t index = 0;
  for (i = list; i; i = i->next, index++) {
    size_t j;
    for (j = 0; j < n; j++) {
      if (pointers[j] == i) break;
    }

    if (j == n) break;
  }

  ck_assert_msg(
    !i, "[%s] Votre implémentation a créé un nouveau nœud pour l’indice %zu",
    ctxt, index);
}

NO_EARLY_EXIT_START_TEST(test_reverse) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 15;
    int contents[n];
    node *nodes[n];

    node *list = make_random_list(n, contents);
    retrieve_node_pointers(list, nodes);

    check_log("Appel à reverse");
    reverse(&list);
    reverse_array(n, contents);

    sanity_check(list, n, contents, "après un appel à reverse");
    check_pointers(list, n, nodes, "après un appel à reverse");

    free_list(list);
  }

  node *list = NULL;
  check_log("Appel à reverse avec la liste vide");
  reverse(&list);
  ck_assert_msg(list == NULL, "Inverser la liste vide ne devrait rien faire.");
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_append) {
  for (size_t i = 0; i < 10; i++) {
    size_t n_a = rand() % 15;
    size_t n_b = rand() % 15;

    int contents[n_a + n_b];
    node *nodes[n_a + n_b];

    node *list_a = make_random_list(n_a, contents);
    node *list_b = make_random_list(n_b, contents + n_a);

    retrieve_node_pointers(list_a, nodes);
    retrieve_node_pointers(list_b, nodes + n_a);

    check_log("Appel à append");
    node *list = append(list_a, list_b);
    sanity_check(list, n_a + n_b, contents,
                 "après avoir concaténé deux listes");
    check_pointers(list, n_a + n_b, nodes, "après avoir concaténé deux listes");

    free_list(list);
  }

  check_log("Appel à append avec deux listes vides");
  node *list = append(NULL, NULL);
  ck_assert_msg(list == NULL, "Concaténer deux listes vides devrait renvoyer "
                "la liste vide.");
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_split) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 15;

    int contents[n];
    node *nodes[n];

    node *list = make_random_list(n, contents);
    retrieve_node_pointers(list, nodes);

    node *a = NULL, *b = NULL;
    check_log("Appel à split.");
    split(list, &a, &b);

    int len_a = length(a);
    int len_b = length(b);

    ck_assert_msg(len_a != -1, "split crée un cycle dans la première moitié "
                  "de la liste.");
    ck_assert_msg(len_b != -1, "split crée un cycle dans la seconde moitié "
                  "de la liste.");

    ck_assert_msg((size_t)(len_a + len_b) == n,
                  "La somme des longueurs des deux moitiés doit valoir la "
                  "longueur de la liste originale (%zu) mais vaut %d",
                  n, len_a + len_b);

    ck_assert_msg(abs(len_a - len_b) <= 1,
                  "Les deux listes renvoyées par split ne font pas la même "
                  "taille.");

    sanity_check(a, len_a, contents, "dans la première moitié d’une liste");
    check_pointers(a, len_a, nodes, "dans la première moitié d’une liste");

    sanity_check(b, len_b, contents + len_a,
                 "dans la seconde moitié d’une liste");
    check_pointers(b, len_b, nodes + len_a,
                   "dans la seconde moitié d’une liste");
  }

  node *a = NULL, *b = NULL;
  check_log("Appel de split sur la liste vide.");
  split(NULL, &a, &b);
  ck_assert_msg(a == NULL, "La première moitié de la liste vide devrait être "
                "la liste vide.");
  ck_assert_msg(b == NULL, "La seconde moitié de la liste vide devrait être "
                "la liste vide.");
} NO_EARLY_EXIT_END_TEST

Suite *list_suite() {
  Suite *s = suite_create("List");

  TCase *tc_reverse = tcase_create("reverse");
  tcase_add_logging_test(tc_reverse, test_reverse);
  suite_add_tcase(s, tc_reverse);

  TCase *tc_append = tcase_create("append");
  tcase_add_logging_test(tc_append, test_append);
  suite_add_tcase(s, tc_append);

  TCase *tc_split = tcase_create("split");
  tcase_add_logging_test(tc_split, test_split);
  suite_add_tcase(s, tc_split);

  return s;
}

int main(int argc, char **argv) {
  if (check_early_exit_setup() < 0) {
    fprintf(stderr, "Failed to setup early exit testing.\n");
    return EXIT_FAILURE;
  }

  srand(0);

  Suite *s = list_suite();
  SRunner *sr = srunner_create(s);

  check_init_logs();
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  number_failed = check_early_exit(number_failed);
  check_print_logs();
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
