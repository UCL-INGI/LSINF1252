#include "student_code.h"
#include "CTester/CTester.h"

void test_get_4_rightmost_bits(void)
{
	set_test_metadata("q1", _("Test the function get_4_rightmost_bits"), 1);

	uint32_t binaryNumberA = 0b0111010110110101010001011;
    uint32_t binaryNumberB = 0b0111010110110101010001110;
	uint32_t binaryNumberC = 0b0111010110110101010011001;


	uint8_t rightMostBitsReturnedA;
	uint8_t rightMostBitsReturnedB;
	uint8_t rightMostBitsReturnedC;


	SANDBOX_BEGIN;
	rightMostBitsReturnedA = get_4_rightmost_bits(binaryNumberA);
	rightMostBitsReturnedB = get_4_rightmost_bits(binaryNumberB);
	rightMostBitsReturnedC = get_4_rightmost_bits(binaryNumberC);
	SANDBOX_END;
    
	CU_ASSERT_EQUAL(rightMostBitsReturnedA,0b1011);
	if ( rightMostBitsReturnedA!=0b1011)
		push_info_msg(_("get_4_rightmost_bits returns the wrong value"));
    
	CU_ASSERT_EQUAL(rightMostBitsReturnedB,0b1110);
	if ( rightMostBitsReturnedB!=0b1110)
		push_info_msg(_("get_4_rightmost_bits returns the wrong value"));
    
	CU_ASSERT_EQUAL(rightMostBitsReturnedC,0b1001);
	if ( rightMostBitsReturnedC!=0b1001)
		push_info_msg(_("get_4_rightmost_bits returns the wrong value"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS(set_tag);
	RUN(test_get_4_rightmost_bits);
}
