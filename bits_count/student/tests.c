/* Tests unitaires pour bitwise-ops

   Adapté en 2018 par Arthur van Stratum
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

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "CTester/CTester.h"
#include "student_code.h"

/* TESTS DE LA METHODE nbits*/

void test_nbits(void)
{
	int ret1 = -1;
	int ret2 = -1;
	int ret3 = -1;
	int ret4 = -1;
	SANDBOX_BEGIN;
	ret1 = nbits(0b1110110100);
	ret2 = nbits(0b0000010000);
	ret3 = nbits(0b1000000000000000);
	ret4 = nbits(0b0000000000000000);
	SANDBOX_END;
	CU_ASSERT_EQUAL(6, ret1);
	CU_ASSERT_EQUAL(1, ret2);
	CU_ASSERT_EQUAL(1, ret3);
	CU_ASSERT_EQUAL(0, ret4);
	if ( 6!= ret1||1!= ret2 ||1!= ret3 ||0!= ret4)
		push_info_msg(_("nbits returns the wrong numbers of bits"));
}
/* TESTS DE LA METHODE set_bit */

// @set_bit:test_set_bit_to_zero => [La fonction set_bit ne modifie pas la valeur d'un bit dont la position est passée en argument à 0]
void test_set_bit_to_zero(void)
{
	uint64_t binaryNumber = 0b10110010;
	uint64_t binaryNumberReturned;
	SANDBOX_BEGIN;
	binaryNumberReturned = set_bit(binaryNumber,2,0);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10110000);
	if ( binaryNumberReturned!=0b10110000)
		push_info_msg(_("set_bit returns the wrong value"));
}

// @set_bit:test_set_bit_to_one => [La fonction set_bit ne modifie pas la valeur d'un bit dont la position est passée en argument à 1]
void test_set_bit_to_one(void)
{
	uint64_t binaryNumber = 0b10110010;
	uint64_t binaryNumberReturned;
	SANDBOX_BEGIN;
	binaryNumberReturned = set_bit(binaryNumber,7,1);
	SANDBOX_END;

	CU_ASSERT_EQUAL(binaryNumberReturned,0b11110010);
	if ( binaryNumberReturned!=0b11110010)
		push_info_msg(_("set_bit returns the wrong value"));
}

/* TESTS DE LA METHODE get_3_leftmost_bits */

// @get_3_leftmost_bits:test_get_3_leftmost_bits => [La fonction get_3_leftmost_bits ne retourne pas les 3 bits les plus à gauche d'une séquence passée en argument]
void test_get_3_leftmost_bits(void)
{
	uint32_t binaryNumber = 0b01000000000010101111011101101111;
	uint8_t leftMostBitsReturned = get_3_leftmost_bits(binaryNumber);

	SANDBOX_BEGIN;
	SANDBOX_END;
	CU_ASSERT_EQUAL(leftMostBitsReturned,0b010);
	if ( leftMostBitsReturned!=0b010)
		push_info_msg(_("get_3_leftmost_bits returns the wrong value"));
}

/* TESTS DE LA METHODE get_4_rightmost_bits */

// @get_4_rightmost_bits:test_get_4_rightmost_bits => [La fonction get_4_rigthmost_bits ne retourne pas les 4 bits les plus à droite d'une séquence passée en argument]
void test_get_4_rightmost_bits(void)
{
	uint32_t binaryNumber = 0b0111010110110101010001011;
	uint8_t rightMostBitsReturned;

	SANDBOX_BEGIN;
	rightMostBitsReturned = get_4_rightmost_bits(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(rightMostBitsReturned,0b1011);
	if ( rightMostBitsReturned!=0b1011)
		push_info_msg(_("get_4_rightmost_bits returns the wrong value"));
}

/* TESTS DE LA METHODE unset_strong_bit */

// @unset_strong_bit:test_unset_strong_bit => [La fonction unset_strong_bit ne ne met pas le premier bit trouvé dans une séquence passée en argument à 0]
void test_unset_strong_bit(void)
{
	uint32_t binaryNumber = 0b1011011101;
	uint32_t binaryNumberReturned;

	SANDBOX_BEGIN;
	binaryNumberReturned = unset_strong_bit(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11011101);
	if ( binaryNumberReturned!=0b11011101)
		push_info_msg(_("unset_strong_bits returns the wrong value"));
}

/* TESTS DE LA METHODE cycle_bits */

// @cycle_bits:test_cycle_bits => [La fonction cycle_bits ne retourne pas la bonne séquence de bits]
void test_cycle_bits(void)
{
	uint32_t binaryNumber = 0b10110110101010001101111111111111;
	uint32_t binaryNumberReturned;

	SANDBOX_BEGIN;
	binaryNumberReturned = cycle_bits(binaryNumber,6);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10101010001101111111111111101101);
	if ( binaryNumberReturned!=0b10101010001101111111111111101101)
		push_info_msg(_("cycle_bits returns the wrong value"));
}

/* LANCEMENT DES TESTS UNITAIRES */

void launch()
{
	test_nbits();
	test_set_bit_to_zero();
	test_set_bit_to_one();
	test_get_3_leftmost_bits();
	test_get_4_rightmost_bits();
	test_unset_strong_bit();
	test_cycle_bits();
}

int main(int argc, char** argv)
{
	RUN(launch);
}
