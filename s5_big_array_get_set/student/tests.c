// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

void get_value_by_index(int i){
    return (i*i*(i/2))%20000;
}

/*
 * Generate a file named "file.txt".
 * The content is n int following the formula defined above.
 */
void gen_file(int n){
    int fd = open("file.txt",O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd == -1) {
        CU_FAIL("Error, can not initialise test file");
    }
    int array[n];
    int i = 0;
    for (i = 0; i < n; i++){
        array[i] = get_value_by_index(i);
        if (write(fd, (void *) &array[i], sizeof(int)) == -1){
            CU_FAIL("Error, can not initialise test file");
        }
    }
    if (close(fd) == -1)
        CU_FAIL("Error, can not initialise test file");
}

/*
 * Test with some int in the file
 */
void test_get() {
    set_test_metadata("q1", _("Test with normal file"), 2);
    gen_file(100);
    
    for(int i = 0; i < 100; i+=5){
        int ret = 0;
        
        SANDBOX_BEGIN;
        ret = get(i);
        SANDBOX_END;
        
        if(ret != get_value_by_index(i)){
            push_info_msg(_("You do not return the correct value."));
            CU_FAIL(); 
        }   
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system);
    RUN(test_get);
}
