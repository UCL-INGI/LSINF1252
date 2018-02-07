// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_enqueue_empty() {
    set_test_metadata("enqueue", _("Enqueue of an empty list"), 1);
    
    node_t* n = NULL;
    queue_t q;
    q.head = &n;

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

