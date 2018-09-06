#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "student_code.h"
#include "/CTester/CTester.h"

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
    
    char* rector_name = "Vincent Blondel";
    char* city_name = "Louvain-la-Neuve";
    
    person_t* p = init_p(rector_name, 53, 6000);
    if(p == NULL)
        return;
    
    university_t* u = init_u(p, city_name, 1425);
    if(u == NULL)
        return;
    
    size_t total_size = sizeof(u) + sizeof(p) + sizeof(rector_name) + sizeof(city_name);
    
    int ret = -2;
    
    monitored.free = true;
    failures.free = FAIL_NEVER;
    
    size_t start = stats.memory.used;
    
    SANDBOX_BEGIN;
    ret = free_all(u);
    SANDBOX_END;
    
    size_t freed_size = start - stats.memory.used;
    
    CU_ASSER_EQUAL(freed_size, total_size);
    
    if(u == NULL){
        push_info_msg(_("Free totalement meme ici"));
    }
    else{
        push_info_msg(_("Pas free totalemenjt"));
        free_all(u);
    }
    
}

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_success);
}
