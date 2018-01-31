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

#include "student_code.h"

unsigned char get_bit(unsigned int x, unsigned int pos)
{

	unsigned int bitShift = x >> (pos-1);
	unsigned char bitGet = (unsigned char) bitShift & 1;
	
	/* la ligne suivante fait échouer le test "test_if_get_bit_return_zero" 
	 * ainsi que les deux tests des fonctions qui utilisent get_bit dans
	 * leur implémentation : "test_unset_first_bit" et "test_cycle_bits"
	 */
	//return 1;
	
	/* la ligne suivante fait échouer le test "test_if_get_bit_return_one" 
	 * ainsi que les deux tests des fonctions qui utilisent get_bit dans
	 * leur implémentation : "test_unset_first_bit" et "test_cycle_bits"
	 */
	 //return 0;
	
	return bitGet;
	
}

unsigned int set_bit(unsigned int x, unsigned int pos, unsigned char value)
{
	
	unsigned int mask;
	unsigned int bitSet;
	
	if(value == 1)
	{
		mask = 1 << (pos-1);
		bitSet = x | mask;
		/* la ligne suivante fait échouer le test "test_set_bit_to_one" et
		 * le test de la fonction qui utilise set_bit pour mettre un bit
		 * à un : "test_cycle_bits"
		 */
		//bitSet = x & mask;
	}
	else
	{
		mask = ~(1 << (pos-1));
		bitSet = x & mask;
		/* la ligne suivante fait échouer le test "test_set_bit_to_zero" et
		 * les tests des deux fonctions qui utilisent set_bit pour mettre 
		 * un bit à zéro : "test_unset_first_bit" et "test_cycle_bits"
		 */
		//bitSet = x | mask;
	}
	
	return bitSet;
	
}

unsigned char get_3_leftmost_bits(unsigned int x)
{

	unsigned int bitShift = x >> 29;
	unsigned char left3bits = (unsigned char) bitShift;
	
	// la ligne suivante fait échouer le test "test_get_3_leftmost_bits"
	// return left3bits >> 1;
	return left3bits;
	
}

unsigned char get_4_rightmost_bits(unsigned int x)
{
	
	unsigned int mask = 0b1111;
	unsigned int bitShift = x & mask;
	unsigned char right4bits = (unsigned char) bitShift;
	
	// la ligne suivante fait échouer le test "test_get_4_rightmost_bits"
	//return right4bits << 1;
	return right4bits;
	
}

unsigned int unset_first_bit(unsigned int x)
{

	unsigned int unsetBit = x;
	
	for(int i=32; i>0;i--)
	{
		// le morceau de code suivant fait échouer le test "test_unset_first_bit"
		/* if(get_bit(x,i+1) == 1)
		 * {
		 *	   unsetBit = set_bit(x,i,0);
		 *	
		 *     return unsetBit;
		 * }
		 */
		if(get_bit(x,i) == 1)
		{
			unsetBit = set_bit(x,i,0);
			
			return unsetBit;
		}
	}

	return unsetBit;

}

unsigned int cycle_bits(unsigned int x, unsigned int n)
{

	unsigned int bitCycle = x;
	
	for(int i=1;i<=32;i++)
	{
		unsigned int position = (i+n)%32;
		unsigned int bitGet = (unsigned int) get_bit(x,i);
 		bitCycle = set_bit(bitCycle,position,bitGet);
	}
	
	// la ligne suivante permet de faire échouer le test "test_cycle_bits"
	//return ~bitCycle;
	return bitCycle;
	
}
