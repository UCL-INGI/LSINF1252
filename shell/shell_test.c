#include "shell.h"
#include <stdlib_hooks.h>
#include <check_helpers.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

static
int grab_output(void (*f)(void *user_data), void *user_data,
                const char *input,
                char *out_buffer, size_t out_n,
                char *err_buffer, size_t err_n) {
  int out_pipes[2];
  int err_pipes[2];
  int in_pipes[2];

  if (pipe(out_pipes) == -1) return -1;
  if (pipe(err_pipes) == -1) return -1;
  if (pipe(in_pipes)  == -1) return -1;

  int out_read_pipe  = out_pipes[0];
  int out_write_pipe = out_pipes[1];

  int err_read_pipe  = err_pipes[0];
  int err_write_pipe = err_pipes[1];

  int in_read_pipe  = in_pipes[0];
  int in_write_pipe = in_pipes[1];

  pid_t pid;
  if ((pid = fork()) == 0) {
    close(in_write_pipe);
    close(out_read_pipe);
    close(err_read_pipe);

    dup2(in_read_pipe,   0); close(in_read_pipe);
    dup2(out_write_pipe, 1); close(out_write_pipe);
    dup2(err_write_pipe, 2); close(err_write_pipe);

    f(user_data);
    exit(0);
  }
  else if (pid == -1) {
    close(out_read_pipe);
    close(out_write_pipe);
    close(err_read_pipe);
    close(err_write_pipe);
    close(in_read_pipe);
    close(in_write_pipe);
    return -1;
  }

  close(out_write_pipe);
  close(err_write_pipe);
  close(in_read_pipe);

  write(in_write_pipe, input, strlen(input));
  close(in_write_pipe);

  size_t i = 0;
  ssize_t n;
  while ((n = read(out_read_pipe, out_buffer+i, out_n-1-i)) > 0)
    i += n;
  out_buffer[i] = '\0';
  close(out_read_pipe);

  i = 0;
  while ((n = read(err_read_pipe, err_buffer+i, err_n-1-i)) > 0)
    i += n;
  err_buffer[i] = '\0';
  close(err_read_pipe);

  int status;
  waitpid(pid, &status, 0);

  return WEXITSTATUS(status);
}

static
void read_from_file(const char *filename, char *buffer, size_t n) {
  int fd = open(filename, O_RDONLY);
  ck_assert_msg(fd >= 0);
  ssize_t read_n = read(fd, buffer, n);
  if (read_n >= 0) buffer[read_n] = '\0';
  close(fd);
}

typedef struct command_data {
  const char *path_a;
  char * const * argv_a;

  const char *path_b;
  char * const * argv_b;

  const char *output_file;
} command_data;

static void run_command_f(void *user_data) {
  command_data *arg = user_data;
  exit(run_command(arg->path_a, arg->argv_a));
}

static void run_redirected_f(void *user_data) {
  command_data *arg = user_data;
  exit(run_redirected(arg->path_a, arg->argv_a, arg->output_file));
}

static void run_pipe_f(void *user_data) {
  command_data *arg = user_data;
  exit(run_pipe(arg->path_a, arg->argv_a,
                arg->path_b, arg->argv_b));
}

static void run_and_f(void *user_data) {
  command_data *arg = user_data;
  exit(run_and(arg->path_a, arg->argv_a,
               arg->path_b, arg->argv_b));
}

static
int grab_command_output(const char *path, char * const argv[],
                        const char *input,
                        char *out_buffer, size_t out_n,
                        char *err_buffer, size_t err_n) {
  command_data data;
  data.path_a = path;
  data.argv_a = argv;

  return grab_output(run_command_f, &data, input,
                     out_buffer, out_n, err_buffer, err_n);
}

static
int grab_redirected_output(const char *path, char * const argv[],
                           const char *output_file,
                           const char *input,
                           char *out_buffer, size_t out_n,
                           char *err_buffer, size_t err_n) {
  command_data data;
  data.path_a = path;
  data.argv_a = argv;
  data.output_file = output_file;

  return grab_output(run_redirected_f, &data, input,
                     out_buffer, out_n, err_buffer, err_n);
}

static
int grab_pipe_output(const char *path_a, char * const argv_a[],
                     const char *path_b, char * const argv_b[],
                     const char *input,
                     char *out_buffer, size_t out_n,
                     char *err_buffer, size_t err_n) {
  command_data data;
  data.path_a = path_a;
  data.argv_a = argv_a;
  data.path_b = path_b;
  data.argv_b = argv_b;

  return grab_output(run_pipe_f, &data, input,
                     out_buffer, out_n, err_buffer, err_n);
}

static
int grab_and_output(const char *path_a, char * const argv_a[],
                    const char *path_b, char * const argv_b[],
                    const char *input,
                    char *out_buffer, size_t out_n,
                    char *err_buffer, size_t err_n) {
  command_data data;
  data.path_a = path_a;
  data.argv_a = argv_a;
  data.path_b = path_b;
  data.argv_b = argv_b;

  return grab_output(run_and_f, &data, input,
                     out_buffer, out_n, err_buffer, err_n);
}

NO_EARLY_EXIT_START_TEST(test_command) {
  char out[4096];
  char err[4096];

#define expect_command(status, input, output, error, path, ...)              \
  do {                                                                       \
    char * const argv[] = {path, __VA_ARGS__, NULL};                         \
    int code = grab_command_output(path, argv, input,                        \
                                   out, sizeof(out), err, sizeof(err));      \
    ck_assert_msg(code == status,                                            \
                  "run_command renvoie %d, le code de retour "               \
                  "aurait dû être " #status, code);                          \
    ck_assert_msg(strcmp(err, error) == 0,                                   \
                  "La sortie d’erreur de la commande aurait dû "             \
                  "être " #error " "                                         \
                  "mais votre programme écrit ceci :\n%s\n", err);           \
    ck_assert_msg(strcmp(out, output) == 0,                                  \
                  "La sortie de la commande aurait dû être " #output " "     \
                  "mais votre programme écrit ceci :\n%s\n", out);           \
  } while (0)

  check_log("Exécution de \"echo Hello World\"");
  expect_command(0,
                 "", "Hello World\n", "",
                 "/usr/bin/echo", "Hello World");

  check_log("Exécution de \"echo -n Hello World\"");
  expect_command(0,
                 "no matter\n", "Hello World", "",
                 "/usr/bin/echo", "-n", "Hello World");

  check_log("Exécution de \"wc -w data/some_file\"");
  expect_command(0,
                 "", "931 data/some_file\n", "",
                 "/usr/bin/wc", "-w", "data/some_file");

  check_log("Exécution de \"wc -l data/some_file\"");
  expect_command(0,
                 "", "19 data/some_file\n", "",
                 "/usr/bin/wc", "-l", "data/some_file");

  check_log("Exécution de \"wc -c data/some_file\"");
  expect_command(0,
                 "aba\n", "6315 data/some_file\n", "",
                 "/usr/bin/wc", "-c", "data/some_file");

  check_log("Exécution de \"wc -l");
  expect_command(0,
                 "a\nb\nc\nd", "3\n", "",
                 "/usr/bin/wc", "-l");

  check_log("Exécution de \"wc -l foo\"");
  expect_command(1,
                 "", "", "/usr/bin/wc: foo: No such file or directory\n",
                 "/usr/bin/wc", "-l", "foo");

  check_log("Exécution de \"sed s/Labor/Amor/\"");
  expect_command(0,
                 "Labor vincit omnia", "Amor vincit omnia", "",
                 "/usr/bin/sed", "s/Labor/Amor/");

  check_log("Exécution de \"sh -c 'exit 19'\"");
  expect_command(19,
                 "", "", "",
                 "/usr/bin/sh", "-c", "exit 19");
#undef expect_command
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_pipe) {
  char out[4096];
  char err[4096];

#define args(...) __VA_ARGS__
#define expect_pipe(status, input, output, error,                            \
                    path_a, args_a, path_b, args_b)                          \
  do {                                                                       \
    char * const argv_a[] = {path_a, args_a, NULL};                          \
    char * const argv_b[] = {path_b, args_b, NULL};                          \
    int code = grab_pipe_output(path_a, argv_a, path_b, argv_b,              \
                                input,                                       \
                                out, sizeof(out), err, sizeof(err));         \
    ck_assert_msg(code == status,                                            \
                  "run_command renvoie %d, le code de retour "               \
                  "aurait dû être " #status, code);                          \
    ck_assert_msg(strcmp(err, error) == 0,                                   \
                  "La sortie d’erreur de la commande aurait dû "             \
                  "être " #error " "                                         \
                  "mais votre programme écrit ceci :\n%s\n", err);           \
    ck_assert_msg(strcmp(out, output) == 0,                                  \
                  "La sortie de la commande aurait dû être " #output " "     \
                  "mais votre programme écrit ceci :\n%s\n", out);           \
  } while (0)

  check_log("Exécution de \"echo Happy plane | wc -c\"");
  expect_pipe(0,
              "foo", "11\n", "",
              "/usr/bin/echo", args("-n", "Happy plane"),
              "/usr/bin/wc", args("-c"));

  check_log("Exécution de \"sh -c 'echo a >&2' | wc -l\"");
  expect_pipe(0,
              "bar", "0\n", "a\n",
              "/usr/bin/sh", args("-c", "echo a >&2"),
              "/usr/bin/wc", args("-l"));

  check_log("Exécution de \"true nothing | sh -c 'echo b >&2'\"");
  expect_pipe(0,
              "bar", "", "b\n",
              "/usr/bin/true", args("nothing"),
              "/usr/bin/sh", args("-c", "echo b >&2"));

  check_log("Exécution de \"true nothing | sh -c 'echo b >&2'\"");
  expect_pipe(50,
              "", "", "",
              "/usr/bin/sh", args("-c", "exit 25"),
              "/usr/bin/sh", args("-c", "exit 50"));

  check_log("Exécution de \"cat - data/some_file | wc -c\"");
  expect_pipe(0,
              "Let's add this to the mix", "6340\n", "",
              "/usr/bin/cat", args("-", "data/some_file"),
              "/usr/bin/wc", args("-c"));

  check_log("Exécution de \"sed s/e/ee/g | wc -c\"");
  expect_pipe(0,
              "I love the angels\n", "21\n", "",
              "/usr/bin/sed", args("s/e/ee/g"),
              "/usr/bin/wc", args("-c"));

#undef expect_pipe
#undef args
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_and) {
  char out[4096];
  char err[4096];

#define args(...) __VA_ARGS__
#define expect_and(status, input, output, error,                             \
                   path_a, args_a, path_b, args_b)                           \
  do {                                                                       \
    char * const argv_a[] = {path_a, args_a, NULL};                          \
    char * const argv_b[] = {path_b, args_b, NULL};                          \
    int code = grab_and_output(path_a, argv_a, path_b, argv_b,               \
                               input,                                        \
                               out, sizeof(out), err, sizeof(err));          \
    ck_assert_msg(code == status,                                            \
                  "run_command renvoie %d, le code de retour "               \
                  "aurait dû être " #status, code);                          \
    ck_assert_msg(strcmp(err, error) == 0,                                   \
                  "La sortie d’erreur de la commande aurait dû "             \
                  "être " #error " "                                         \
                  "mais votre programme écrit ceci :\n%s\n", err);           \
    ck_assert_msg(strcmp(out, output) == 0,                                  \
                  "La sortie de la commande aurait dû être " #output " "     \
                  "mais votre programme écrit ceci :\n%s\n", out);           \
  } while (0)

  check_log("Exécution de \"echo a && echo b\"");
  expect_and(0,
             "", "a\nb\n", "",
             "/usr/bin/echo", args("a"),
             "/usr/bin/echo", args("b"));

  check_log("Exécution de \"echo Hello World && wc -w\"");
  expect_and(0,
             "Not Hello World", "Hello World\n3\n", "",
             "/usr/bin/echo", args("Hello World"),
             "/usr/bin/wc", args("-w"));

  check_log("Exécution de \"sh -c 'exit 123' && echo Don't execute this!\"");
  expect_and(123,
             "", "", "",
             "/usr/bin/sh", args("-c", "exit 123"),
             "/usr/bin/echo", args("Don't execute this!"));

  check_log("Exécution de \"true nothing && sh -c 'exit 70'\"");
  expect_and(70,
             "", "", "",
             "/usr/bin/true", args("nothing"),
             "/usr/bin/sh", args("-c", "exit 70"));

  check_log("Exécution de \"wc -c && wc -l\"");
  expect_and(0,
             "Some input", "10\n0\n", "",
             "/usr/bin/wc", args("-c"),
             "/usr/bin/wc", args("-l"));

  check_log("Exécution de \"wc -c no_such_file && wc -l\"");
  expect_and(1,
             "Some input", "",
             "/usr/bin/wc: no_such_file: No such file or directory\n",
             "/usr/bin/wc", args("-c", "no_such_file"),
             "/usr/bin/wc", args("-l"));

  check_log("Exécution de \"sh -c 'echo error_a >&2' "
            "&& sh -c 'echo error_b >&2'\"");
  expect_and(0,
             "Some input", "", "error_a\nerror_b\n",
             "/usr/bin/sh", args("-c", "echo error_a >&2"),
             "/usr/bin/sh", args("-c", "echo error_b >&2"));

#undef expect_and
#undef args
} NO_EARLY_EXIT_END_TEST

NO_EARLY_EXIT_START_TEST(test_redirect) {
  char file_out[4096];
  char out[4096];
  char err[4096];

#define expect_redirected(status, input, output, error, to_file, path, ...)  \
  do {                                                                       \
    char * const argv[] = {path, __VA_ARGS__, NULL};                         \
    int code = grab_redirected_output(path, argv, to_file,                   \
                                      input,                                 \
                                      out, sizeof(out), err, sizeof(err));   \
    ck_assert_msg(code == status,                                            \
                  "run_command renvoie %d, le code de retour "               \
                  "aurait dû être " #status, code);                          \
    ck_assert_msg(strcmp(err, error) == 0,                                   \
                  "La sortie d’erreur de la commande aurait dû "             \
                  "être " #error " "                                         \
                  "mais votre programme écrit ceci :\n%s\n", err);           \
    ck_assert_msg(strcmp(out, "") == 0,                                      \
                  "La sortie de la commande aurait dû être vide "            \
                  "mais votre programme écrit ceci :\n%s\n", out);           \
    read_from_file(to_file, file_out, sizeof(file_out));                     \
    ck_assert_msg(strcmp(file_out, output) == 0,                             \
                  "Le fichier de sortie de la commande aurait dû contenir "  \
                  #output "mais votre programme écrit ceci :\n%s\n",         \
                  file_out);                                                 \
  } while (0)

  check_log("Exécution de \"echo 'Hello World' > file_a\"");
  expect_redirected(0,
                    "", "Hello World\n", "",
                    "file_a", "/usr/bin/echo", "Hello World");

  check_log("Exécution de \"cat file_a > file_b\"");
  expect_redirected(0, "", "Hello World\n", "",
                    "file_b", "/usr/bin/cat", "file_a");

  check_log("Exécution de \"echo -n 'Hello World' > output_file\"");
  expect_redirected(0,
                    "no matter\n", "Hello World", "",
                     "output_file", "/usr/bin/echo", "-n", "Hello World");

  check_log("Exécution de \"wc -w data/some_file > output_file\"");
  expect_redirected(0,
                    "", "931 data/some_file\n", "",
                    "output_file", "/usr/bin/wc", "-w", "data/some_file");

  check_log("Exécution de \"wc -l data/some_file > output_file\"");
  expect_redirected(0,
                    "", "19 data/some_file\n", "",
                    "output_file", "/usr/bin/wc", "-l", "data/some_file");

  check_log("Exécution de \"wc -c data/some_file > output_file\"");
  expect_redirected(0,
                    "aba\n", "6315 data/some_file\n", "",
                    "output_file", "/usr/bin/wc", "-c", "data/some_file");

  check_log("Exécution de \"wc -l > output_file\"");
  expect_redirected(0,
                    "a\nb\nc\nd", "3\n", "",
                    "output_file", "/usr/bin/wc", "-l");

  check_log("Exécution de \"wc -l foo > output_file\"");
  expect_redirected(1,
                    "", "", "/usr/bin/wc: foo: No such file or directory\n",
                    "output_file", "/usr/bin/wc", "-l", "foo");

  check_log("Exécution de \"sed s/Labor/Amor/ > output_file\"");
  expect_redirected(0,
                    "Labor vincit omnia", "Amor vincit omnia", "",
                    "output_file", "/usr/bin/sed", "s/Labor/Amor/");

  check_log("Exécution de \"sh -c 'exit 19' > output_file\"");
  expect_redirected(19,
                    "", "", "",
                    "output_file", "/usr/bin/sh", "-c", "exit 19");

#undef expect_redirected
} NO_EARLY_EXIT_END_TEST

Suite *shell_suite(void) {
  Suite *s = suite_create("Shell");

  TCase *tc_core = tcase_create("Core");
  tcase_add_logging_test(tc_core, test_command);
  tcase_add_logging_test(tc_core, test_pipe);
  tcase_add_logging_test(tc_core, test_and);
  tcase_add_logging_test(tc_core, test_redirect);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(int argc, char **argv) {
  if (check_early_exit_setup() < 0) {
    fprintf(stderr, "Failed to setup early exit testing.\n");
    return EXIT_FAILURE;
  }

  srand(0);

  Suite *s = shell_suite();
  SRunner *sr = srunner_create(s);

  check_init_logs();
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  number_failed = check_early_exit(number_failed);
  check_print_logs();
  srunner_free(sr);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
