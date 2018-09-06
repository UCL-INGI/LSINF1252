#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "student_code.h"
#include "CTester/CTester.h"

person_t* init_p(char* name, int age, int salary){
    person_t *p = (person_t*)malloc(sizeof(person_t));
    if(p == NULL)
        return NULL;
    
    p->age = age;
    p->salary = salary;
    
    char* new_name = (char*)malloc(sizeof(char)*strlen(name));
    if(new_name == NULL){
        free(p);
        return NULL;
    }
    strncpy(new_name, name, strlen(name));
    p->name = new_name;
    
    return p;
}

university_t* init_u(person_t rector, const char* city, int creation){
    university_t *u = (university_t*)malloc(sizeof(university_t));
    if(u == NULL)
        return NULL;
    
    u->rector = rector;
    u->creation = creation;

    char* city = (char*)malloc(sizeof(char)*strlen(city));
    if(city == NULL){
        free(u);
        free(p->name);
        free(p);
        return NULL;
    }
    strncpy(new_city, city, strlen(city));
    u->city = new_city;

    return u;
}

void test_success(){
    
    set_test_metadata("free_all", _("All the data correctly initialised"), 1);
    
    person_t* p = init_p("Vincent Blondel", 53, 6000);
    if(p == NULL)
        return;
    
    university_t* u = init_u(p, "Louvain-la-Neuve", 1425);
    if(u == NULL)
        return;
    
    int ret = -2;
    
    
    
}

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_success,test_fail_first,test_fail_twice,test_fail_five,test_fail_eight,test_fail_ten,test_fail_always);
}
