#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_tab1() {
    set_test_metadata("first", _("test for the first input"), 1);

    int tab1[4] = {2, 3 , 5, 9};
    int tab2[4] = {1, 4, 4, 10};
    int tab3[4] = {10, 4, 21, 3};
    int tab4[4] = {10, 456789087, 2121212122, 1672386787};
    int* a = (int*) tab1;
    int ret = 0;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,5);

    a = (void *)tab2;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,4);

    a = (void *)tab3;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,21);

    a = (void *)tab4;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,2121212122);
}

void test_tab2() {
    set_test_metadata("second", _("test for the second input"), 1);

    int tab1[4] = {2, 3 , 5, 9};
    int tab2[4] = {1, 4, 4, 10};
    int tab3[4] = {10, 4, 21, 3};
    int tab4[4] = {10, 456789087, 2121212122, 1672386787};
    int *a = (void *)tab1;

    char ret = 0;

    SANDBOX_BEGIN;
    ret = (int) second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    a = (void *)tab2;

    SANDBOX_BEGIN;
    ret = (int) second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    a = (void *)tab3;

    SANDBOX_BEGIN;
    ret =(int) second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);

    a = (void *)tab4;

    SANDBOX_BEGIN;
    ret =(int) second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,12);

    ret = 0;
}

void test_tab3() {
    set_test_metadata("third", _("test for the third input"), 1);

    int tab1[4] = {2, 3 , 5, 9};
    int tab2[4] = {1, 4, 4, 10};
    int tab3[4] = {10, 4, 21, 3};
    int tab4[4] = {10, 456789087, 2121212122, 1672386787};
    int * a = (void *)tab1;

    int ret = 0;

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,589824);

    a = (void *)tab2;

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,655360);

    a = (void *)tab3;

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,196608);

    a = (void *)tab4;

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,-1729921425);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_tab1,test_tab2,test_tab3);
}
