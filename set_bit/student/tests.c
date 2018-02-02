// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
	set_test_metadata("q1", _("Test function set_bits"), 1);

	uint64_t binaryNumber = 0b10110010;
	uint64_t binaryNumberReturned;
	SANDBOX_BEGIN;
	binaryNumberReturned = set_bit(binaryNumber,1,0);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b10110000);
	if ( binaryNumberReturned!=0b10110000)
		push_info_msg(_("set_bit returns the wrong value when setting to 0"));
	
	SANDBOX_BEGIN;
	binaryNumberReturned = set_bit(binaryNumber,6,1);
	SANDBOX_END;

	CU_ASSERT_EQUAL(binaryNumberReturned,0b11110010);
	if ( binaryNumberReturned!=0b11110010)
		push_info_msg(_("set_bit returns the wrong value when setting to 1"));

}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

