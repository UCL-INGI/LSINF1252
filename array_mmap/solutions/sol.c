#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

int free_all(university_t* u){
    if(u == NULL){
        return -1;
    }

    if(u->rector != NULL){
        if(u->rector->name != NULL){
            free(u->rector->name);
        }
        free(u->rector);
    }
    if(u->city != NULL){
        free(u->city);
    }
    free(u);

    return 0;
}
