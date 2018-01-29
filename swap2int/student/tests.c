// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_swap() {
    set_test_metadata("swap", _("Teste que swap échange bien les valeurs"), 1);
    int i = rand();
    int j = rand();
    while (i==j)//to make sure i!=j
	    j = rand();
    int i_orig = i;
    int j_orig = j;
    SANDBOX_BEGIN;
    swap(&i, &j);
    SANDBOX_END;

    CU_ASSERT_EQUAL(i,j_orig);
    CU_ASSERT_EQUAL(j,i_orig);
    if (i!=j_orig || j!=i_orig)
	    push_info_msg(_("Les variables ne sont pas bien inversées."));
}

int main(int argc,char** argv)
{
    RUN(test_swap);
}

