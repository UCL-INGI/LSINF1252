// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

typedef struct 3dpoint{
    double x;
    double y;
    double z;
} 3dpoint_t;

3dpoint_t* gen_struct(int size){
    3dpoint_t* tab = malloc(size * sizeof(3dpoint_t));
    if (tab == NULL)
        return (3dpoint_t*)NULL;
    for (int i = 0; i < size; i++){
        tab[i] = malloc(sizeof(3dpoint_t));
        tab[i]->x = i+i;
        tab[i]->y = i+i+i;
        tab[i]->z = i+i*i;
    }
    return tab;
}

/*
 * Test with open fail
 */
void test() {
    set_test_metadata("q1", _("Test fail open"), 1);
    int size = 3;
    int ret = 0;
    3dpoint_t* tab = gen_struct(size);
    
    monitored.open = true;
    failures.open = FAIL_TWICE;
    failures.open_ret = -1;
    
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;
    
    //free
    for(int i = 0; i < size; i++)
        free(tab[i]);
    free(tab);
    
    if (ret != -1){
        push_info_msg(_("bla"));
        CU_FAIL();
}



int main(int argc,char** argv){
    BAN_FUNCS(system);
    RUN(test);
}
