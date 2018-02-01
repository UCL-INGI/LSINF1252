// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
	set_test_metadata("pal", _("Test the function palindrome"), 1);

	int *ret1 = -2;
	int *ret2 = -2;
	int *ret3 = -2;
	int *ret4 = -2;
	int *ret5 = -2;

	char *str1 = trap_buffer(44, TRAP_RIGHT, PROT_READ, "Tityre tu patulae recubans sub tegmine fagi");
	char *str2 = trap_buffer(39, TRAP_RIGHT, PROT_READ, "tu l as trop ecrase cesar ce port salut");
	char *str3 = trap_buffer(1, TRAP_RIGHT, PROT_READ, "");
	char *str4 = trap_buffer(23, TRAP_RIGHT, PROT_READ, "azertyuioppptpoiuytreza");
	char *str5 = trap_buffer(22, TRAP_RIGHT, PROT_READ, "qsdfghjklimmlkjhgfdsq");

	SANDBOX_BEGIN;
	ret1 = pal(str1);
	ret2 = pal(str2);
	ret3 = pal(str3);
	ret4 = pal(str4);
	ret5 = pal(str5);
	ret6 = pal(NULL);
	SANDBOX_END;

	CU_ASSERT_EQUAL(ret1,0);
	CU_ASSERT_EQUAL(ret2,1);
	CU_ASSERT_EQUAL(ret3,1);
	CU_ASSERT_EQUAL(ret4,0);
	CU_ASSERT_EQUAL(ret5,1);
	CU_ASSERT_EQUAL(ret6,-1);
	if (ret1!=0 || ret4!=0)
		push_info_msg(_("Your function doesn't work for strings that are not palindromes"));
	if (ret2!=1 || ret3!=1 || ret5!=1)
		push_info_msg(_("Your function doesn't work for strings that are palindromes"));
	if (ret6!=-1)
		push_info_msg(_("Your function doesn't work for NULL strings"));
}

int main(int argc,char** argv)
{
	BAN_FUNCS();
	RUN(test_myfunc_ret);
}

