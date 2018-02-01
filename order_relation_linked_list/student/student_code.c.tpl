#include<stdio.h>
#include<stdlib.h>

void modem_read(void* buffer, int* buf_len){
    *buf_len = 64;
}

int has_42() {
    @@modem@@
}
