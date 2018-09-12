#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
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
    }
    
    return 0;
}

void test_success(){
    CU_ASSERT_EQUAL(1,1);
}

int main(int argc,char* argv[])
{
    student_t* root1 = create_linked_list(12351600, 5);
    if(root1 == NULL)
        return;
    create_file("test1.dat", root1);
    BAN_FUNCS();
    RUN(test_success);
}
