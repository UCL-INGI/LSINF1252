#include "student_code.h"

uint8_t get_3_leftmost_bits(uint32_t x)
{

	uint32_t bitShift = x >> 29;
	uint8_t left3bits = (uint8_t) bitShift;
	
	// la ligne suivante fait échouer le test "test_get_3_leftmost_bits"
	// return left3bits >> 1;
	return left3bits;
	
}
