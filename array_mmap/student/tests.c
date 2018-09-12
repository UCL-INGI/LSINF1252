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
    for(i = 0; i < nb; i++){
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
    set_test_metadata("load_linked_list",_("Test in a normal case"),1);
    student_t* sol = creat_linked_list(12351600, 5);
    if(sol == NULL){
        return;
    }
    student_t* root_ret;
    
    SANDBOX_BEGIN;
    root_ret = load_linked_list("normal_case.txt");
    SANDBOX_END;
    
    int cmp = same_linked_list(sol, root_ret);
    
    CU_ASSERT_EQUAL(cmp, 1);
    if(cmp != 1){
        push_info_msg(_("Your linked list is not what was expected"));
    }
}

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_normal_case);
}
