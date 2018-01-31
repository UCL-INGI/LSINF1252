#include <stdlib.h>
#include <stdio.h>
#include "student_code.h"

void * sleep_malloc(size_t s){
    int count = 0;
    void* p = NULL;
    while(!p && count < 10){
        p = malloc(s);
        if(!p) sleep(5);
        count ++;
    }
    return p;
}
