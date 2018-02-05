#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
 * @pre prog!=NULL, arg!=NULL
 * @post execute le programme prog avec les arguments arg et retourne: 
 * -1 si le programme n'est pas executable
 * 0 si il s'execute correctement
 * -2 si il s'execute et est termine par la reception d'un signal
 */
int run(char *prog, char *arg[]) {
@@fork@@

}

void montest(void) {

@@montest@@

}
