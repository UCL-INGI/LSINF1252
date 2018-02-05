/* Tests unitaires pour gcd
Copyright (C) 2016 Kevin Stoffler, Mathieu Xhonneux

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


#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "student_code.h"
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

//Ces 2 variables sont à déclarer globalement car utilisées dans mymalloc.c
int nb_times_malloc_used = 0;
int let_malloc_fail = 0;
int nb_times_free_used = 0;
int nb_times_thread_create=0;

// @factorize:test_factorize => [La fonction factorize ne donne pas les bons diviseurs dans l'ordre décroissant du nombre 25.]
void test_factorize() {
    int vals[] = {25, 5, 1};
    unsigned int k = 25;

    struct Node *curr = (struct Node*) factorize((void *)&k);
    struct Node *next;
    int nbOK = 0;
    while(curr != NULL) {
        if (curr->divisor == vals[nbOK])
            nbOK++;

        next = curr->next;
        free(curr);
        curr = next;
    }

    CU_ASSERT_EQUAL(nbOK, 3);
}
            
// @gcd:test_same_number => [La fonction gcd ne renvoie pas x pour gcd(x,x).]
void test_same_number() {
	CU_ASSERT_EQUAL(gcd(25, 25), 25);
}

// @gcd:test_nothing_common => [La fonction gcd ne renvoie pas 1 pour deux sans diviseurs communs.]
void test_nothing_common() {
	CU_ASSERT_EQUAL(gcd(13, 42), 1);
}

// @gcd:test_gcd_ok => [La fonction gcd ne renvoie pas 100 comme PGCD pour 500 et 600.]
void test_gcd_ok(){
	CU_ASSERT_EQUAL(gcd(500, 600), 100);
	CU_ASSERT_EQUAL(gcd(600, 500), 100);
}

// @gcd:test_malloc_equals_free => [Vous devez faire un appel à malloc et à free par diviseur trouvé.]
void test_malloc_equals_free(void) {
    nb_times_malloc_used = 0; 
	nb_times_free_used = 0;
    gcd(3, 9);
    
    CU_ASSERT_EQUAL(nb_times_malloc_used, 5); //malloc de 5 noeuds [3,1] et [9, 3, 1]
    CU_ASSERT_TRUE(nb_times_free_used == 73); //car pthread utilise free, dépend probablement de glibc, risque peut-être de casser un jour ...
}

// @gcd:test_nb_threads => [La fonction gcd n'a pas lancé 2 threads.]
void test_nb_threads(void) {
    nb_times_thread_create = 0; 
    gcd(24510250, 124250);
        
    CU_ASSERT_EQUAL(nb_times_thread_create, 2); 
}

jmp_buf label_test_malloc_fail;

void sig_handler(int signo) {
    //Le signal handler a été exécuté, on a donc reçu un SIGSEGV
    //On provoque alors un jump vers le label avec le paramètre 1
    longjmp(label_test_malloc_fail,1);
}

// @factorize:test_malloc_fail => [Votre programme segfault si malloc échoue.]
void test_malloc_fail(void) {
    let_malloc_fail = 1; //On indique qu'on veut que malloc utilisé par gcd échoue


    if (signal(SIGSEGV, sig_handler) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;
    }

    if(setjmp(label_test_malloc_fail)==0) {
        gcd(1525,502521);
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

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("gcd_suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test_same_number", test_same_number)) ||
        (NULL == CU_add_test(pSuite, "test_nothing_common", test_nothing_common)) ||
        (NULL == CU_add_test(pSuite, "test_factorize", test_factorize)) ||
        (NULL == CU_add_test(pSuite, "test_gcd_ok", test_gcd_ok)) ||
        (NULL == CU_add_test(pSuite, "test_malloc_fail", test_malloc_fail)) ||
        (NULL == CU_add_test(pSuite, "test_malloc_equals_free", test_malloc_equals_free)) ||
        (NULL == CU_add_test(pSuite, "test_nb_threads", test_nb_threads)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
