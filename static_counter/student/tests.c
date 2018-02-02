// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test() {
    set_test_metadata("static_counter", _("Test"), 1);
    int count = 1;
    
    // We run multiples times the test to avoid student returning randomly 1 or 0.
    for(int i = 0; i < 20; i++){
        int ret = 0;

        SANDBOX_BEGIN;
        ret = stexp();
        SANDBOX_END;
    
        if(ret != count){
            CU_FAIL();
            push_info_msg(_("Your counter is not correct."));
            break;
        }
        count *= 2;
        if(count > 4096)
            count = 1;
    }
}


int main(int argc,char** argv){
    BAN_FUNCS(system, set_tag);
    RUN(test);
}