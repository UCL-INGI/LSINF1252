// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
    set_test_metadata("myfunc", _("Test if return = n+1"), 1);

    int ret = 0;

    SANDBOX_BEGIN;
    ret = myfunc(41);
    SANDBOX_END;
    push_info_msg(_("Your function didn't fail."));
    set_tag("return");

    if (ret % 2 == 0)
        push_info_msg(_("You returned a pair number !"));


    CU_ASSERT_EQUAL(ret,42);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_myfunc_ret);
}

