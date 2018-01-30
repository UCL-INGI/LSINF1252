#include <stdlib.h>
#include <time.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_absval_ret() {
	set_test_metadata("absval", _("test the function absval"), 1);

	srand(time(NULL));
	int i = rand();
	while (!i)
		i = rand();
	int ret1 = 0;
	int ret2 = 0;
	int ret3 = 567;


	SANDBOX_BEGIN;
	ret1 = absval(i);
	ret2 = absval(-i);
	ret3 = absval(0);
	SANDBOX_END;

	CU_ASSERT_EQUAL(ret1, i);
	CU_ASSERT_EQUAL(ret2, i);
	CU_ASSERT_EQUAL(ret3, 0);
	if (i!=ret2){
		char *errtpl = _("absval returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret2, i);
		push_info_msg(errmsg);
	}
	if (i!=ret1){
		char *errtpl = _("absval returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret1, i);
		push_info_msg(errmsg);
	}
	if (0!=ret3){
		char *errtpl = _("absval returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret3, 0);
		push_info_msg(errmsg);
	}
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_absval_ret);
}

