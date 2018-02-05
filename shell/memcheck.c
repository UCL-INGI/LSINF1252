#include "shell.h"
#include <stdlib.h>

int main(int argc, char **argv) {

#define args(...) __VA_ARGS__

#define test_command(path, ...)                         \
  do {                                                  \
    char * const argv[] = {path, __VA_ARGS__, NULL};    \
    run_command(path, argv);                            \
  } while (0)

#define test_pipe(path_a, args_a, path_b, args_b)         \
  do {                                                    \
    char * const argv_a[] = {path_a, args_a, NULL};       \
    char * const argv_b[] = {path_b, args_b, NULL};       \
    run_pipe(path_a, argv_a, path_b, argv_b);             \
  } while (0)

#define test_and(path_a, args_a, path_b, args_b)          \
  do {                                                    \
    char * const argv_a[] = {path_a, args_a, NULL};       \
    char * const argv_b[] = {path_b, args_b, NULL};       \
    run_and(path_a, argv_a, path_b, argv_b);              \
  } while (0)

#define test_redirected(to_file, path, ...)                \
  do {                                                     \
    char * const argv[] = {path, __VA_ARGS__, NULL};       \
    run_redirected(path, argv, to_file);                   \
  } while (0)

  test_command("/usr/bin/echo", "Hello World");
  test_command("/usr/bin/echo", "-n", "Hello World");
  test_command("/usr/bin/wc", "-w", "data/some_file");
  test_command("/usr/bin/wc", "-l", "data/some_file");
  test_command("/usr/bin/wc", "-c", "data/some_file");
  test_command("/usr/bin/wc", "-c");
  test_command("/usr/bin/wc", "-l", "foo");
  test_command("/usr/bin/sed", "s/Labor/Amor/");
  test_command("/usr/bin/sh", "-c", "exit 19");

  test_pipe("/usr/bin/echo", args("-n", "Happy plane"),
            "/usr/bin/wc", args("-c"));
  test_pipe("/usr/bin/sh", args("-c", "echo a >&2"),
            "/usr/bin/wc", args("-l"));
  test_pipe("/usr/bin/true", args("nothing"),
            "/usr/bin/sh", args("-c", "echo b >&2"));
  test_pipe("/usr/bin/sh", args("-c", "exit 25"),
            "/usr/bin/sh", args("-c", "exit 50"));
  test_pipe("/usr/bin/cat", args("-", "data/some_file"),
            "/usr/bin/wc", args("-c"));
  test_pipe("/usr/bin/sed", args("s/e/ee/g"),
            "/usr/bin/wc", args("-c"));

  test_and("/usr/bin/echo", args("a"),
           "/usr/bin/echo", args("b"));
  test_and("/usr/bin/echo", args("Hello World"),
           "/usr/bin/wc", args("-w"));
  test_and("/usr/bin/sh", args("-c", "exit 123"),
           "/usr/bin/echo", args("Don't execute this!"));
  test_and("/usr/bin/true", args("nothing"),
           "/usr/bin/sh", args("-c", "exit 70"));
  test_and("/usr/bin/wc", args("-c"),
           "/usr/bin/wc", args("-l"));
  test_and("/usr/bin/wc", args("-c", "no_such_file"),
           "/usr/bin/wc", args("-l"));
  test_and("/usr/bin/sh", args("-c", "echo error_a >&2"),
           "/usr/bin/sh", args("-c", "echo error_b >&2"));

  test_redirected("file_a", "/usr/bin/echo", "Hello World");
  test_redirected("file_b", "/usr/bin/cat", "file_a");
  test_redirected("output_file", "/usr/bin/echo", "-n", "Hello World");
  test_redirected("output_file", "/usr/bin/wc", "-w", "data/some_file");
  test_redirected("output_file", "/usr/bin/wc", "-l", "data/some_file");
  test_redirected("output_file", "/usr/bin/wc", "-c", "data/some_file");
  test_redirected("output_file", "/usr/bin/wc", "-c");
  test_redirected("output_file", "/usr/bin/wc", "-l", "foo");
  test_redirected("output_file", "/usr/bin/sed", "s/Labor/Amor/");
  test_redirected("output_file", "/usr/bin/sh", "-c", "exit 19");

  return 0;
}
