#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * @pre prog!=NULL, argv!=NULL, file!=NULL
 * execute le programme prog avec les arguments argv
 * en redirigeant la sortie standard et l'erreur standard
 * vers le fichier file. Retourne la valeur de retour
 * du programme et -1 en cas d'erreur d'exécution du programme
 * ou d'accès au fichier
 */
int run_redir(char *prog, char* argv[], char *file) {
@@run_redir@@

}

void montest(void) {

@@contest@@

}
