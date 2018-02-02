#include "student_code.h"
#include "CTester/CTester.h"

void test_get_4_rightmost_bits(void)
{
	set_test_metadata("q1", _("Test the function get_4_rightmost_bits"), 1);

	uint32_t binaryNumber = 0b0111010110110101010001011;
	uint8_t rightMostBitsReturned;

	SANDBOX_BEGIN;
	rightMostBitsReturned = get_4_rightmost_bits(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(rightMostBitsReturned,0b1011);
	if ( rightMostBitsReturned!=0b1011)
		push_info_msg(_("get_4_rightmost_bits returns the wrong value"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_get_4_rightmost_bits);
}
