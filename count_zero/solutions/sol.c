#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

int counting_zero(char* str, int length){
    int count = 0;
    int i;
    for(i = 0; i < length; i++){
        if(str[i] = '\0')
            count++;
    }
    return count;
}