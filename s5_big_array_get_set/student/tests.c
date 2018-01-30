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

void test_close(){
    int close_tag = 0;
    char *q[2];
    q[0] = "q1";
    q[1] = "q2";
    for(int i = 0; i < 2; i++){
        set_test_metadata(q[i], _("Test close"), 1);
    
        monitored.close = true;
        stats.close.called = 0;
        SANDBOX_BEGIN;
        if (i==0)
            get(0);
        else
            set(0,0);
        SANDBOX_END;

        printf("FFFFFF%d\n", stats.close.called);
        if (stats.close.called != 1){
            push_info_msg(_("You did not close the file."));
            CU_FAIL();
            close_tag++;
        }
    }
    if (close_tag == 0){
        set_tag("close");
    }
}

int main(int argc, char** argv){
    BAN_FUNCS(system, set_tag);
    RUN(test_get, test_set, test_close);
}
