#include<stdio.h>
#include<stdlib.h>

void modem_read(void* buffer, int* buf_len){
    *buf_len = 64;
}

int has_42() {
    void * buf = malloc(256);
    if (!buf) return -1;
    int a = 0;
    modem_read(buf,&a);
    int i, res;
    int8_t* b = (int8_t*) buf;
    for(i = 0 ; i < a; i ++){
        if(( res = *(b + i)) == 42) {
            free(buf);
            return 1;
        }
    }
    free(buf);
    return 0;
}
