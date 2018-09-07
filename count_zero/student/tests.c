#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_1(){
    
    set_test_metadata("counting_zero", _("Test 1"), 1);
    
    char* str1 = "a\0b\0c\0d\0e";
    int ret = -2;
    
    SANDBOX_BEGIN;
    ret = counting_zero(str1,9);
    SANDBOX_END;
    
    CU_ASSERT_EQUAL(ret,4);
    if(ret != 4){
        push_info_msg(_("Vous n'avez pas le bon compte"));
    }
}

void test_2(){
    set_test_metadata("counting_zero", _("Test 2"), 1);
    
    char* str2 = "\0epl\0";
    int ret = -2;
    
    SANDBOX_BEGIN;
    ret = counting_zero(str2, 5);
    SANDBOX_END;
    
    CU_ASSERT_EQUAL(ret, 2);
    if(ret != 2){
        push_info_msg(_("Votre code ne fonctionne pas pour des strings commencant/terminant par \\0"));
    }
}

void test_complexe(){
    set_test_metadata("counting_zero", _("Test plus complexe"), 1);
    
    char* str3 = "\0\0\0\0a\\0b\0\\0";
    int ret = -2;
    
    SANDBOX_BEGIN;
    ret = counting_zero(str3, 11);
    SANDBOX_END;
    
    CU_ASSERT_EQUAL(ret,5);
    if(ret != 5){
        push_info_msg(_("Votre code ne fonctionne pas pour des strings plus complexes"));
    }
}

void test_null(){
    set_test_metadata("counting_zero", _("Test avec de mauvais arguments"), 1);
    
    char* str4 = NULL;
    int ret = -2;
    
    SANDBOX_BEGIN;
    ret = counting_zero(str4,1);
    SANDBOX_END;
    
    CU_ASSERT_EQUAL(ret, -1);
    if(ret != -1){
        push_info_msg(_("Votre code ne prend pas en compte les strings NULL"));
    }
}

int main(int argc,char* argv[])
{
    BAN_FUNCS(strlen, strncmp);
    RUN(test_1, test_2, test_complexe, test_null);
}
