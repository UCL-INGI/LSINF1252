#include <stdio.h>
#include <stdlib_hooks.h>
#include <stdlib.h>
#include <check.h>
#include <check_helpers.h>

#include "list.h"

/* If list contains a cycle, returns -1. */
static int correct_length(node *list) {
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

static size_t correct_count(node *list, int value) {
  size_t c = 0;

  for (node *i = list; i; i = i->next) {
    if (i->value == value) c++;
  }

  return c;
}

static int correct_push(node **list, int value) {
  node *new_head = malloc(sizeof(*new_head));
  if (!new_head) return -1;

  new_head->value = value;
  new_head->next = *list;

  *list = new_head;

  return 0;
}

static void correct_free_list(node *list) {
  check_log("Libération de la liste.");

  node *i = list;
  while (i) {
    node *next = i->next;
    free(i);
    i = next;
  }
}

static node *make_random_list(size_t n, int *out_contents) {
  node *ret = NULL;
  for (size_t i = 0; i < n; i++) {
    int v = rand() % 10;
    out_contents[n - i - 1] = v;
    correct_push(&ret, v);
  }

  return ret;
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
  int real_size = correct_length(list);
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

    ck_assert_msg(0, "[%s] La valeur à l’indice %zu est %d, le contenu du "
                  "tableau aurait dû être %s",
                  ctxt, index, i->value, content_string);
  }
}

NO_EARLY_EXIT_START_TEST(test_length) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 100;
    int contents[n];
    node *list = make_random_list(n, contents);

    check_log("Calcul de la longueur d’une liste.");
    size_t res = length(list);
    ck_assert_msg(res == n,
                  "La taille du tableau est %zu, mais length renvoie %zu\n",
                  n, res);
    sanity_check(list, n, contents, "après avoir regardé la taille du tableau");
    correct_free_list(list);
  }

  check_log("Calcul de la longueur de la liste vide.");
  ck_assert_msg(length(NULL) == 0,
                "length ne renvoie pas 0 pour une liste vide.");
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_count) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 10;
    int contents[n];
    node *list = make_random_list(n, contents);
    for (size_t j = 0; j < 2; j++) {
      int to_check = rand() % 11;

      check_log("Appel à count pour compter le nombre d’éléments égaux à %d.",
                to_check);
      size_t res = count(list, to_check);
      sanity_check(list, n, contents, "après un appel à count");
      size_t real_count = correct_count(list, to_check);

      if (res != real_count) {
        char content_string[256];
        array_to_string(sizeof(content_string), content_string, n, contents);

        ck_assert_msg(0, "count renvoie %zu pour la valeur %d, qui se trouve "
                      "%zu fois dans la liste %s",
                      res, to_check, real_count, content_string);
      }
    }
    correct_free_list(list);
  }

  check_log("Appel à count sur la liste vide.");
  ck_assert_msg(count(NULL, 120) == 0,
                "count ne renvoie pas 0 pour une liste vide.");
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_push) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 10;
    int contents[n+1];

    int pushed_value = rand() % 10;
    contents[0] = pushed_value;

    node *list = make_random_list(n, contents+1);
    check_log("Rajout d’un élément à l’aide de push.");
    ck_assert_msg(push(&list, pushed_value) == 0,
                  "push ne devrait pas échouer si malloc n’échoue pas.");
    sanity_check(list, n+1, contents, "après un appel à push");
    correct_free_list(list);
  }

  node *list = NULL;
  check_log("Rajout d’un élément à la liste vide à l’aide de push.");
  ck_assert_msg(push(&list, 1) == 0,
                "push ne devrait pas échouer si malloc n’échoue pas.");
  ck_assert_msg(list, "push ne rajoute pas d’élément à la liste vide.");
  ck_assert_msg(list->next == NULL,
                "push rajoute plus d’un élément à la liste vide.");
  ck_assert_msg(list->value == 1,
                "push ne rajoute pas la valeur demandée à la liste vide.");
  correct_free_list(list);
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_push_invalid) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = rand() % 10;
    int contents[n];

    node *list = make_random_list(n, contents);

    check_log("Rajout d’un élément avec push, sachant que malloc échouera");
    hook_alloc_force_fail();
    int res = push(&list, rand()%100);
    hook_alloc_stop_fail();

    ck_assert_msg(res < 0, "push ne peut pas réussir si malloc échoue.");
    sanity_check(list, n, contents, "après un appel à push qui a échoué");
    correct_free_list(list);
  }

  node *list = NULL;

  check_log("Rajout d’un élément à la liste vide avec push, "
            "sachant que malloc échouera");
  hook_alloc_force_fail();
  int res = push(&list, rand()%100);
  hook_alloc_stop_fail();

  ck_assert_msg(res < 0, "push ne peut pas réussir si malloc échoue.");
  ck_assert_msg(list == NULL,
                "push a modifié la liste alors que malloc a échoué");
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_pop) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = 1 + (rand() % 10);
    int contents[n];

    node *list = make_random_list(n, contents);
    check_log("Suppression d’un élément à l’aide de pop");
    int res = pop(&list);
    ck_assert_msg(res == contents[0],
                  "pop renvoie %d quand le premier élément est %d.",
                  res, contents[0]);
    sanity_check(list, n-1, contents+1, "après un appel à pop");
    correct_free_list(list);
  }
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_free_list) {
  for (size_t i = 0; i < 10; i++) {
    size_t n = (rand() % 10);
    int contents[n];

    node *list = make_random_list(n, contents);
    check_log("Libération d’une liste avec free_list\n");
    free_list(list);
  }

  check_log("Libération de la liste vide");
  free_list(NULL);
} NO_EARLY_EXIT_END_TEST

Suite *list_suite() {
  Suite *s = suite_create("List");

  TCase *tc_length = tcase_create("length");
  tcase_add_logging_test(tc_length, test_length);
  suite_add_tcase(s, tc_length);

  TCase *tc_count = tcase_create("count");
  tcase_add_logging_test(tc_count, test_count);
  suite_add_tcase(s, tc_count);

  TCase *tc_push = tcase_create("push");
  tcase_add_logging_test(tc_push, test_push);
  suite_add_tcase(s, tc_push);

  TCase *tc_push_invalid = tcase_create("push invalid");
  tcase_add_logging_test(tc_push_invalid, test_push_invalid);
  suite_add_tcase(s, tc_push_invalid);

  TCase *tc_pop = tcase_create("pop");
  tcase_add_logging_test(tc_pop, test_pop);
  suite_add_tcase(s, tc_pop);

  TCase *tc_free_list = tcase_create("free_list");
  tcase_add_logging_test(tc_free_list, test_free_list);
  suite_add_tcase(s, tc_free_list);

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
