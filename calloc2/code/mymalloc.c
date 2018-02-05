/* Tests unitaires pour le mini-projet-string
Copyright (C) 2016 Mathieu Xhonneux

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


void *malloc(size_t size) {
    // Ces 2 variables sont déclarées, initialisées et utilisées dans tests.c
    // Afin de pouvoir y accéder, on y ajoute le mot-clé extern
    extern int nb_times_malloc_used;
    extern int let_malloc_fail;

    nb_times_malloc_used++; //On enregistre le nombre d'utilisations de malloc

    if (let_malloc_fail) //Si let_malloc_fail != 0, on impose que malloc échoue
        return NULL;

    // On déclare un pointeur vers une fonction qui a le même prototype
    void *(*original_malloc) (size_t size);
    // On crée une copie du pointeur vers la fonction malloc originale
    original_malloc = dlsym(RTLD_NEXT, "malloc");

    // On exécute la fonction malloc originale, car exécuter malloc(size)
    // reviendrait à faire un appel récursif infini ...
    void *ptr = original_malloc(size);

    if (ptr == NULL) // Dans le cas où malloc aurait réellement échoué
        return NULL;

    // On impose que le bloc mémoire n'ait pas déjà des valeurs nulles
    memset(ptr, 42, size);

    return ptr;
}


