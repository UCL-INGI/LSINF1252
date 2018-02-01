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

void test_open() {
    set_test_metadata("q1", _("Test open"), 1);
    system("rm file.txt"); //Be sure the file does not exist
    int ret = 0;
    int tag_open = 0;
    //Test with open fail
    monitored.open = true;
    failures.open = FAIL_FIRST;
    failures.open_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;

    if (ret != -1){
        push_info_msg(_("When there is no file, your code does not return -1."));
        CU_FAIL();
        tag_open++;
    }
    
    //Test if open() return a correct file descriptor.
    gen_file(13);
    monitored.open = true;
    SANDBOX_BEGIN;
    myfunc("file.txt");
    SANDBOX_END;

    if(stats.open.last_return <= 2){
        push_info_msg(_("When the open() should be fine, your code returns -1."));
        CU_FAIL();
        tag_open++;
    }
    if(tag_open == 0){
        set_tag("open");
    }
}

void test_no_integer() {
    set_test_metadata("q1", _("Test without any integers."), 1);
    int sum = gen_file(0); //Empty file
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != sum){
        push_info_msg(_("When the file does not contain any integers, your code does not return 0."));
        CU_FAIL();
    }
}

void test_some_integers() {
    set_test_metadata("q1", _("Test with some integers."), 1);
    int sum = gen_file(15);
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != sum){
        push_info_msg(_("When the file contains some integers, your code does not return the correct sum."));
        CU_FAIL();
    }
}

void test_some_integers_fail_read() {
    set_test_metadata("q1", _("Test with some integers (read() fail)"), 1);
    gen_file(8);
    int ret = 0;
    
    monitored.read = true;
    failures.read = FAIL_THIRD;
    failures.read_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt");
    SANDBOX_END;
    
    if (ret != -1){
        push_info_msg(_("When a read() fails, your code does not return -1."));
        set_tag("failure_handling");
        CU_FAIL();
    }
}

void test_close() {
    set_test_metadata("q1", _("Test close()."), 1);
    gen_file(2);
    
    monitored.close = true;
    monitored.open = true;
    SANDBOX_BEGIN;
    myfunc("file.txt");
    SANDBOX_END;
    
    int close_ok = 0;
    if (stats.close.called == 0){
        push_info_msg(_("You did not close() the file."));
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
    RUN(test_open, test_no_integer, test_some_integers, test_some_integers_fail_read, test_close);
}
