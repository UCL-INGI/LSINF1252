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

void test_get() {
    set_test_metadata("q1", _("Test with normal file"), 2);
    gen_file(1000);
    
    system("cp file.txt file_copy.txt");
    
    int should_count_read = 0;
    monitored.read = true; 
    for(int i = 0; i < 1000; i+=50){
        should_count_read++;
        int ret = 0;
        
        SANDBOX_BEGIN;
        ret = get("file.txt", i);
        SANDBOX_END;
        
        if(ret != get_value_by_index(i)){
            push_info_msg(_("You do not return the correct value."));
            CU_FAIL(); 
        }   
    }
    if(stats.read.called > should_count_read){
        set_tag("too_many_op");
        push_info_msg(_("You perform too many read()."));
        CU_FAIL();  
    }
    
    if(system("diff file.txt file_copy.txt") != 0){
        push_info_msg(_("You have modified the file when reading it..."));
        CU_FAIL();
    }
}

void test_set() {
    set_test_metadata("q2", _("Test with normal file"), 2);
    gen_file(1000);
    
    int should_count_write = 0;
    monitored.write = true;        
    for(int i = 0; i < 1000; i+=50){ 
        should_count_write++;
        SANDBOX_BEGIN;
        set("file.txt", i, 2222+i);
        SANDBOX_END;  
    }
    
    if(stats.write.called > should_count_write){
        set_tag("too_many_op");
        push_info_msg(_("You perform too many write()."));
        CU_FAIL();  
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
            break;
        }
    }
    for(int i = 0; i < 1000; i++){
        if (i % 50 != 0){
        	lseek(fd, (off_t) i*sizeof(int), SEEK_SET);
        	int res;
        	read(fd, (void *) &res, sizeof(int));
        	if (res != get_value_by_index(i)){
            	push_info_msg(_("You have modified some wrong elements in the array"));
            	CU_FAIL();
                break;
        	}
        }
    }
    close(fd);
}

int close_tag = 0;
void test_close_q1(){
    set_test_metadata("q1", _("Test close"), 1);
    monitored.close = true;        
    SANDBOX_BEGIN;
    get("file.txt", 0);
    SANDBOX_END;
    if (stats.close.called != 1){
        push_info_msg(_("You did not close the file."));
        CU_FAIL();
        close_tag++;
    }
}

void test_close_q2(){
    set_test_metadata("q2", _("Test close"), 1);
    monitored.close = true;        
    SANDBOX_BEGIN;
    set("file.txt", 0,0);
    SANDBOX_END;
    if (stats.close.called != 1){
        push_info_msg(_("You did not close the file."));
        CU_FAIL();
        close_tag++;
    }
    if (close_tag == 0)
        set_tag("close");
}

void test_get_oob() {
    set_test_metadata("q1", _("Test get out of bound"), 1);
    gen_file(10);    
    int ret = 0;
        
    SANDBOX_BEGIN;
    ret = get("file.txt", 200);
    SANDBOX_END;
        
    if(ret != -2){
        push_info_msg(_("You do not return -2 when index is bigger than the size of the array."));
        CU_FAIL(); 
    }   
}

void test_get_fail() {
    set_test_metadata("q1", _("Test get fail"), 1);
    gen_file(100);    
    int ret = 0;
    
    monitored.read = true;
    failures.read=FAIL_FIRST;
    failures.read_ret = -1;
    SANDBOX_BEGIN;
    ret = get("file.txt", 50);
    SANDBOX_END;
        
    if(ret != -1){
        push_info_msg(_("You do not return -1 when the read() function fails."));
        CU_FAIL(); 
    }   
}

void test_open_q1_fail(){
    set_test_metadata("q1", _("Test open fail"), 1);
    monitored.open = true;
    failures.open=FAIL_FIRST;
    failures.open_ret = -1;   
    int ret = 0;
    SANDBOX_BEGIN;
    ret = get("file_no_exits.txt", 3);
    SANDBOX_END;
    
    if (ret != 1){
        push_info_msg(_("You do not return -1 when open() fails."));
        CU_FAIL();
    }
}

int main(int argc, char** argv){
    BAN_FUNCS(system, set_tag);
    RUN(test_get, test_set, test_close_q1, test_close_q2, test_get_oob, test_get_fail, test_open_q1_fail);
}
