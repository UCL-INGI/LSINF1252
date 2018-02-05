/* Tests unitaires pour bitwise-ops

Copyright (C) 2016  Maxime Wattiaux
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
    
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "student_code.h"

/* FONCTIONS D'INITIALISATION */

int setup(void)
{
		return 0;
}

int teardown(void)
{
	return 0;
}

/* TESTS DE LA METHODE get_bit */

// @get_bit:test_if_get_bit_return_zero => [La fonction get_bit ne retourne pas 0 lorsque la valeur du bit recherchée est celle-ci]
void test_if_get_bit_return_zero(void)
{
	unsigned int binaryNumber = 0b10110010;
	unsigned char bitReturned = get_bit(binaryNumber,6);
	
	CU_ASSERT_EQUAL(bitReturned,0);
}

// @get_bit:test_if_get_bit_return_one => [La fonction get_bit ne retourne pas 1 lorsque la valeur du bit recherchée est celle-ci]
void test_if_get_bit_return_one(void)
{
	unsigned int binaryNumber = 0b10110110;
	unsigned char bitReturned = get_bit(binaryNumber,2);
	
	CU_ASSERT_EQUAL(bitReturned,1);
}

/* TESTS DE LA METHODE set_bit */

// @set_bit:test_set_bit_to_zero => [La fonction set_bit ne modifie pas la valeur d'un bit dont la position est passée en argument à 0]
void test_set_bit_to_zero(void)
{
	unsigned int binaryNumber = 0b10110010;
	unsigned int binaryNumberReturned = set_bit(binaryNumber,1,0);
	
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10110000);
}

// @set_bit:test_set_bit_to_one => [La fonction set_bit ne modifie pas la valeur d'un bit dont la position est passée en argument à 1]
void test_set_bit_to_one(void)
{
	unsigned int binaryNumber = 0b10110010;
	unsigned int binaryNumberReturned = set_bit(binaryNumber,6,1);
	
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11110010);
}

/* TESTS DE LA METHODE get_3_leftmost_bits */

// @get_3_leftmost_bits:test_get_3_leftmost_bits => [La fonction get_3_leftmost_bits ne retourne pas les 3 bits les plus à gauche d'une séquence passée en argument]
void test_get_3_leftmost_bits(void)
{
	unsigned int binaryNumber = 0b01000000000010101111011101101111;
	unsigned char leftMostBitsReturned = get_3_leftmost_bits(binaryNumber);
	
	CU_ASSERT_EQUAL(leftMostBitsReturned,0b010);
}

/* TESTS DE LA METHODE get_4_rightmost_bits */

// @get_4_rightmost_bits:test_get_4_rightmost_bits => [La fonction get_4_rigthmost_bits ne retourne pas les 4 bits les plus à droite d'une séquence passée en argument]
void test_get_4_rightmost_bits(void)
{
	unsigned int binaryNumber = 0b0111010110110101010001011;
	unsigned char rightMostBitsReturned = get_4_rightmost_bits(binaryNumber);
	
	CU_ASSERT_EQUAL(rightMostBitsReturned,0b1011);
}

/* TESTS DE LA METHODE unset_last_bit */

// @unset_last_bit:test_unset_last_bit => [La fonction unset_last_bit ne ne met pas le premier bit trouvé dans une séquence passée en argument à 0]
void test_unset_last_bit(void)
{
	unsigned int binaryNumber = 0b1011011101;
	unsigned int binaryNumberReturned = unset_last_bit(binaryNumber);
	
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11011101)
}

/* TESTS DE LA METHODE cycle_bits */

// @cycle_bits:test_cycle_bits => [La fonction cycle_bits ne retourne pas la bonne séquence de bits pour un entier de 32 bits]
void test_cycle_bits(void)
{
	unsigned int binaryNumber = 0b10110110101010001101111111111111;
	unsigned int binaryNumberReturned = cycle_bits(binaryNumber,6);
	
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10101010001101111111111111101101);
}

// @cycle_bits:test_cycle_bits2 => [La fonction cycle_bits ne retourne pas la bonne séquence de bits pour le nombre 10]
void test_cycle_bits2(void)
{
	unsigned int binaryNumber = 0b1010;
	unsigned int binaryNumberReturned = cycle_bits(binaryNumber,4);
	
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10100000);
}


/* LANCEMENT DES TESTS UNITAIRES */

int main(int argc, char* argv[])
{
	
	CU_pSuite pSuite = NULL;

	/* Initialise le catalogue de suites de test */
	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();	
	}
	
	/* Ajoute la suite de test "test_bitwise_ops" au catalogue */
	pSuite = CU_add_suite("test_bitwise_ops", setup, teardown);
	if (pSuite == NULL)
	{	
		CU_cleanup_registry();
		return CU_get_error();	
	}
	
	/* Ajoute les tests à la suite de test "test_bitwise_ops" */
	if ( (NULL == CU_add_test(pSuite, "test_if_get_bit_return_zero", test_if_get_bit_return_zero)) ||
		 (NULL == CU_add_test(pSuite, "test_if_get_bit_return_one", test_if_get_bit_return_one)) ||
		 (NULL == CU_add_test(pSuite, "test_set_bit_to_zero", test_set_bit_to_zero)) ||
		 (NULL == CU_add_test(pSuite, "test_set_bit_to_one", test_set_bit_to_one)) ||
		 (NULL == CU_add_test(pSuite, "test_get_3_leftmost_bits", test_get_3_leftmost_bits)) ||
		 (NULL == CU_add_test(pSuite, "test_get_4_rightmost_bits", test_get_4_rightmost_bits)) ||
		 (NULL == CU_add_test(pSuite, "test_unset_last_bit", test_unset_last_bit)) ||
		 (NULL == CU_add_test(pSuite, "test_cycle_bits2", test_cycle_bits2)) ||
		 (NULL == CU_add_test(pSuite, "test_cycle_bits", test_cycle_bits)) )
	{  
      CU_cleanup_registry();
      return CU_get_error();  
	}
    
    /* Exécution des tests, affichage des tests échoués et vidage de la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
   
}
