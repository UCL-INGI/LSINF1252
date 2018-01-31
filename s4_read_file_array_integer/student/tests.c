// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

/*
 * Generate a file named "file.txt".
 * The content is the integers in range [0, n[
 * The sum of integers in the file is returned.
 */
int gen_file(int n){
    int fd = open("file.txt",O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd == -1) {
        CU_FAIL(_("Error, can not initialise test file"));
    }
    int array[n];
    int i = 0;
    int sum = 0;
    for (i = 0; i < n; i++){
        array[i]=i;
        sum+=i;
        if (write(fd, (void *) &array[i], sizeof(int)) == -1){
            CU_FAIL(_("Error, can not initialise test file"));
        }
    }
    if (close(fd) == -1)
        CU_FAIL(_("Error, can not initialise test file"));
    return sum;
}

/*
 * Test with no file
 */
void test_no_file() {
    set_test_metadata("q1", _("Test with no file"), 1);
    int ret = 0;

    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != -1){
        push_info_msg(_("When there is no file, your code does not return -1."));
        CU_FAIL();
    }else{
        set_tag("open");
    }
}

/*
 * Test with no file
 */
void test_fail_open() {
    set_test_metadata("q1", _("Test fail open"), 1);
    int ret = 0;
    
    monitored.open = true;
    failures.open = FAIL_FIRST;
    failures.open_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;

    if (ret != -1){
        push_info_msg(_("When there is no file, your code does not return -1."));
        CU_FAIL();
    }else{
        set_tag("open");
    }
}


/*
 * Test with no integer in the file
 */
void test_no_integer() {
    set_test_metadata("q1", _("Test without any integers."), 1);
    int sum = gen_file(0);
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != sum){
        push_info_msg(_("When the file does not contain any integers, your code does not return 0."));
        CU_FAIL();
    }
}


/*
 * Test with some integers in the file
 */
void test_some_integers() {
    set_test_metadata("q1", _("Test with some intagers."), 2);
    int sum = gen_file(12);
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != sum){
        push_info_msg(_("When the file contains some integers, your code does not return the correct sum."));
        CU_FAIL();
    }
}

/*
 * Test with some integers in the file with close()
 */
void test_close() {
    set_test_metadata("q1", _("Test close()."), 1);
    gen_file(2);
    
    monitored.close = true;
    monitored.open = true;
    SANDBOX_BEGIN;
    myfunc("file.txt");
    SANDBOX_END;
    
    int close_ok = 0;
    if (stats.close.called != 1){
        push_info_msg(_("You did not close() the file."));
        close_ok++;
        CU_FAIL();
    }if (stats.open.called != 1){
        push_info_msg(_("The open should be use only once."));
        close_ok++;
        CU_FAIL();
    }if(stats.open.last_return != stats.close.last_params.fd){
        push_info_msg(_("The close() does not close the file you opened before."));
        close_ok++;
        CU_FAIL();
    }if(close_ok == 0){
        set_tag("close");
    }

}

int main(int argc,char** argv){
    BAN_FUNCS();
    RUN(test_no_file, test_fail_open, test_no_integer, test_some_integers, test_close);
}
