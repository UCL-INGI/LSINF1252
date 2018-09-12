#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include "student_code.h"

student_t *load_linked_list(char* filename){
    int fd = open(filename, O_RDWR, S_IRUSR|S_IWUSR);
    if(fd == -1){
        return NULL;
    }
    
    struct stat st;
    fstat(fd, &st);
    
    int length = st.st_size;
    
    if(length == 0){
        close(fd);
        return NULL;
    }
    
    int length_by_struct = length/sizeof(student_t);
    
    student_t* map = (student_t*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
    if(map == NULL){
        close(fd);
        return NULL;
    }
    
    student_t* root = (student_t*)malloc(sizeof(student_t));
    if(root == NULL){
        munmap(map);
        close(fd);
        return -1;
    }
    
    root->
}
