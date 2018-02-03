#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
	set_test_metadata("q1", _("Test the task main_argc"), 1);

	int ret1 = 1;
	int ret2 = 1;
	int ret3 = 1;

	char **argv1 = {"tests", "a", "b", "ihuqfiudshfsi", "kujsvglqiu"};
	char **argv2 = {"tests", "a", "b", "ihuqfiudshfsi"};
	char **argv3 = {"tests"};
	char buf[16];

	SANDBOX_BEGIN;
	ret1 = main2(5, argv1);
	SANDBOX_END;
	ssize_t size = read(stdout_cpy, buf, 16);
	CU_ASSERT_EQUAL(size, 16);
	if (size<16)
		push_info_msg(_("Your function doesn't output enough characters"));
	else if (read(stdout_cpy, buf, 16)){
		CU_FAIL("");
		push_info_msg(_("Your function too much characters"));
		while (read(stdout_cpy, buf, 16)) //empties stdout
			;
	}
	else if (strncmp("a ihuqfiudshfsi\n", buf, 16)){
		CU_FAIL("");
		push_info_msg(_("Your function output the wrong string"));
	}
	else
		CU_PASS("");


	SANDBOX_BEGIN;
	ret2 = main2(4, argv1);
	SANDBOX_END;
	size = read(stdout_cpy, buf, 16);
	CU_ASSERT_EQUAL(size, 16);
	if (size<16){
		CU_FAIL("");
		push_info_msg(_("Your function doesn't output enough characters"));
	}
	else if (read(stdout_cpy, buf, 16)){
		CU_FAIL("");
		push_info_msg(_("Your function too much characters"));
		while (read(stdout_cpy, buf, 16)) //empties stdout
			;
	}
	else if (strncmp("a ihuqfiudshfsi\n", buf, 16)){
		CU_FAIL("");
		push_info_msg(_("Your function output the wrong string"));
	}
	else
		CU_PASS("");

	SANDBOX_BEGIN;
	ret3 = main2(1, argv1);
	SANDBOX_END;
	size = read(stdout_cpy, buf, 1);
	CU_ASSERT_EQUAL(size, 1);
	if (size<1){
		push_info_msg(_("Your function doesn't output anything when no arguments are provided"));
		CU_FAIL("");
	}
	else if (read(stdout_cpy, buf, 16)){
		CU_FAIL("");
		push_info_msg(_("Your function too much characters"));
		while (read(stdout_cpy, buf, 16)) //empties stdout
			;
	}
	else if (strncmp("\n", buf, 1)){
		CU_FAIL("");
		push_info_msg(_("Your function output the wrong string when no arguments are provided"));
	}
	else
		CU_PASS("");

	CU_ASSERT_EQUAL(ret1,0);
	CU_ASSERT_EQUAL(ret2,0);
	CU_ASSERT_EQUAL(ret3,0);
	if (ret1||ret2||ret3)
		push_info_msg(_("Your function didn't return 0"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

