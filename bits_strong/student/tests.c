#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

uint32_t unset_strong_bit(uint32_t x)
{
	if (!x)
		return 0;
	for (short i = 1; i<32; i++)
		if (!(x>>i))
			return x - (1<<(i-1));
	return x;
}



void test_myfunc_ret() {
	set_test_metadata("q1", _("Test reset_highestorder_bits"), 1);
	uint32_t binaryNumber = 0b1011011101;
	uint32_t binaryNumberReturned;

	SANDBOX_BEGIN;
	binaryNumberReturned = reset_highestorder_bit(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,unset_strong_bit(binaryNumber));
	if ( binaryNumberReturned!=unset_strong_bit(binaryNumber)) {
		push_info_msg(_("reset_highestorder_bits does not return the correct value when its input is 0b0...01011011101"));
    }
    binaryNumber = 0b00000001;
    SANDBOX_BEGIN;
	binaryNumberReturned = reset_highestorder_bit(binaryNumber);
	SANDBOX_END;
CU_ASSERT_EQUAL(binaryNumberReturned,unset_strong_bit(binaryNumber));
	if ( binaryNumberReturned!=unset_strong_bit(binaryNumber)) {
		push_info_msg(_("reset_highestorder_bits does not return the correct value when its input is 0b0...0000001"));
    }

    
    binaryNumber = 0b00000000;
    SANDBOX_BEGIN;
	binaryNumberReturned = reset_highestorder_bit(binaryNumber);
	SANDBOX_END;
CU_ASSERT_EQUAL(binaryNumberReturned,unset_strong_bit(binaryNumber));
	if ( binaryNumberReturned!=unset_strong_bit(binaryNumber)) {
		push_info_msg(_("reset_highestorder_bits does not return the correct value when its input is 0b0...0000000"));
    }


    
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

