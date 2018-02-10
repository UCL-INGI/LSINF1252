#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
	set_test_metadata("q1", _("Test the function nbits"), 1);
	int ret1 = -1;
	int ret2 = -1;
	int ret3 = -1;
	int ret4 = -1;
	SANDBOX_BEGIN;
	ret1 = nbits(0b1110110100);
	ret2 = nbits(0b0000010000);
	ret3 = nbits(0b1000000000000000);
	ret4 = nbits(0b0000000000000000);
	SANDBOX_END;
	CU_ASSERT_EQUAL(6, ret1);
	CU_ASSERT_EQUAL(1, ret2);
	CU_ASSERT_EQUAL(1, ret3);
	CU_ASSERT_EQUAL(0, ret4);
	if ( 6!= ret1) {
		push_info_msg(_("nbits does not return the correct answer when its input is 0b1110110100"));
    }
    if ( 1!= ret2 ) {
		push_info_msg(_("nbits does not return the correct answer when its input is 0b0000010000"));
    }
    if ( 1!= ret3 ) {
		push_info_msg(_("nbits does not return the correct answer when its input is 0b1000000000000000"));
    }
    if ( 0!= ret4) {
		push_info_msg(_("nbits does not return the correct answer when its input is 0b0000000000000000"));
    }
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

