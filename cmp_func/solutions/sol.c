#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

int cmp_func(uint8_t (*fun1)(uint8_t), uint8_t (*fun2)(uint8_t)){
    int i;
    for(i = 0; i < 256; i++){
        if((*fun1)(i) != (*fun2)(i)) return 0;
    }
    return 1;
}
