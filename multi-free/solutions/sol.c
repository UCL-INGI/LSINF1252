#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"

/*
 * Frees all the memory associated with u
 * @pre: u != NULL
 */
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

/*
 * @return: a pointer to the university structure containing the elements
 */
university_t* init_all(char* city, int creation, char* rectname, int age, int salary){
    //Assuming that the strings != NULL
    
}
