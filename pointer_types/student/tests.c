#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_tab1() {
    set_test_metadata("first", _("test for the first input"), 1);

    int tab1[4] = {2, 3 ,5, 9};
    int tab2[4] = {1, 4, 4, 10};
    int tab3[4] = {10, 4, 21, 3};
    int tab4[4] = {10, 456789087, 2121212122, 1672386787};
    int ret = 0;

    SANDBOX_BEGIN;
    ret = first((void *) tab1);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,9);

    SANDBOX_BEGIN;
    ret = first((void *) tab2);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,10);

    SANDBOX_BEGIN;
    ret = first((void *) tab3);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,3);

    SANDBOX_BEGIN;
    ret = first((void *) tab4);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,1672386787);
}

void test_tab2() {
    set_test_metadata("second", _("test for the second input"), 1);

    char tab1[7] = {2, 3, 5, 9, 21, 1, 3};
    char tab2[7] = {1, 4, 4, 10, 0, 0, 0};
    char tab3[7] = {10, 4, -21, 3, -41, 20, -120};

    char ret = 0;

    SANDBOX_BEGIN;
    ret = second((void *)tab1);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,3);

    SANDBOX_BEGIN;
    ret = second((void *)tab2);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,0);

    SANDBOX_BEGIN;
    ret = second((void *)tab3);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,-120);
}

void test_tab3() {
    set_test_metadata("third", _("test for the third input"), 1);

    int tab1[180] = {2, 3 , 5, 9, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,-3,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, -2,0,0,0,0,0,0,0,0,0};
    int tab2[4] = {1, 4, 4, 10};
    int tab3[4] = {10, 0, 21, 3};
    int tab4[4] = {10, 456789087, 2121212122, 1672386787};

    int ret = -1;
    char *ptr;

    ptr = (char *)tab1;
    ptr -= 41;
    SANDBOX_BEGIN;
    ret = third((void *)ptr);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,tab1[1]);
    if(ret == -512){
        push_info_msg("Remember that an int is 4-bytes long. Add +180 to an int pointer will therefore add 720 bytes !");
    }
    else if(ret == -768){
        push_info_msg("Remember that an int is 4-bytes long. Add +45 to an int pointer will therefore add 180 bytes !");
    }


    ptr = (char *)tab2;
    ptr -= 41;
    SANDBOX_BEGIN;
    ret = third((void *)ptr);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,tab2[1]);

    ptr = (char *)tab3;
    ptr -= 41;
    SANDBOX_BEGIN;
    ret = third((void *)ptr);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,tab3[1]);

    ptr = (char *)tab4;
    ptr -= 41;
    SANDBOX_BEGIN;
    ret = third((void *)ptr);
    SANDBOX_END;
    CU_ASSERT_EQUAL(ret,tab4[1]);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_tab1,test_tab2,test_tab3);
}
