#include"student_code.h"

uint8_t get_4_rightmost_bits(uint32_t x)
{
	uint32_t mask = 0b1111;
	uint32_t bitShift = x & mask;
	uint8_t right4bits = (uint8_t) bitShift;
	
	// la ligne suivante fait échouer le test "test_get_4_rightmost_bits"
	//return right4bits << 1;
	return right4bits;
}
