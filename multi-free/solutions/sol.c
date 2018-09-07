#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

void free_all(university_t* u){
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
}
