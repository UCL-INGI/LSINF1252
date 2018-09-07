#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "student_code.h"
#include "../CTester/CTester.h"

person_t* init_p(char* name, int age, int salary){
    person_t *p = (person_t*)malloc(sizeof(person_t));
    if(p == NULL)
        return NULL;
    
    p->age = age;
    p->salary = salary;
    
    if(name != NULL){
        char* new_name = (char*)malloc(sizeof(char)*strlen(name));
        if(new_name == NULL){
            free(p);
            return NULL;
        }
        strncpy(new_name, name, strlen(name));
        
        p->name = new_name;
    }
    else{
        p->name = NULL;
    }
    
    return p;
}

university_t* init_u(person_t* rector, const char* city, int creation){
    university_t *u = (university_t*)malloc(sizeof(university_t));
    if(u == NULL)
        return NULL;
    
    u->rector = rector;
    u->creation = creation;
    
    if(city != NULL){
        char* new_city = (char*)malloc(sizeof(char)*strlen(city));
        if(new_city == NULL){
            free(u->rector->name);
            free(u->rector);
            free(u);
            return NULL;
        }
        strncpy(new_city, city, strlen(city));
        u->city = new_city;
    }
    else{
        u->city = NULL;
    }

    return u;
}

university_t* init_new(){
    char* rector_name = "Vincent Blondel";
    char* city_name = "Louvain-la-Neuve";
    
    
    person_t* p = init_p(rector_name, 53, 6000);
    if(p == NULL)
        return NULL;
    
    university_t* u = init_u(p, city_name, 1425);
    if(u == NULL)
        return NULL;
    return u;
}

int free_a(university_t* u){
    free(u->rector->name);
    free(u->rector);
    free(u->city);
    free(u);
    return 0;
}

void test_success(){
    
    set_test_metadata("free_all", _("Testing in a normal case"), 1);
    
    university_t* u = init_new();
    if(u == NULL)
        return;
    
    int ret = -2;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    ret = free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(ret,0);
    if(ret != 0){
        push_info_msg("You should return 0 in a normal case");
    }
    
    CU_ASSERT_EQUAL(stats.free.called, 4);
    if(stats.free.called != 4){
        push_info_msg(_("You did not free all the memory"));
    }
    
    
    
}

void test_u_null(){
    set_test_metadata("free_all", _("Testing when u is NULL"), 1);
    
    university_t* u = NULL;
    
    int ret = -2;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    ret = free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(ret,-1);
    if(ret != -1){
        push_info_msg(_("Your function does not work for NULL pointers"));
    }
}

void test_rector_null(){
    set_test_metadata("free_all", _("Testing when there is no rector"), 1);
    
    university_t* u = init_u(NULL, "Ottawa", 1848);
    if(u == NULL)
        return;
    
    int ret = -2;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    ret = free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(ret, 0);
    if(ret != 0){
        push_info_msg(_("Your function does not return the right value when there is no rector"));
    }
    
    CU_ASSERT_EQUAL(stats.free.called, 2);
    if(stats.free.called != 2){
        push_info_msg(_("You did not free all the memory"));
    }
}

void test_strings_null(){
    set_test_metadata("free_all", _("Testing when the strings are NULL"), 1);
    
    person_t* p = init_p(NULL, 50, 10000);
    if(p == NULL)
        return;
    university_t* u = init_u(p,NULL,1800);
    if(u == NULL)
        return;
    
    int ret = -2;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    ret = free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(ret,0);
    if(ret != 0){
        push_info_msg(_("Your function does not return the right value when the strings are NULL"));
    }
    CU_ASSERT_EQUAL(stats.free.called,2);
    if(stats.free.called > 2){
        push_info_msg(_("Your code does not work correctly when the strings are NULL"));
    }
    if(stats.free.called < 2){
        push_info_msg(_("You did not free all the memory"));
    }
    
}

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_success, test_u_null, test_rector_null, test_strings_null);
}
