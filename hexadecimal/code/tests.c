/* Tests unitaires pour gcd
Copyright (C) 2016 Simon Hick, Sébastien Strebelle

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

char *hex = NULL;
unsigned int n = 0;

// @int_to_hex:test_hex_0 => [La fonction ne gère pas l'entier nul]
void test_hex_0(void) {
	int_to_hex(0, hex);
	CU_ASSERT_STRING_EQUAL(hex, "0");
}

// @hex_to_int:test_int_0 => [La fonction ne gère pas le nombre hexadécimal nul]
void test_int_0(void) {
	n = hex_to_int("0");
	CU_ASSERT_EQUAL(n, 0);
}

// @int_to_hex:test_hex_normal => [La fonction ne renvoie pas les bonnes valeurs (42->2A, 19->13, 408->198)]
void test_hex_normal(void) {
	int_to_hex(42, hex);
	CU_ASSERT_STRING_EQUAL(hex, "2A");
	int_to_hex(19, hex);
	CU_ASSERT_STRING_EQUAL(hex, "13");
	int_to_hex(408, hex);
	CU_ASSERT_STRING_EQUAL(hex, "198");
}

// @hex_to_int:test_int_normal => [La fonction ne renvoie pas les bonnes valeurs (2A->42, 13->19, 198->408)]
void test_int_normal(void) {
	n = hex_to_int("2A");
	CU_ASSERT_EQUAL(n, 42);
	n = hex_to_int("13");
	CU_ASSERT_EQUAL(n, 19);
	n = hex_to_int("198");
	CU_ASSERT_EQUAL(n, 408);
}

// @int_to_hex:test_hex_bigvalue => [La fonction ne gère pas l'entier non signé le plus grand]
void test_hex_bigvalue(void) {
	unsigned int big = -1;
	int_to_hex(big, hex);
	CU_ASSERT_STRING_EQUAL(hex, "FFFFFFFF");
}

// @hex_to_int:test_int_bigvalue => [La fonction ne gère pas l'entier signé le plus grand]
void test_int_bigvalue(void) {
	unsigned int big = -1;
	n = hex_to_int("FFFFFFFF");
	CU_ASSERT_EQUAL(n, big);
}

// @int_to_hex:test_hex_0_inside => [La fonction ne gère pas les nombres hexadécimaux contenant des 0]
void test_hex_0_inside(void) {
	int_to_hex(1037, hex);
	CU_ASSERT_STRING_EQUAL(hex, "40D");
}

// @hex_to_int:test_int_0_inside => [La fonction ne gère pas les nombres hexadécimaux contenant des 0]
void test_int_0_inside(void) {
	n = hex_to_int("40D");
	CU_ASSERT_EQUAL(n, 1037);
}

// @int_to_hex:test_hex_crit => [La fonction ne gère pas les valeurs critiques (1->1, 16->10, 17->11)]
void test_hex_crit(void) {
	int_to_hex(1, hex);
	CU_ASSERT_STRING_EQUAL(hex, "1");
	int_to_hex(16, hex);
	CU_ASSERT_STRING_EQUAL(hex, "10");
	int_to_hex(17, hex);
	CU_ASSERT_STRING_EQUAL(hex, "11");
}

// @hex_to_int:test_int_crit => [La fonction ne gère pas les valeurs critiques (1->1, 10->16, 11->17)]
void test_int_crit(void) {
	n = hex_to_int("1");
	CU_ASSERT_EQUAL(n, 1);
	n = hex_to_int("10");
	CU_ASSERT_EQUAL(n, 16);
	n = hex_to_int("11");
	CU_ASSERT_EQUAL(n, 17);
}

//@int_to_hex:test_return_pointer => [int_to_hex doit retourner la même adresse que celle donnee à l'argument dest.]
void test_return_pointer(void)
{
	char* hex2 = NULL;
	hex2 = int_to_hex(1000, hex);
	CU_ASSERT_PTR_EQUAL(hex, hex2);
}


//Fonction d'initialisation
int setup(void) {
	hex = malloc(9*sizeof(char));
	if (hex != NULL)
		return 0;
	else
		return -1;
}

//Fonction de fin de tests
int teardown(void) {
	free(hex);
	return 0;
}

int main(int argc, char const *argv[]) {
	//Initialisation des tests
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	CU_pSuite pSuite = NULL;
	CU_pSuite pSuite2 = NULL;

	//Première suite de tests : fonction hex_to_int
	pSuite = CU_add_suite("int_to_hex", setup, teardown);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	//Ajout des tests dans la première suite
	if (NULL == CU_add_test(pSuite, "test_hex_0", test_hex_0) ||
		NULL == CU_add_test(pSuite, "test_hex_normal", test_hex_normal) ||
		NULL == CU_add_test(pSuite, "test_hex_bigvalue", test_hex_bigvalue) ||
		NULL == CU_add_test(pSuite, "test_hex_0_inside", test_hex_0_inside) ||
		NULL == CU_add_test(pSuite, "test_return_pointer", test_return_pointer) ||
		NULL == CU_add_test(pSuite, "test_hex_crit", test_hex_crit)
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	//Deuxième suite de tests : fonction int_to_hex
	pSuite2 = CU_add_suite("hex_to_int", setup, teardown);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	//Ajout des tests dans la deuxième suite
	if (NULL == CU_add_test(pSuite2, "test_int_0", test_int_0) ||
		NULL == CU_add_test(pSuite2, "test_int_normal", test_int_normal) ||
		NULL == CU_add_test(pSuite2, "test_int_bigvalue", test_int_bigvalue) ||
		NULL == CU_add_test(pSuite2, "test_int_0_inside", test_int_0_inside) ||
		NULL == CU_add_test(pSuite2, "test_int_crit", test_int_crit)
	) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	//Lancement des tests
	CU_automated_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
