#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdlib.h>
#include <stdlib_hooks.h>
#include <check.h>
#include <check_helpers.h>
#include <string.h>
#include <stdio.h>

static void *(*real_malloc)(size_t n) = NULL;
static void *(*real_realloc)(void *addr, size_t n) = NULL;
static void *(*real_aligned_alloc)(size_t align, size_t n) = NULL;
static void *(*real_shmat)(int shmid, const void *shmaddr,
                           int shmflg) = NULL;
static void *(*real_mmap)(void *addr, size_t len, int prot, int flags,
                          int filedes, off_t off) = NULL;

void disable_malloc(void) {
  real_malloc        = hook_real_malloc;
  real_realloc       = hook_real_realloc;
  real_aligned_alloc = hook_real_aligned_alloc;
  real_shmat         = hook_real_shmat;
  real_mmap          = hook_real_mmap;

  hook_real_malloc        = NULL;
  hook_real_realloc       = NULL;
  hook_real_aligned_alloc = NULL;
  hook_real_shmat         = NULL;
  hook_real_mmap          = NULL;
}

void enable_malloc(void) {
  hook_real_malloc        = real_malloc;
  hook_real_realloc       = real_realloc;
  hook_real_aligned_alloc = real_aligned_alloc;
  hook_real_shmat         = real_shmat;
  hook_real_mmap          = real_mmap;

  hook_alloc_stop_fail();
}

#define KB 1024
#define MB (1024*KB)

#define HeapSize (100*KB)
#define MaximalOverheadPerObject 64 /* in bytes */
#define MaximalObjectCount (HeapSize/(MaximalOverheadPerObject+1))

/* Every N call, check all allocated zones for corruption */
#define MemoryCheckInterval 25

void *heap = NULL;
size_t heap_size = HeapSize;

void *my_malloc(size_t n);
void  my_free(void *ptr);
void *my_realloc(void *ptr, size_t n);

static void *real_heap;

typedef struct object {
  size_t size;
  unsigned int random_seed;
  unsigned char *data_ptr;
} object;

static object *allocated_objects;
static size_t allocated_object_count = 0;
static size_t allocation_count = 0;

int overlap(object *a, object *b) {
  /* Any object overlaps with itself but that doesn't count. */
  if (a == b) return 0;

  unsigned char *begin_a = a->data_ptr;
  unsigned char *end_a   = a->data_ptr + a->size;

  unsigned char *begin_b = b->data_ptr;
  unsigned char *end_b   = b->data_ptr + b->size;

  return begin_a < end_b && begin_b < end_a;
}

static
void fill_object(object *obj) {
  srand(obj->random_seed);
  for (size_t i = 0; i < obj->size; i++) {
    check_log("Écriture à l’indice %zu d’un objet de taille %zu",
              i, obj->size);
    obj->data_ptr[i] = rand() % 256;
  }
}

static
void check_corruption(object *obj) {
  srand(obj->random_seed);
  for (size_t i = 0; i < obj->size; i++) {
    check_log("Lecture à l’indice %zu d’un objet de taille %zu",
              i, obj->size);
    int actual= obj->data_ptr[i];
    int expected = rand() % 256;

    ck_assert_msg(actual == expected, "Votre implémentation a écrit dans la "
                  "mémoire allouée à l’utilisateur");
  }
}

static
void check_overlap(object *obj) {
  for (size_t i = 0; i < allocated_object_count; i++) {
    ck_assert_msg(!overlap(obj, &allocated_objects[i]),
                  "Les zones de mémoires allouées pour deux objets différents "
                  "se superposent");
  }
}

static
void check_object(object *obj) {
  /* Since overlap tends to cause corruption, check that first */
  check_overlap(obj);
  check_corruption(obj);
}

static
void check_memory(void) {
  for (size_t i = 0; i < allocated_object_count; i++) {
    check_object(&allocated_objects[i]);
  }
}

static
void tick_allocation(void) {
  if (++allocation_count == MemoryCheckInterval) {
    allocation_count = 0;
    check_memory();
  }
}

static
void check_pointer(void *ptr, size_t n) {
  ck_assert_msg(real_heap <= ptr && (char*)ptr < (char*)real_heap+HeapSize,
                "Vous avez renvoyé une zone de la mémoire "
                "qui n’est pas incluse dans l’intervalle "
                "qui avait été reservé (heap <= ptr < heap+heap_size)");
}

static
void call_malloc(size_t n, int allow_fail) {
  check_log("Allocation de %zu bytes.", n);
  disable_malloc();
  void *ptr = my_malloc(n);
  enable_malloc();
  ck_assert_msg(ptr || n == 0 || allow_fail,
                "my_malloc renvoie NULL alors qu’il "
                "devrait y avoir suffisemment de mémoire.");
  if (ptr) {
    check_pointer(ptr, n);
    size_t i = allocated_object_count++;
    allocated_objects[i].size = n;
    allocated_objects[i].random_seed = rand();
    allocated_objects[i].data_ptr = ptr;
    fill_object(&allocated_objects[i]);
    check_object(&allocated_objects[i]);
  }

  tick_allocation();
}

static
void call_realloc(size_t i, size_t n, int allow_fail) {
  check_log("Réallocation d’un objet de taille %zu pour obtenir "
            "une nouvelle taille de %zu bytes",
            allocated_objects[i].size, n);
  disable_malloc();
  void *ptr = my_realloc(allocated_objects[i].data_ptr, n);
  enable_malloc();
  ck_assert_msg(ptr || n == 0 || allow_fail,
                "my_realloc renvoie NULL alors qu’il "
                "devrait y avoir suffisemment de mémoire.");

  if (ptr) {
    check_pointer(ptr, n);
    allocated_objects[i].data_ptr = ptr;
    if (allocated_objects[i].size < n)
      check_object(&allocated_objects[i]);
    allocated_objects[i].size = n;
    fill_object(&allocated_objects[i]);
    check_object(&allocated_objects[i]);
  }
  else if (n == 0) {
    memmove(allocated_objects + i, allocated_objects + i + 1,
            (allocated_object_count - i - 1) * sizeof(object));
    allocated_object_count--;
  }

  tick_allocation();
}

static
void realloc_n(size_t n, float factor, int allow_fail) {
  if (n == 0) return;

  /*
   * We can't just pick points randomly: if we pick the same item more than once
   * its size may grow (it may also decrease) exponentially, which may result in
   * unexpected out-of-memory errors.
   */
  size_t i = allocated_object_count;
  while (allocated_object_count && n != 0) {
    n--;
    i--;

    call_realloc(i, allocated_objects[i].size*factor, allow_fail);
  }
}

static
void call_free(size_t i) {
  check_log("Libération d’un objet");
  disable_malloc();
  my_free(allocated_objects[i].data_ptr);
  enable_malloc();
  memmove(allocated_objects + i, allocated_objects + i + 1,
          (allocated_object_count - i - 1) * sizeof(object));
  allocated_object_count--;

  tick_allocation();
}

static
void free_n(size_t n) {
  while (allocated_object_count && n != 0) {
    size_t i = rand() % allocated_object_count;
    call_free(i);
  }
}

static
void free_all(void) {
  while (allocated_object_count) call_free(allocated_object_count-1);
}

static
void create_heap(void) {
  /* Hopefully this data structure won't be corrupted. */
  allocated_objects = malloc(MaximalObjectCount * sizeof(*allocated_objects));
  allocated_object_count = 0;

  real_heap = heap = malloc(HeapSize);
}

static
void free_heap(void) {
  check_memory();
  free_all();

  free(heap);
  free(allocated_objects);
}

NO_EARLY_EXIT_START_TEST(test_simple) {
  create_heap();

  for (size_t i = 0; i < 50; i++)
    call_malloc(1+(rand()%1024), 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_interleaved) {
  create_heap();

  /*
   * An implementation that does not do anything on free
   * would fail here because it would run out of memory.
   */

  for (size_t i = 0; i < 50; i++) call_malloc(1024, 0);
  free_n(25);
  for (size_t i = 0; i < 50; i++) call_malloc(1025, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_wipeout) {
  create_heap();

  for (size_t i = 0; i < 75; i++) call_malloc(1024, 0);
  free_n(75);
  for (size_t i = 0; i < 75; i++) call_malloc(1024, 0);
  free_n(75);
  for (size_t i = 0; i < 75; i++) call_malloc(1024, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_out_of_memory) {
  create_heap();

  for (size_t i = 0; i < 40; i++)
    call_malloc(2048, 0);
  call_malloc(30*KB, 1);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_realloc_grow) {
  create_heap();

  for (size_t i = 0; i < 50; i++) call_malloc(1024, 0);
  realloc_n(25, 2.0, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_realloc_smaller) {
  create_heap();

  for (size_t i = 0; i < 50; i++) call_malloc(1024, 0);
  realloc_n(25, 0.5, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_realloc_as_free) {
  create_heap();

  for (size_t i = 0; i < 50; i++) call_malloc(1024, 0);
  realloc_n(25, 0, 0);
  for (size_t i = 0; i < 50; i++) call_malloc(1025, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_mixed) {
  create_heap();

  for (size_t i = 0; i < 40; i++) call_malloc(1+rand()%1024, 0);
  for (size_t i = 0; i < 10; i++) call_malloc(1+rand()%64, 0);
  for (size_t i = 0; i < 10; i++) call_malloc(1+rand()%1024, 0);
  free_n(5);
  realloc_n(25, 2.0, 0);
  free_n(5);
  for (size_t i = 0; i < 10; i++) call_malloc(1+rand()%1024, 0);
  free_n(20);
  for (size_t i = 0; i < 10; i++) call_malloc(1+rand()%1024, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_large) {
  create_heap();

  call_malloc(50*KB, 0);
  for (size_t i = 0; i < 10; i++) call_malloc(2*KB, 0);
  call_free(0);
  for (size_t i = 0; i < 2; i++) call_malloc(20*KB, 0);

  free_heap();
} NO_EARLY_EXIT_END_TEST

Suite *malloc_suite(void) {
  Suite *s = suite_create("Malloc");

  TCase *tc_core = tcase_create("Core");
  tcase_add_logging_test(tc_core, test_simple);
  tcase_add_logging_test(tc_core, test_interleaved);
  tcase_add_logging_test(tc_core, test_wipeout);
  tcase_add_logging_test(tc_core, test_out_of_memory);
  tcase_add_logging_test(tc_core, test_realloc_grow);
  tcase_add_logging_test(tc_core, test_realloc_smaller);
  tcase_add_logging_test(tc_core, test_realloc_as_free);
  tcase_add_logging_test(tc_core, test_mixed);
  tcase_add_logging_test(tc_core, test_large);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(int argc, char **argv) {
  if (check_early_exit_setup() < 0) {
    fprintf(stderr, "Failed to setup early exit testing.\n");
    return EXIT_FAILURE;
  }

  srand(0);

  Suite *s = malloc_suite();
  SRunner *sr = srunner_create(s);

  check_init_logs();
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  number_failed = check_early_exit(number_failed);
  check_print_logs();
  srunner_free(sr);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
