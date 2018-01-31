#include<stdio.h>
#include<stdlib.h>

void modem_read(void* buffer, int* buf_len){
    *buf_len = 64;
}

int has_42() {
    void * buf = malloc(256);
    int a = 0;
    modem_read(buf,&a);
    int i;
    for(i = 0 ; i < a; i ++){
        if((int res = *((int8_t*) (buf + i))_ == 42) {
            free(buf);
            return 1;
        }
    }
    free(buf);
    return 0;
}
