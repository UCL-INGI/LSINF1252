// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_exist_not() {
    set_test_metadata("q1", _("Test write array struct"), 1);
    int ret = 0;

    system("rm file.txt");
    SANDBOX_BEGIN;
    ret = file_exists("file.txt");
    SANDBOX_END;
    
    if(ret != -1){
        CU_FAIL();
        push_info_msg(_("When the file does not exist, you do not return -1"));
    }
}

void test_exist() {
    set_test_metadata("q1", _("Test write array struct"), 1);
    int ret = 0;

    system("touch file.txt");
    SANDBOX_BEGIN;
    ret = file_exists("file.txt");
    SANDBOX_END;
    
    if(ret != 0){
        CU_FAIL();
        push_info_msg(_("When the file exist, you do not return 0"));
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system);
    RUN(test_exist_not, test_exist);
}
