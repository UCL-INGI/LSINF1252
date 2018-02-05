#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <fcntl.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 

/*
 * @pre filename!=NULL
 * @post Modifie le contenu du fichier filename en echangeant :
 *  - le premier byte du fichier avec le dernier
 *  - le deuxieme byte avec l'avant dernier
 *  - ...
 * Apres modification, le fichier est ferme.
 * retourne 0 en cas de succes, -1 en cas d'erreur.
 * L'implementation ne pas utiliser read, write, fread, fwrite, fgetc, fgets, ... mmap est obligatoire.
 */ 
int reverse(char *filename) {

@@reverse@@

}

void montest(void) {

@@montest@@

}