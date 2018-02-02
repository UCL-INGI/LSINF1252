// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

int tab_find_correct(int tab[], int len, int elem){
    for(int i = 0; i < len; i++){
        if(tab[i] == elem)
            return i;
    }
    return -1;
}

void test(){
    set_test_metadata("tab_find", _("Test function tab find"), 1);    

    const int size = 10;
    int array[10] = {1,3,5,7,9,0,8,6,4,2};
    int *tab = trap_buffer(size*sizeof(int), TRAP_RIGHT, PROT_WRITE, array);
    int ret = 0;
    
    for(int i = -5; i < size+5; i++){
        SANDBOX_BEGIN;
        ret = tab_find(tab, size, i);
        SANDBOX_END;
        
        if(ret != tab_find_correct(tab, size, i)){
            push_info_msg(_("Your function does not return the correct index."));
            CU_FAIL();
            break;
        }
    }
    free_trap(tab,size*sizeof(int));
}

int main(int argc,char** argv) {
    BAN_FUNCS(system, set_tag);
    RUN(test);
}

