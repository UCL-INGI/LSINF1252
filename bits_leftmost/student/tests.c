#include "student_code.h"
#include "CTester/CTester.h"

void test_get_3_leftmost_bits(void)
{
	set_test_metadata("q1", _("Test of function test_get_3_leftmost_bits"), 1);
	uint32_t binaryNumber = 0b01000000000010101111011101101111;
	uint8_t leftMostBitsReturned = 0;
	SANDBOX_BEGIN;
	leftMostBitsReturned = get_3_leftmost_bits(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(leftMostBitsReturned,0b010);
	if ( leftMostBitsReturned!=0b010)
		push_info_msg(_("get_3_leftmost_bits returns the wrong value"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_get_3_leftmost_bits);
}

