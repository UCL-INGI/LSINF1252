// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_exist_not() {
    set_test_metadata("q1", _("Test file does not exist"), 1);

    // We run multiples times the test to avoid student returning randomly 1 or 0.
    for(int i = 0; i < 4; i++){
        int ret = 0;

        system("rm file.txt");
        SANDBOX_BEGIN;
        ret = file_exists("file.txt");
        SANDBOX_END;
    
        if(ret != -1){
            CU_FAIL();
            push_info_msg(_("When the file does not exist, you do not return -1"));
            return;
        }else{
             set_tag("test_file_exist_not");
        }
    }
}

void test_exist() {
    set_test_metadata("q1", _("Test file exists"), 1);

    // We run multiples times the test to avoid student returning randomly 1 or 0.
    for(int i = 0; i < 4; i++){
        int ret = 0;
        int stop = 0;
        system("touch file.txt");
        monitored.close = true;
        monitored.open = true;
        SANDBOX_BEGIN;
        ret = file_exists("file.txt");
        SANDBOX_END;
    
        if(ret != 0){
            CU_FAIL();
            stop++;
            push_info_msg(_("When the file exist, you do not return 0"));
        }else{
            set_tag("test_file_exist");
        }
        
        if (stats.open.called > 0 && stats.close.called == 0){
            stop++;
            push_info_msg(_("You did not close() the file when the file exists."));
            CU_FAIL();
        }
        if(stop > 0)
            return;
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system, fopen, fread, fwrite, fclose, stat);
    RUN(test_exist_not, test_exist);
}