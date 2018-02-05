/* Tests unitaires pour l'exercice calloc2
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


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "student_code.h"

//Ces 2 variables sont à déclarer globalement car utilisées dans mymalloc.c
int nb_times_malloc_used = 0;
int let_malloc_fail = 0;

int init_suite1(void) {
    return 0;
}

int clean_suite1(void) {
    return 0;
}

// @calloc2:test_calloc2_1 => [Votre fonction calloc n'utilise pas malloc.]
void test_calloc2_1(void) {
    nb_times_malloc_used = 0; //On remet la variable à 0 car CUnit a lui-même utilisé malloc

    void *ptr = calloc2(13, 37);
    if (ptr == NULL) {
        CU_FAIL("Erreur lors l'allocation de la mémoire.");
        return;
    }

    // nb_times_malloc_used sera incrémenté par le malloc modifié de mymalloc.c
    CU_ASSERT_EQUAL(nb_times_malloc_used, 1);

    free(ptr);
}

// @calloc2:test_calloc2_2 => [Votre fonction calloc n'a pas initialisé la mémoire allouée à 0.]
void test_calloc2_2(void) {
    void *ptr = calloc2(42, 1);
    if (ptr == NULL) {
        CU_FAIL("Erreur lors l'allocation de la mémoire.");
        return;
    }

    CU_ASSERT_EQUAL(*(char*)(ptr+10), 0);

    free(ptr);
}

jmp_buf label_test_calloc2_3;

void sig_handler(int signo) {
    //Le signal handler a été exécuté, on a donc reçu un SIGSEGV
    //On provoque alors un jump vers le label avec le paramètre 1
    longjmp(label_test_calloc2_3, 1);
}

// @calloc2:test_calloc2_3 => [Votre fonction calloc ne prend pas en compte le fait que malloc puisse échouer.]
void test_calloc2_3(void) {
    let_malloc_fail = 1; //On indique qu'on veut que malloc utilisé par calloc2 échoue


    // Pour plus de détails sur le fonctionnement des signaux et l'interception des segfaults,
    // voir mini-projet-string/tests.c
    if (signal(SIGSEGV, sig_handler) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;
    }

    if(setjmp(label_test_calloc2_3)==0) {
        calloc2(42, 42);
    }
    else {
        /* IMPORTANT ! On remet let_malloc_fail à 0 pour que CUnit puisse réutiliser malloc par la suite.
         * Ici, si on ne pense pas à remettre cette variable à 0, CUnit ne prend pas en compte l'échec du test. */
        let_malloc_fail = 0; 
        CU_ASSERT_TRUE(0);
    }
    
    /* IMPORTANT ! On remet let_malloc_fail à 0 pour que CUnit puisse réutiliser malloc par la suite. */
    let_malloc_fail = 0; 
    signal(SIGSEGV, SIG_DFL);
}


int main() {
    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour calloc2", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test_calloc2_1", test_calloc2_1) ||
       NULL == CU_add_test(pSuite, "test_calloc2_2", test_calloc2_2) ||
       NULL == CU_add_test(pSuite, "test_calloc2_3", test_calloc2_3)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
