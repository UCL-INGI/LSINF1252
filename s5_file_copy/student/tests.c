// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

/*
 * Generate a file named "file.txt".
 * The content is the bytes in range [0, n[
 */
void gen_file(int n){
    system("rm newfile.txt");
    int fd = open("file.txt",O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd == -1) {
        CU_FAIL("Error, can not initialise test file");
    }
    char array[n];
    int i = 0;
    for (i = 0; i < n; i++){
        array[i] = i;
        if (write(fd, (void *) &array[i], sizeof(char)) == -1){
            CU_FAIL("Error, can not initialise test file");
        }
    }
    if (close(fd) == -1)
        CU_FAIL("Error, can not initialise test file");
}

/*
 * Test with open fail
 */
void test_fail_open() {
    set_test_metadata("q1", _("Test when open() fails"), 1);
    int ret = 0;
    
    monitored.open = true;
    failures.open = FAIL_TWICE;
    failures.open_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file_that_does_not_exists.txt", "newfile_that_should_not_be_create.txt");
    SANDBOX_END;

    if (ret != -1){
        push_info_msg(_("When the open() function fails, your code does not return -1."));
        CU_FAIL();
    }else{
        set_tag("open");
    }
}


/*
 * Test with an empty file
 */
void test_empty_file() {
    set_test_metadata("q1", _("Test with empty file"), 1);
    gen_file(0);
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    system("chmod 644 newfile.txt"); //If student does not set permission correctly, we set it to avoid test error
    if (system("diff file.txt newfile.txt") != 0){
        push_info_msg(_("You copy is not identical."));
        CU_FAIL();
    }
    if (ret != 0){
        push_info_msg(_("When the file is empty, your copy is not identical."));
        CU_FAIL();
    }
}


/*
 * Test permission
 */
void test_file_permission() {
    set_test_metadata("q1", _("Test if permissions are equal"), 1);
    gen_file(122);
    
    SANDBOX_BEGIN;
    myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    system("ls -al file.txt | head -c 10 > PERM_A.txt");
    system("ls -al newfile.txt | head -c 10 > PERM_B.txt");
    if (system("diff PERM_A.txt PERM_B.txt") != 0){
        push_info_msg(_("The permission of the two files are not equals."));
        CU_FAIL();
    }else{
        set_tag("permission");
    }
}



/*
 * Test with some bytes in the file
 */
void test_file() {
    set_test_metadata("q1", _("Test with non-empty file"), 2);
    gen_file(122);
    int ret = 0;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    system("chmod 644 newfile.txt"); //If student does not set permission correctly, we set it to avoid test error
    // S_IRUSR|S_IWUSR = 644
    if (system("diff file.txt newfile.txt") != 0){
        push_info_msg(_("You copy is not identical."));
        CU_FAIL();
    }else{
        set_tag("copy");
    }
    if (ret != 0){
        push_info_msg(_("You do not return 0 when the copy should be successful."));
        CU_FAIL();
    }
}

/*
 * Test with some bytes in the file
 */
void test_file_write_fail() {
    set_test_metadata("q1", _("Test with normal file (with some syscall failures)"), 1);
    gen_file(125);
    int ret = 0;
    
    monitored.write = true;
    failures.write = FAIL_FIRST;
    failures.write_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt", "newfile.txt");
    SANDBOX_END;

    if (ret != -1){
        push_info_msg(_("You do not return -1 when a fail occurs with read() or write()"));
        set_tag("failure_handling");
        CU_FAIL();
    }
}

void test_stat_fail() {
    set_test_metadata("q1", _("Test return value when stat() fails"), 1);
    gen_file(125);
    int ret = 0;
    
    monitored.stat = true;
    failures.stat = FAIL_ALWAYS;
    failures.stat_ret = -1;
    
    SANDBOX_BEGIN;
    ret = myfunc("file.txt", "newfile.txt");
    SANDBOX_END;

    if (ret != -1){
        push_info_msg(_("You do not return -1 when a fail occurs with stat()"));
        set_tag("failure_handling");
        CU_FAIL();
    }
}

/*
 * Test with some bytes in the file with close()
 */
void test_close() {
    set_test_metadata("q1", _("Test close()."), 1);
    gen_file(2);
    
    monitored.close = true;
    monitored.open = true;
    SANDBOX_BEGIN;
    myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    int close_tag = 0;
    if (stats.close.called != 2){
        push_info_msg(_("You did not close() the two files."));
        close_tag++;
        CU_FAIL();
    }
    
    //We fail a close()
    gen_file(3);
    int ret = 0;
    monitored.close = true;
    failures.close = FAIL_TWICE;
    failures.close_ret = -1;
    SANDBOX_BEGIN;
    ret = myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    if(ret != -1){
        push_info_msg(_("When close() fails, your code does not return -1."));
        close_tag++;
        CU_FAIL(); 
    }
    if(close_tag == 0){
        set_tag("close");
    }
}


/*
 * Test original file not modified (content and permissions)
 */
void test_original_integrity() {
    set_test_metadata("q1", _("Test original file not modified"), 1);
    gen_file(70);
    system("cp file.txt file_original.txt");
    system("ls -al file.txt | head -c 10 > PERM_FILE_1.txt");
    
    SANDBOX_BEGIN;
    myfunc("file.txt", "newfile.txt");
    SANDBOX_END;
    
    if (system("diff file.txt file_original.txt") != 0){
        push_info_msg(_("You can not modify the original file"));
        CU_FAIL();
        set_tag("original_modif");
    }
    system("ls -al file.txt | head -c 10 > PERM_FILE_2.txt");
    if (system("diff PERM_FILE_1.txt PERM_FILE_2.txt") != 0){
        push_info_msg(_("You can not modify the permissions of the original file."));
        CU_FAIL();
        set_tag("original_modif");
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system, set_tag, fopen, fread, fwrite, fclose, fstat);
    RUN(test_fail_open, test_empty_file, test_file_permission, test_file, test_file_write_fail, test_stat_fail, test_original_integrity, test_close);
}
