// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_exist_not() {
    // We run multiples times the test to avoid student returning randomly 1 or 0.
    for(int i = 0; i < 4; i++){
        set_test_metadata("q1", _("Test file does not exist"), 1);
        int ret = 0;

        system("rm file.txt");
        SANDBOX_BEGIN;
        ret = file_exists("file.txt");
        SANDBOX_END;
    
        if(ret != -1){
            CU_FAIL();
            push_info_msg(_("When the file does not exist, you do not return -1"));
        }else{
             set_tag("test_file_exist_not");
        }
    }
}

void test_exist() {
    // We run multiples times the test to avoid student returning randomly 1 or 0.
    for(int i = 0; i < 4; i++){
        set_test_metadata("q1", _("Test file exists"), 1);
        int ret = 0;

        system("touch file.txt");
        SANDBOX_BEGIN;
        ret = file_exists("file.txt");
        SANDBOX_END;
    
        if(ret != 0){
            CU_FAIL();
            push_info_msg(_("When the file exist, you do not return 0"));
        }else{
            set_tag("test_file_exist");
        }
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system, open, set_tag);
    RUN(test_exist_not, test_exist);
}