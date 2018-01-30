// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

int get_value_by_index(int i){
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
    gen_file(1000);
    
    for(int i = 0; i < 1000; i+=50){
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


/*
 * Test with some int in the file
 */
void test_set() {
    set_test_metadata("q2", _("Test with normal file"), 2);
    gen_file(1000);
    
    for(int i = 0; i < 1000; i+=50){        
        SANDBOX_BEGIN;
        set(i, 2222+i);
        SANDBOX_END;  
    }
    
    int fd = open("file.txt", O_RDONLY);
    if(fd == -1) {
        CU_FAIL("Error, can not initialise test file");
    }
    for(int i = 0; i < 1000; i+=50){
        lseek(fd, (off_t) i*sizeof(int), SEEK_SET);
        int res;
        read(fd, (void *) &res, sizeof(int));
        if (res != 2222+i){
            push_info_msg(_("You do not set the correct value in the file."));
            CU_FAIL(); 
        }
    }
    close(fd);
}

void test_close_q1(){
    set_test_metadata("q1", _("Test close"), 1);
    monitored.close = true;        
    SANDBOX_BEGIN;
    get(0);
    SANDBOX_END;
    printf("Q1:%d\n", stats.close.called);
    if (stats.close.called != 1){
        push_info_msg(_("You did not close the file."));
        CU_FAIL();
    }
}

void test_close_q2(){
    set_test_metadata("q2", _("Test close"), 1);
    monitored.close = true;        
    SANDBOX_BEGIN;
    set(0,0);
    SANDBOX_END;
    printf("Q2:%d\n", stats.close.called);
    if (stats.close.called != 1){
        push_info_msg(_("You did not close the file."));
        CU_FAIL();
    }
}

int main(int argc, char** argv){
    BAN_FUNCS(system, set_tag);
    RUN(test_get, test_set, test_close_q1, test_close_q2);
}
