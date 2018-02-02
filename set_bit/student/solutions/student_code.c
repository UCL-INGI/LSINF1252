#include"student_code.h"

uint64_t set_bit(uint64_t x, int pos, bool value)
{
	uint64_t mask;
	uint64_t bitSet;

	if(value == 1){
		mask = 1 << (pos-1);
		bitSet = x | mask;
		/* la ligne suivante fait échouer le test "test_set_bit_to_one" et
		 * le test de la fonction qui utilise set_bit pour mettre un bit
		 * à un : "test_cycle_bits"
		 */
		//bitSet = x & mask;
	}
	else{
		mask = ~(1 << (pos-1));
		bitSet = x & mask;
		/* la ligne suivante fait échouer le test "test_set_bit_to_zero" et
		 * les tests des deux fonctions qui utilisent set_bit pour mettre 
		 * un bit à zéro : "test_unset_strong_bit" et "test_cycle_bits"
		 */
		//bitSet = x | mask;
	}

	return bitSet;
}
