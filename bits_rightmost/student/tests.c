// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret() {
    set_test_metadata("myfunc", _("Brief description of the test"), 1);

    int ret = 0;

    SANDBOX_BEGIN;
    ret = myfunc(21);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_myfunc_ret);
}

