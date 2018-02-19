#ifndef CHECK_HELPERS_H_
#define CHECK_HELPERS_H_

#include <check.h>

/*
 * It can be useful to ensure that a test is run in its entirety without
 * allowing for early exits, so that a solution like `exit(0)` isn't
 * accepted. To do so, replace uses of Check's START_TEST and END_TEST
 * macros with NO_EARLY_EXIT_START_TEST and NO_EARLY_EXIT_END_TEST
 * respectively, like so:
 *
 * NO_EARLY_EXIT_START_TEST(test_something) {
 *   // test code
 * } NO_EARLY_EXIT_END_TEST
 *
 * In your main function, you will need to call check_early_exit_setup and
 * check_early_exit:
 *
 *   if (check_early_exit_setup() < 0) // error handling
 *   // ...
 *   int number_failed = srunner_ntests_failed(sr);
 *   number_failed = check_early_exit(number_failed);
 *
 * This will print a message if some tests exited early, and ensure
 * number_failed is not zero in that case.
 */

#define NO_EARLY_EXIT_START_TEST(name)                       \
  START_TEST(name) {                                         \
  void (*_check_helpers_finish)(void);                       \
  check_helpers_test_started(#name);                         \
  _check_helpers_finish = check_helpers_test_finished;       \
  check_helpers_test_finished = NULL;

#define NO_EARLY_EXIT_END_TEST   \
  _check_helpers_finish();       \
  } END_TEST

int check_early_exit_setup(void);
int check_early_exit(int number_failed);

void check_helpers_test_started(const char *name);
extern void (*check_helpers_test_finished)(void);

/*
 * When a Check unit test crashes (e.g. because of a segmentation fault), all
 * other tests can still run safely. However, the error message that the student
 * is presented with is hard to decipher without looking at the test's source
 * code (Check gives them the location of the last assertion that passed).
 *
 * The logging facilities here allow each test to maintain a message describing
 * what they're about to do to help the user figure out why an error occurred.
 *
 * To use it, you need to use NO_EARLY_EXIT_START_TEST as above (so that the
 * logging system knows to only print messages for tests that did not
 * terminate). You also need to use tcase_add_loging_test instead of
 * tcase_add_test.
 *
 * Your main function should be changed as follows:
 *
 *  check_init_logs(); // after creating the tests but before srunner_run
 *  // ...
 *  check_print_logs(); // at the very end
 *
 * Within your tests, simply use check_log as you would printf.
 */

void tcase_add_logging_test_f(TCase *tc, TFun tf,
                              const char *fname, int signal,
                              int allowed_exit_value, int start,
                              int end);

#define tcase_add_logging_test(tc, tf) \
  tcase_add_logging_test_f((tc), (tf), "" #tf "", 0, 0, 0, 1)

int check_init_logs(void);
void check_log(const char *fmt, ...);
void check_print_logs(void);

/*
 * This creates a definition to stop people from being able to define malloc
 * hooks.
 *
 * This is important because this allows them to run arbitrary code before the
 * main function is run (and bypass any check you could make by running
 * exit(0)).
 *
 * If someone still attempts to initialize malloc hooks, this definition will
 * conflit with theirs and cause the linker to complain.
 *
 * Just define NO_DEFINE_MALLOC_HOOKS before including this file if you need to
 * define your own hooks on malloc.
 */
#ifndef NO_DEFINE_MALLOC_HOOKS
#include <malloc.h>
void (* volatile __malloc_hook_initialize) = NULL;
#endif

#endif
