#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

void * sleep_malloc(size_t s){
    int count = 0;
    void* p = NULL;
    while(!p && count < 10){
        p = malloc(s);
        if(p) return p;
        sleep(5);
        count ++;
    }
    return NULL;
}
