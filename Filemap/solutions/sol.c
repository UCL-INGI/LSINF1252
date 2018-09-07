#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

int count(char* filename, char c){
    
    int fd = open(filename, O_RDONLY);
    if(fd == -1){
        return -1;
    }
    
    int count =0;
    
    struct stat st;
    stat(filename, &st);
    int length = st.st_size;
    if(length == 1){
        return 0;
    }
    
    char* buffer = (char*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
    if(buffer == NULL){
        perror("Erreur");
        return -1;
    }
    
    int i;
    for(i = 0; i < length; i++){
        if(buffer[i] == c)
            count++;
    }
    
    munmap(buffer, length);
    
    close(fd);
    
    return count;
    
}
