#include "shell.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int run_command(const char *path, char * const argv[]) {
#include "run_command.h"
}

int run_pipe(const char *path_a, char * const argv_a[],
             const char *path_b, char * const argv_b[]) {
#include "run_pipe.h"
}

int run_and(const char *path_a, char * const argv_a[],
            const char *path_b, char * const argv_b[]) {
#include "run_and.h"
}

int run_redirected(const char *path, char * const argv[],
                   const char *output_path) {
#include "run_redirected.h"
}
