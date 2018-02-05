/* Tests unitaires pour matrix-mult

Copyright (C) 2016 Adrien Ballet

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

#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "student_code.h"

int A[4][4] = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };
int B[4][4] = { {17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32} };
int *C = NULL;

int init_suite1(void) {
    C = malloc(sizeof(int)*16);

    if (C == NULL) {
        return -1;
    }

    return 0;
}

int clean_suite1(void) {
    free(C);
    return 0;
}

// @access:test_access => [Quand on utilise la fonction access sur la matrice { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} }, le mauvais élément est renvoyé.]
void test_access(void)
{
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 1, 1), 1);
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 1, 4), 4);
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 2, 1), 5);
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 3, 4), 12);
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 3, 2), 10);
    CU_ASSERT_EQUAL(access(&A[0][0], 4, 4, 4), 16);
}

// @mult:test_mult => [La multiplication de { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} } par { {17,18,19,20}, {21,22,23,24}, {25,26,27,28}, {29,30,31,32} } ne renvoie pas la bonne matrice.]
void test_mult(void)
{
    mult(&A[0][0], &B[0][0], C, 4);
    CU_ASSERT_EQUAL(access(C, 4, 1, 1), 250);
    CU_ASSERT_EQUAL(access(C, 4, 4, 1), 1354);
    CU_ASSERT_EQUAL(access(C, 4, 2, 3), 670);
    CU_ASSERT_EQUAL(access(C, 4, 3, 4), 1112);
    CU_ASSERT_EQUAL(access(C, 4, 4, 4), 1528);
    CU_ASSERT_EQUAL(access(C, 4, 2, 1), 618);
}

int main(int argc, char const *argv[]) {

    CU_pSuite pSuite = NULL;

    // Initialisation de la suite de tests
    if(CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
    }

    // Ajout de la suite au catalogue
    pSuite = CU_add_suite("Suite de tests pour matrix-mult", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Ajout des tests à la suite
    if(NULL == CU_add_test(pSuite, "test_access", test_access) ||
    NULL == CU_add_test(pSuite, "test_mult", test_mult)
    ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Exécution des tests et vidage de la mémoire
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
