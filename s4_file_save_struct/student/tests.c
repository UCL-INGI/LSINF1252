// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

point_t* gen_struct(int size){
    point_t *tab = malloc(size*sizeof(point_t));
    if (tab == NULL){
        CU_FAIL(_("Error, can not initialise test file"));
        return (point_t*)NULL;
    }
    for (int i = 0; i < size; i++){
        tab[i].x = i+i;
        tab[i].y = i+i+i;
        tab[i].z = i+i*i;
    }
    return tab;
}

void _test() {
    int size = 6;
    int ret = 0;
    point_t* tab = gen_struct(size);
    
    monitored.open = true;
    monitored.close = true;
    monitored.write = true;
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;

    free(tab);
    tab = NULL;
    
    if (stats.open.called != 1) {
        CU_FAIL();
        push_info_msg(_("You should call open() only once."));
    }
    if (stats.write.called != 1) {
        CU_FAIL();
        push_info_msg(_("You should call write() only once."));
        set_tag("too_many_op");
    }
    if (stats.close.called != 1) {
        CU_FAIL();
        push_info_msg(_("You should call close() only once."));
    }
    
    //Regenerate the struct in case student modified it
    tab = gen_struct(size);
    int fd = open("file.txt",O_RDONLY); 
    if(fd == -1) { 
        CU_FAIL(_("Error, can not initialise test file"));
        push_info_msg(_("You didn't create the file."));
        return;
    }

    point_t s;
    for(int i = 0; i < size; i++){
        if ((int)read(fd, (void *) &s, sizeof(point_t)) < 1){
            push_info_msg(_("You did not write all content of the array in the file."));
            CU_FAIL();
            return;
        }
        else if (tab[i].x != s.x || tab[i].y != s.y || tab[i].z != s.z){
            push_info_msg(_("You did not write the array of struct correctly in the file."));
            CU_FAIL();
            return;
        }
    }
    
    if ((int)read(fd, (void *) &s, 1) > 0) {
        CU_FAIL();
        push_info_msg(_("There's data in the file, after the end of the array."));
    }
    free(tab);
    close(fd);
    if(ret != 0){
        push_info_msg(_("You did not return 0 when everything should work."));
        CU_FAIL(); 
    }   
}

void test_no_file() {
    set_test_metadata("q1", _("Test writing the struct, no file already created"), 2);
    system("rm file.txt");
    _test();
}

void test_with_file() {
    set_test_metadata("q1", _("Test writing the struct, with file already created"), 1);
    system("rm file.txt");
    system("echo \"INTXINTYINTZINTXINTYINTZINTXINTYINTZINTXINTYINTZINTXINTYINTZINTXINTYINTZINTXINTYINTZINTXINTYINTZ\" > file.txt");
    _test();
}

void test_close() {
    set_test_metadata("q1", _("Test close()."), 1);
    int size = 6;
    point_t* tab = gen_struct(size);
    system("rm file.txt");
    
    monitored.close = true;
    monitored.open = true;
    SANDBOX_BEGIN;
    save(tab, size, "file.txt");
    SANDBOX_END;
    
    free(tab);
    tab = NULL;
    
    int close_ok = 0;
    if (stats.close.called != 1){
        push_info_msg(_("You did not close() the file."));
        close_ok++;
        CU_FAIL();
    }if(stats.open.last_return != stats.close.last_params.fd){
        push_info_msg(_("The close() does not close the file you opened before."));
        close_ok++;
        CU_FAIL();
    }
    
    
    //We fail the close()
    system("rm file.txt");
    tab = gen_struct(size);
    int ret = 0;
    monitored.close = true;
    failures.close = FAIL_FIRST;
    failures.close_ret = -1;
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;
    
    if(ret != -3){
        push_info_msg(_("When close() fails, your code does not return -3."));
        close_ok++;
        CU_FAIL(); 
    }
    if(close_ok == 0){
        set_tag("close");
    }
    free(tab);
    tab = NULL;
}

void test_open() {
    set_test_metadata("q1", _("Test open()."), 1);
    int size = 6;
    point_t* tab = gen_struct(size);
    int ret = 0;
    system("rm file.txt");
    
    monitored.open = true;
    failures.open = FAIL_FIRST;
    failures.open_ret = -1;
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;
    
    free(tab);
    tab = NULL;
    int tag_open = 0;
    if(ret != -1){
        push_info_msg(_("You do not return -1 when open() fails."));
        CU_FAIL();
        tag_open++;
    }
    
    //Test if open() return a correct file descriptor.
    system("rm file.txt");
    tab = gen_struct(size);
    monitored.open = true;
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;

    if(stats.open.last_return <= 2){
        push_info_msg(_("When the open() should be fine, your code returns -1."));
        CU_FAIL();
        tag_open++;
    }
    if(tag_open == 0){
        set_tag("open");
    }
    free(tab);
    tab = NULL;
}

void test_write_fail() {
    set_test_metadata("q1", _("Test write() fail."), 1);
    int size = 6;
    point_t* tab = gen_struct(size);
    int ret = 0;
    system("rm file.txt");
    
    monitored.write = true;
    failures.write = FAIL_ALWAYS;
    failures.write_ret = -1;
    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;
    
    free(tab);
    tab = NULL;
    
    if(ret != -2){
        push_info_msg(_("You do not return -2 when write() fails."));
        set_tag("failure_handling");
        CU_FAIL();
    }
}

int main(int argc,char** argv){
    BAN_FUNCS(system, fopen, fread, fwrite, fclose);
    RUN(test_no_file, test_with_file, test_close, test_open, test_write_fail);
}
