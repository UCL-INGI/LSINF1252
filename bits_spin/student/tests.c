// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
	set_test_metadata("q1", _("Test the functioon cycle_bits"), 1);

	uint32_t binaryNumber = 0b10110110101010001101111111111111;
	uint32_t binaryNumberReturned = 0;

	SANDBOX_BEGIN;
	binaryNumberReturned = cycle_bits(binaryNumber,6);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10101010001101111111111111101101);
	if ( binaryNumberReturned!=0b10101010001101111111111111101101)
		push_info_msg(_("cycle_bits returns the wrong value"));

}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

