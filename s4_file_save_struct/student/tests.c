// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

point_t* gen_struct(int size){
    point_t *tab = malloc(size*sizeof(point_t));
    if (tab == NULL){
        CU_FAIL("Can not initialize test suite");
        return (point_t*)NULL;
    }
    for (int i = 0; i < size; i++){
        tab[i].x = i+i;
        tab[i].y = i+i+i;
        tab[i].z = i+i*i;
    }
    return tab;
}

/*
 * Test with open fail
 */
void test() {
    set_test_metadata("q1", _("Test write array struct"), 1);
    int size = 6;
    int ret = 0;
    point_t* tab = gen_struct(size);

    SANDBOX_BEGIN;
    ret = save(tab, size, "file.txt");
    SANDBOX_END;
    
    free(tab);
    tab = NULL;
    
    //Regenerate the struct in case student modified it
    tab = gen_struct(size);
    int fd = open("file.txt",O_RDONLY); 
    if(fd == -1) 
        CU_FAIL("Can not initialize test suite");

    point_t s;
    for(int i = 0; i < size; i++){
        if ((int)read(fd, (void *) &s, sizeof(point_t)) < 1){
            push_info_msg(_("You did not write all content of the array in the file."));
            CU_FAIL();
        }
        if (tab[i].x != s.x || tab[i].y != s.y || tab[i].z != s.z){
            push_info_msg(_("You did not write the array of struct correctly in the file."));
            CU_FAIL();
        }
    }
    free(tab);
    close(fd);
    if(ret != 0){
        push_info_msg(_("You did not return 0 when everything occurs fine."));
        CU_FAIL(); 
    }
}


/*
 * Test with some integers in the file with close()
 */
void test_close() {
    set_test_metadata("q1", _("Test close()."), 1);
    int size = 6;
    point_t* tab = gen_struct(size);
    
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
    BAN_FUNCS(system);
    RUN(test, test_close);
}
