#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
	set_test_metadata("facto", _("test the function factorial"), 1);

	srand(time(NULL));
	int i = rand()%12;
	while (i<2)
		i= rand()%12;
	int ret1 = 0;
	int ret2 = 0;
	int ret3 = 0;

	SANDBOX_BEGIN;
	ret1 = factorial(i);
	ret2 = factorial(0);
	ret3 = factorial(1);
	SANDBOX_END;

	int sol = i;
	for(int j = i-1; j>0; j--)
		sol = sol*j;

	CU_ASSERT_EQUAL(ret1, sol);
	CU_ASSERT_EQUAL(ret2, 1);
	CU_ASSERT_EQUAL(ret3, 1);

	if (sol!=ret1){
		char *errtpl = _("factorial returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret1, i);
		push_info_msg(errmsg);
	}
	if (1!=ret2){
		char *errtpl = _("factorial returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret2, 0);
		push_info_msg(errmsg);
	}
	if (1!=ret3){
		char *errtpl = _("factorial returns the wrong value: you returned %d for %d");
		char errmsg[strlen(errtpl+30)];
		sprintf(errmsg, errtpl, ret2, 1);
		push_info_msg(errmsg);
	}
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

