// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
	set_test_metadata("q1", _("Test the function cycle_bits"), 1);

	uint32_t binaryNumber = 0b10110110101010001101111111111111;
	uint32_t binaryNumberReturned = 0;
	uint32_t ret2 = 0;


	SANDBOX_BEGIN;
	binaryNumberReturned = cycle_bits(binaryNumber,6);
	ret2 = cycle_bits(binaryNumber,38);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10101010001101111111111111101101);
	CU_ASSERT_EQUAL(ret2,0b10101010001101111111111111101101);
	if ( binaryNumberReturned!=0b10101010001101111111111111101101)
		push_info_msg(_("cycle_bits returns the wrong value"));
	if ( ret2!=0b10101010001101111111111111101101)
		push_info_msg(_("cycle_bits returns the wrong value for n>32"));

}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

