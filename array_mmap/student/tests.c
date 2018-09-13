#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include "CTester/CTester.h"
#include "student_code.h"

void free_all(student_t* root){
    student_t* runner = root;
    while(runner != NULL){
        root = runner;
        runner = runner->next;
        free(root);
    }
}

student_t *create_linked_list(int noma_depart, int nb){
    if(nb <= 0)
        return NULL;
    
    student_t* root = (student_t*)malloc(sizeof(student_t));
    if(root == NULL)
        return NULL;
    
    root->noma = noma_depart;
    noma_depart += 10000;
    root->next = NULL;
    
    student_t* previous = root;
    
    int i;
    for(i = 1; i < nb; i++){
        student_t* new = (student_t*)malloc(sizeof(student_t));
        if(new == NULL){
            free_all(root);
            return NULL;
        }
        previous->next = new;
        new->next = NULL;
        new->noma = noma_depart;
        noma_depart += 10000;
        previous = new;
    }
    
    return root;
}

int create_file(char* filename, student_t* root){
    int fd = open(filename, O_CREAT|O_RDWR, S_IRUSR,S_IWUSR);
    if(fd == -1){
        return -1;
    }
    
    student_t* runner = root;
    while(runner != NULL){
        int error = write(fd, (void*) runner, sizeof(student_t));
        if(error == -1){
            free_all(root);
            close(fd);
            return -1;
        }
        runner = runner->next;
    }
    
    close(fd);
    
    return 0;
}

int same_linked_list(student_t* st1, student_t* st2){
    while(st1 != NULL && st2 != NULL){
        if(st1->noma != st2->noma){
            return 0;
        }
        st1 = st1->next;
        st2 = st2->next;
    }
    if(st1 == NULL && st2 == NULL){
        return 1;
    }
    return 0;
}

void test_normal_case(){
    set_test_metadata("load_linked_list", _("Test with a file containing many elements"), 1);
    
    int nb_elem = 5;
    
    student_t* sol = create_linked_list(12351600, nb_elem);
    if(sol == NULL){
        return;
    }
    student_t* root_ret;
    
    
    monitored.open = true;
    monitored.malloc = true;
    monitored.free = true;
    
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("normal_case.txt");
    SANDBOX_END;
    
    int memory_used = stats.memory.used - start;
    
    monitored.open = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int memory_used_sol = sizeof(student_t) * nb_elem;
    
    int cmp = same_linked_list(sol, root_ret);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your linked list is not what was expected"));
    }
    
    CU_ASSERT_EQUAL(memory_used, memory_used_sol);
    if(memory_used != memory_used_sol){
        push_info_msg(_("You did not allocate the wright amount of memory"));
    }
}

void test_open_fails(){
    set_test_metadata("load_linked_list", _("Test with fail of open"), 1);
    
    student_t* root_ret;
    
    failures.open = FAIL_ALWAYS;
    
    monitored.open = true;
    monitored.malloc = true;
    monitored.free = true;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("normal_case.txt");
    SANDBOX_END;
    
    monitored.open = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work when open fails"));
    }
}

void test_empty_file(){
    set_test_metadata("load_linked_list", _("Test with an empty file"), 1);
    
    student_t* root_ret;
    
    monitored.open = true;
    monitored.malloc = true;
    monitored.free = true;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("empty_file.txt");
    SANDBOX_END;
    
    int cmp = same_linked_list(root_ret, NULL);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work with an empty file"));
    }
}

void test_one_element(){
    set_test_metadata("load_linked_list", _("Test with a file containing one element"), 1);
    
    int nb_elem = 1;
    
    student_t* sol = create_linked_list(46691600, nb_elem);
    
    student_t* root_ret;
    
    monitored.open = true;
    monitored.malloc = true;
    monitored.free = true;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("one_element.txt");
    SANDBOX_END;
    
    monitored.open = false;
    monitored.malloc = false;
    monitored.free = false;
    
    int cmp = same_linked_list(root_ret, sol);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your function does not work with a file containing one element"));
    }
}
/*
void test_malloc_fails_first time(){
    set_test_metadata("load_linked_list"), _("Test whith malloc failing first time"));
}
*/

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_open_fails, test_empty_file, test_one_element, test_normal_case);
}
