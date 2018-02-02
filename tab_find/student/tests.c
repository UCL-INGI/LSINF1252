// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test()
{
    set_test_metadata("tab_find", _("Test function tab find"), 1);    

    int array[10] = {1,3,5,7,9,0,8,6,4,2};
    int *tab = trap_buffer(10, TRAP_RIGHT, PROT_WRITE, array);

    int ret = 0;
    SANDBOX_BEGIN;
    ret = tab_find(tab, 10, 2);
    SANDBOX_END;
        
    free_trap(tab,10);
    if(ret != 9){
        push_info_msg(_("Your function does not return the correct index."));
        CU_FAIL();
    }
}

int main(int argc,char** argv) {
    BAN_FUNCS(syetem, set_tag);
    RUN(test);
}

