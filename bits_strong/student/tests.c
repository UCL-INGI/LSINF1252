#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
	set_test_metadata("q1", _("Test unset_strong_bits"), 1);
	uint32_t binaryNumber = 0b1011011101;
	uint32_t binaryNumberReturned;

	SANDBOX_BEGIN;
	binaryNumberReturned = unset_strong_bit(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11011101);
	if ( binaryNumberReturned!=0b11011101)
		push_info_msg(_("unset_strong_bits returns the wrong value"));

}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

