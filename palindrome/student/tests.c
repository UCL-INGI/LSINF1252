// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
	set_test_metadata("q1", _("Test the function palindrome"), 1);

	int ret1 = -2;
	int ret2 = -2;
	int ret3 = -2;
	int ret4 = -2;
	int ret5 = -2;
	int ret6 = -2;
	int ret7 = -2;

	char *str1 = trap_buffer(44, TRAP_RIGHT, PROT_WRITE, "Tityre tu patulae recubans sub tegmine fagi");
	char *str2 = trap_buffer(40, TRAP_RIGHT, PROT_WRITE, "tu l as trop ecrase cesar ce port salut");
	char *str3 = trap_buffer(1, TRAP_RIGHT, PROT_WRITE, "");
	char *str4 = trap_buffer(25, TRAP_RIGHT, PROT_WRITE, "azertyuioppptpoiuytreza");
	char *str5 = trap_buffer(22, TRAP_RIGHT, PROT_WRITE, "qsdfghjklmmlkjhgfdsq");
	char *str6 = trap_buffer(2, TRAP_RIGHT, PROT_WRITE, "e");

	SANDBOX_BEGIN;
	ret1 = pal(str1);
	ret2 = pal(str2);
	ret3 = pal(str3);
	ret4 = pal(str4);
	ret5 = pal(str5);
	ret6 = pal(NULL);
	ret7 = pal(str6);
	SANDBOX_END;
	free_trap(str1,44);
    free_trap(str2,40);
    free_trap(str3,1);
    free_trap(str4,25);
    free_trap(str5,22);
    free_trap(str6,2);
	CU_ASSERT_EQUAL(ret1,0);
	CU_ASSERT_EQUAL(ret2,1);
	CU_ASSERT_EQUAL(ret3,1);
	CU_ASSERT_EQUAL(ret4,0);
	CU_ASSERT_EQUAL(ret5,1);
	CU_ASSERT_EQUAL(ret6,-1);
	CU_ASSERT_EQUAL(ret7,1);
	if (ret1!=0 || ret4!=0)
		push_info_msg(_("Your function does not work correctly for strings that are not palindromes"));
	if (ret2!=1 || ret3!=1 || ret5!=1 || ret7!=1)
		push_info_msg(_("Your function doesn't work for strings that are palindromes"));
	if (ret6!=-1)
		push_info_msg(_("Your function doesn't work for NULL strings"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

