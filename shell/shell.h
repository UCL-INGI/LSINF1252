#ifndef SHELL_H_
#define SHELL_H_

int run_command(const char *path, char * const argv[]);
int run_pipe(const char *path_a, char * const argv_a[],
             const char *path_b, char * const argv_b[]);
int run_and(const char *path_a, char * const argv_a[],
            const char *path_b, char * const argv_b[]);
int run_redirected(const char *path, char * const argv[],
                   const char *output_path);

#endif
