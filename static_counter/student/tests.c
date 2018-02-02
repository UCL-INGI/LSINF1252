// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test() {
    set_test_metadata("static_counter", _("Test counter"), 1);
    int count = 1;
    
    for(int i = 0; i < 40; i++){
        int ret = 0;

        SANDBOX_BEGIN;
        ret = stexp();
        SANDBOX_END;
        if(ret > 4096){
            set_tag("reset_counter");
        }
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