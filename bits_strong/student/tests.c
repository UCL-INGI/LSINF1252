#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
	set_test_metadata("q1", _("Test reset_highestorder_bits"), 1);
	uint32_t binaryNumber = 0b1011011101;
	uint32_t binaryNumberReturned;

	SANDBOX_BEGIN;
	binaryNumberReturned = reset_highestorder_bit(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11011101);
	if ( binaryNumberReturned!=0b01011101) {
		push_info_msg(_("reset_highestorder_bits doest not return the correct value"));
    }
 
    
    binaryNumber = 0b00000001;
    SANDBOX_BEGIN;
	binaryNumberReturned = reset_highestorder_bit(binaryNumber);
	SANDBOX_END;
	CU_ASSERT_EQUAL(binaryNumberReturned,0b11011101);
	if ( binaryNumberReturned!=0b00000000) {
		push_info_msg(_("reset_highestorder_bits doest not return the correct value"));
    }


    
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

