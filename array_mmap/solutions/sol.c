#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include "student_code.h"

/*
 * @filename: the file to be manipulated, filename != NULL
 * @pre: the length of the file is an entire multiple of
 *      the size of student_t
 * @return: the pointer to the head of the linked list,
 *         NULL in case of error
 * you cannot use fopen, read, fread, fgetc, fgets
 * which means that you have to use mmap
 */
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
        munmap(map, length);
        close(fd);
        return NULL;
    }

    memcpy((void*) root, (void*) &map[0], sizeof(student_t));

    root->next = NULL;

    student_t* previous = root;

    int i;
    for(i = 1; i < length_by_struct; i++){
        student_t* new = (student_t*)malloc(sizeof(student_t));
        if(new == NULL){
            student_t* failure_runner = root;
            student_t* failure_previous = NULL;

            while(failure_runner != NULL){
                failure_previous = failure_runner;
                failure_runner = failure_runner->next;
                free(failure_previous);
            }

            munmap(map, length);
            close(fd);
            return NULL;
        }
        memcpy((void*) new, (void*) &map[i], sizeof(student_t));
        previous->next = new;
        new->next = NULL;
        previous = new;
    }

    munmap(map, length);
    close(fd);

    return root;
}