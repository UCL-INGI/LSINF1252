#include <stdlib.h>
#include <fcntl.h>
#include "CTester/CTester.h"
#include "student_code.h"


void test_foo() {
    set_test_metadata("simple", _("Test simple printf"), 1);

    SANDBOX_BEGIN;
    print_foo();
    SANDBOX_END;

    char buf[4];
    int n = read(stdout_cpy, buf, 4);
    CU_ASSERT_EQUAL(n, 4);
    CU_ASSERT_TRUE(!strncmp(buf, "foo\n", 4));
}

void test_digit() {
    set_test_metadata("digit", _("Test digit formatting"), 1);

    SANDBOX_BEGIN;
    print_digit(21);
    SANDBOX_END;
    
    char* the_magic_NUMBER = "The magic number is NUMBER.\n";

    char buf[24];
    int n = read(stdout_cpy, buf, 24);
    CU_ASSERT_EQUAL(n, 24);
    CU_ASSERT_TRUE(!strncmp(buf, "The magic number is 21.\n", 24));
    if(!strncmp(buf, the_magic_NUMBER, 24)){
        push_info_msg("You should replace NUMBER by the variable number");
    }

    SANDBOX_BEGIN;
    print_digit(42);
    SANDBOX_END;

    n = read(stdout_cpy, buf, 24);
    CU_ASSERT_EQUAL(n, 24);
    CU_ASSERT_TRUE(!strncmp(buf, "The magic number is 42.\n", 24));
}

void test_sprintf() {
    set_test_metadata("sprintf", _("Test sprintf"), 1);
    
    char buf[50];
    format_str(buf, 42, "Olivier", 'B');
    CU_ASSERT_TRUE(!strncmp(buf, "Mister Olivier B. has 42 eggs", 30));

    format_str(buf, 1, "A", 'G');
    CU_ASSERT_TRUE(!strncmp(buf, "Mister A G. has 1 eggs", 23));
}


int main(int argc,char** argv)
{
    BAN_FUNCS(fprintf);
    RUN(test_foo, test_digit, test_sprintf);
}
