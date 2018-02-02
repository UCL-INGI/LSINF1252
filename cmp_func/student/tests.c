#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

uint8_t f_even(uint8_t n) {
    return (n%2 == 0);
}

uint8_t f_odd(uint8_t n) {
    return (n%2 == 1);
}

uint8_t f_inc1(uint8_t n) {
    return n+1;
}

uint8_t f_inc2(uint8_t n) {
    return n+1;
}

uint8_t f_42(uint8_t n) {
    if (n == 42)
        return 1;
    return 0;
}

uint8_t f_zero(uint8_t n) {
    return 0;
}


void test_cmp_func_ret1() {
    set_test_metadata("cmp_func", _("Comparison between a function which tells if the argument is even and another which tells if it's odd"), 1);

    int ret = 1;

    SANDBOX_BEGIN;
    ret = cmp_func(&f_odd,&f_even);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);
}

void test_cmp_func_ret2() {
    set_test_metadata("cmp_func", _("Comparison between two different functions which do the same computations"), 1);

    int ret = 0;

    SANDBOX_BEGIN;
    ret = cmp_func(&f_inc1,&f_inc2);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
}

void test_cmp_func_ret3() {
    set_test_metadata("cmp_func", _("Comparison between a function which returns 1 if the input is 42, 0 otherwise and another function which always returns 0"), 1);

    int ret = 1;

    SANDBOX_BEGIN;
    ret = cmp_func(&f_42,&f_zero);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,0);
}

void test_cmp_func_ret4() {
    set_test_metadata("cmp_func", _("Comparison between the same function given twice"), 1);

    int ret = 0;

    SANDBOX_BEGIN;
    ret = cmp_func(&f_42,&f_42);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,1);
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_cmp_func_ret1, test_cmp_func_ret2, test_cmp_func_ret3, test_cmp_func_ret4);
}
