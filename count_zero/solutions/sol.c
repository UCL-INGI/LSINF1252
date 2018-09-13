#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

/*
 * @str: the string to be counted
 * @length: the length of the string, length > 0
 * @return: the number of occuence of '\0'
 *         -1 in case of error
 */
int counting_zero(char* str, int length){
    int count = 0;
    
    int i;
    for(i = 0; i < length; i++){
        if(str[i] = '\0')
            count++;
    }
    
    return count;
}