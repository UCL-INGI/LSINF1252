/*
* MODIFIED, to check
*/

#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_myfunc_ret()
{
    set_test_metadata("q1", _("Test the task main_argc"), 1);

    int ret1 = 1;
    int ret2 = 1;
    int ret3 = 1;
    int ret4 = 1;
    int ret5 = 1;

    char *str1 ="tests";
    char *str2 ="a";
    char *str3 ="b";
    char *str4 ="ihuqfiudshfsi";
    char *str5 ="kujsvglqiu";
    char *argv1[5] = {str1, str2, str3, str4, str5};
    char *argv2[4] = {str1, str2, str3, str5};
    char *argv3[1] = {str1};
    char *argv4[3] = {str1, str2, str3};
    char *argv5[2] = {str1, str5};
    char buf[16];

    SANDBOX_BEGIN;
    ret1 = main2(5, argv1);
    SANDBOX_END;
    read(stdout_cpy, buf, 16);
    if (!strncmp("a ihuqfiudshfsi ", buf, 16)) {
        CU_FAIL("");
        push_info_msg(_("Your function output the wrong string when 4 arguments are provided. Be sure not to have a space after the last argument"));
    }
    else if (strncmp("a ihuqfiudshfsi\n", buf, 16)){
        CU_FAIL("");
        char error_msg[200];
        sprintf(error_msg, _("Arguments : '%s %s %s %s %s', expected : 'a ihuqfiudshfsi\\\\n', your answer : '%s'"), str1, str2, str3, str4, str5, buf);
        push_info_msg(error_msg);
        if(!strncmp(buf,"tests",5)){
            push_info_msg(_("Don't forget that the first argument (index 0) is the name of the program ! myProg in this case !"))
        }
    }
    else
        CU_PASS("");


    SANDBOX_BEGIN;
    ret2 = main2(4, argv2);
    SANDBOX_END;

    read(stdout_cpy, buf, 13);
    if (!strncmp("a kujsvglqiu ", buf, 13)) {
        CU_FAIL("");
        push_info_msg(_("Your function output the wrong string when 3 arguments are provided. Be sure not to have a space after the last argument"));
    }
    else if (strncmp("a kujsvglqiu\n", buf, 13)){
        CU_FAIL("");
        char error_msg[200];
        sprintf(error_msg, _("Arguments : '%s %s %s %s', expected : 'a kujsvglqiu\\\\n', your answer : '%s'"), str1, str2, str3, str5, buf);
        push_info_msg(error_msg);
        if(!strncmp(buf,"tests",5)){
            push_info_msg(_("Don't forget that the first argument (index 0) is the name of the program ! myProg in this case !"))
        }
    }
    else
        CU_PASS("");

    SANDBOX_BEGIN;
    ret3 = main2(1, argv3);
    SANDBOX_END;
    read(stdout_cpy, buf, 1);
    if (strncmp("\n", buf, 1)){
        CU_FAIL("");
        char error_msg[200];
        sprintf(error_msg, _("Arguments : '%s', expected : '\\\\n', your answer : '%s'"), str1, buf);
        push_info_msg(error_msg);
    }
    else
        CU_PASS("");

    SANDBOX_BEGIN;
    ret4 = main2(3, argv4);
    SANDBOX_END;

    read(stdout_cpy, buf, 2);
    if (!strncmp("a ", buf, 2)) {
        CU_FAIL("");
        push_info_msg(_("Your function output the wrong string when 2 arguments are provided. Be sure not to have a space after the last argument"));
    }
    else if (strncmp("a\n", buf, 2)){
        CU_FAIL("");
        char error_msg[200];
        sprintf(error_msg, _("Arguments : '%s %s %s', expected : 'a\\\\n', your answer : '%s'"), str1, str2, str3, buf);
        push_info_msg(error_msg);
        if(!strncmp(buf,"tests",5)){
            push_info_msg(_("Don't forget that the first argument (index 0) is the name of the program ! myProg in this case !"))
        }
    }
    else
        CU_PASS("");

    SANDBOX_BEGIN;
    ret5 = main2(2, argv5);
    SANDBOX_END;

    read(stdout_cpy, buf, 11);
    if (!strncmp("kujsvglqiu ", buf, 11)) {
        CU_FAIL("");
        push_info_msg(_("Your function output the wrong string when 1 arguments are provided. Be sure not to have a space after the last argument"));
    }
    if (strncmp("kujsvglqiu\n", buf, 11)){
        CU_FAIL("");
        char error_msg[200];
        sprintf(error_msg, _("Arguments : '%s %s', expected : 'kujsvglqiu\\\\n', your answer : '%s'"), str1, str5, buf);
        push_info_msg(error_msg);
        if(!strncmp(buf,"tests",5)){
            push_info_msg(_("Don't forget that the first argument (index 0) is the name of the program ! myProg in this case !"))
        }
    }
    else
        CU_PASS("");

    CU_ASSERT_EQUAL(ret1,0);
    CU_ASSERT_EQUAL(ret2,0);
    CU_ASSERT_EQUAL(ret3,0);
    CU_ASSERT_EQUAL(ret4,0);
    CU_ASSERT_EQUAL(ret5,0);
    if (ret1||ret2||ret3||ret4||ret5)
        push_info_msg(_("Your function didn't return 0"));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_myfunc_ret);
}

