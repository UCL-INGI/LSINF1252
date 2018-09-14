#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "student_code.h"
#include "CTester/CTester.h"

/*

I tried to use stats.memory.used to check if the memory has been freed, but it did not work. Doing the following thing did not work:

int start = stats.memory.used;

SANDBOX_BEGIN;
free_all(u);
SANDBOX_END;

int freed = start - stats.memory.used;

Everytime, freed = 0. Instead, I had to check if all the memory pointed by the pointers has been freed individually.

*/


/*
 * Creates a person_t, with his name, age and salary
 * @return: a pointer to the new malloced person_t, NULL in case of error
 */
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


/*
 * Creates a university_t, with its rector (person_t), its city and
 * the date of the creation
 * @return: a pointer to the new malloced university_t, NULL in case of error
 */
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

/*
 * Creates a new university_t with basic informations
 * @return: a pointer to the new malloced university_t, NULL in case of error
 */
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

/*
 * Frees all the memory associated with u
 */
int free_a(university_t* u){
    free(u->rector->name);
    free(u->rector);
    free(u->city);
    free(u);
    return 0;
}

/*
 * Checks if two universities have the same values (not checking for NULL cases)
 */

/*
int uniEquals(university_t* u1, university_t* u2){
    return (!strcmp(u1->city,u2->city) && u1->creation==u2->creation && !strcmp((u1->rector)->name,(u2->rector)->name) && (u1->rector)->age==(u2->rector)->age && (u1->rector)->salary==(u2->rector)->salary);
}
*/

int uniEquals(university_t* u1, university_t* u2){
    int cityEquals = !strcmp(u1->city, u2->city);
    int creationEquals = u1->creation == u2->creation;
    int rector_nameEquals = !strcmp((u1->rector)->name,(u2->rector)->name);
    int rector_ageEquals = u1->rector->age == u2->rector->age;
    int rector_salaryEquals = u1->rector->salary == u2->rector->salary;
    
    return cityEquals && creationEquals && rector_nameEquals && rector_ageEquals && rector_salaryEquals;
}

/*
 * @return: 1 if @array contains @value, 0 otherwise
 */
int containsArray(int* array, int size, int value){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(array[i] == value)
            return 1;
    }
    return 0;
}

/*
 * Test with a normal case: all the memory should be freed
 */
void test_free_success(){

    set_test_metadata("free_all", _("Testing in a normal case"), 1);

    university_t* u = init_new();
    if(u == NULL)
        return;

    university_t* u_c = u;
    char* city_c = u->city;
    person_t* p_c = u->rector;
    char* name_c = u->rector->name;

    monitored.free = true;
    monitored.malloc = true;

    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;

    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called){
        push_info_msg(_("Why are you using malloc ?"));
    }

    CU_ASSERT_EQUAL(stats.free.called, 4);
    if(stats.free.called != 4){
        push_info_msg(_("You did not free all the memory"));
        set_tag("not_free");
    }

    int verif_u = malloced(u_c);
    int verif_city = malloced(city_c);
    int verif_rector = malloced(p_c);
    int verif_name = malloced(name_c);

    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_city, false);
    CU_ASSERT_EQUAL(verif_rector, false);
    CU_ASSERT_EQUAL(verif_name, false);
    
    int not_free = 0;

    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
        not_free++;
    }
    if(verif_city != false){
        push_info_msg(_("You did not free the memory associated with the city"));
        not_free++;
    }
    if(verif_rector != false){
        push_info_msg(_("You did not free the memory associated with the rector"));
        not_free++;
    }
    if(verif_name != false){
        push_info_msg(_("You did not free the memory associated with the rector's name"));
        not_free++;
    }
    
    if(not_free){
        set_tag("not_free");
    }

    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
        push_info_msg(_("The last free you should do is on the struct"));
}

/*
 * Test when the rector is NULL
 */
void test_free_rector_null(){
    set_test_metadata("free_all", _("Testing when there is no rector"), 1);

    university_t* u = init_u(NULL, "Ottawa", 1848);
    if(u == NULL)
        return;

    university_t* u_c = u;
    char* city_c = u->city;

    monitored.free = true;
    monitored.malloc = true;

    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;

    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called){
        push_info_msg(_("Why are you using malloc ?"));
    }

    CU_ASSERT_EQUAL(stats.free.called, 2);
    if(stats.free.called > 3){
        push_info_msg(_("Your function does not work correctly when the rector is NULL"));
        set_tag("wrong_args_handling");
    }
    else if(stats.free.called < 2){
        push_info_msg(_("You did not free all the memory"));
        set_tag("wrong_args_handling");
    }

    int verif_u = malloced(u_c);
    int verif_city = malloced(city_c);

    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_city, false);
    
    int not_free = 0;

    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
        not_free++;
    }
    if(verif_city != false){
        push_info_msg(_("You did not free the memory associated with the city"));
        not_free++;
    }
    
    if(not_free){
        set_tag("not_free");
    }

    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
        push_info_msg(_("The last free you should do is on the struct"));
}

/*
 * Test when the strings (city and rector's name) are NULL
 */
void test_free_strings_null(){
    set_test_metadata("free_all", _("Testing when the strings are NULL"), 1);

    person_t* p = init_p(NULL, 50, 10000);
    if(p == NULL)
        return;
    university_t* u = init_u(p,NULL,1800);
    if(u == NULL)
        return;

    university_t* u_c = u;
    person_t* p_c = u->rector;

    monitored.free = true;
    monitored.malloc = true;

    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;

    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.malloc.called,0);
    if(stats.malloc.called){
        push_info_msg(_("Why are you using malloc ?"));
    }

    CU_ASSERT_EQUAL(stats.free.called,2);
    if(stats.free.called > 2){
        push_info_msg(_("Your code does not work correctly when the strings are NULL"));
        set_tag("wrong_args_handling");
    }
    if(stats.free.called < 2){
        push_info_msg(_("You did not free all the memory"));
        set_tag("wrong_args_handling");
    }

    int verif_u = malloced(u_c);
    int verif_rector = malloced(p_c);

    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_rector, false);
    
    int not_free = 0;

    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
        not_free++;
    }
    if(verif_rector != false){
        push_info_msg(_("You did not free the memory associated with the rector"));
        not_free++;
    }
    
    if(not_free){
        set_tag("not_free");
    }

    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
        push_info_msg(_("The last free you should do is on the struct"));
}

void test_init_normal_case(){
    set_test_metadata("init_all", _("Testing the behaviour of the function in a normal case"),1);

    university_t* ucl = init_new();
    char* city = malloc(sizeof(char)*17);
    char* name = malloc(sizeof(char)*16);

    if(!ucl || !city || !name){
        if(ucl) free_a(ucl);
        if(city) free(city);
        if(name) free(name);
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(city,"Louvain-la-Neuve");
    strcpy(name,"Vincent Blondel");
    
    int begin = logs.malloc.n;
    monitored.free = true;
    monitored.malloc = true;
    
    SANDBOX_BEGIN;
    university_t* ret = init_all(city, creation, rectname, age, salary);
    SANDBOX_END;

    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.free.called, 0);
    if(stats.free.called)
        push_info_msg(_("Why did you use free ?"));

    CU_ASSERT_EQUAL(stats.malloc.called, 4);
    if(stats.malloc.called != 4){
        push_info_msg(_("You should call malloc 4 times !"));
        return;
    }
    else{ // did he allocate enough memory ? or not too much ?
        int size[4];
        size[0] = logs.malloc.log[begin].size;
        size[1] = logs.malloc.log[begin+1].size;
        size[2] = logs.malloc.log[begin+2].size;
        size[3] = logs.malloc.log[begin+3].size;
        if(!containsArray(size,4,sizeof(university_t))){
            CU_FAIL();
            push_info_msg(_("You didn't malloc the right space for the university node. You should have malloced 20 bytes since you have 2 pointers (of 8 bytes each) + an int"));
        }
        if(!containsArray(size,4,sizeof(person_t))){
            CU_FAIL();
            push_info_msg(_("You didn't malloc the right space for the person node. You should have malloced 16 bytes since you have 1 pointer (of 8 bytes) and 2 ints (of 4 bytes each)"));
        }
        if(!containsArray(size,4,strlen(city)+1) || !containsArray(size,4,strlen(name)+1)){
            CU_FAIL();
            push_info_msg(_("You didn't malloc the right space for at least one string. Maybe you forgot to allocate space for the \\\\0 character"));
        }
        if(containsArray(size,4,8)){
            CU_FAIL();
            push_info_msg(_("It seems like you allocated at least one pointer"))
        }
    }

    if(!ret){
        CU_FAIL();
        push_info_msg(_("You returned a null value"));
        return;
    }

    if(!(ret->city)){
        CU_FAIL();
        push_info_msg(_("You returned a NULL city"));
        return;
    }

    if(!(ret->rector)){
        CU_FAIL();
        push_info_msg(_("You returned a NULL rector"));
        return;
    }
    if(!(ret->rector)->name){
        CU_FAIL();
        push_info_msg(_("You returned a NULL rector name"));
        return;
    }

    int uniCmp = uniEquals(ret,ucl);
    CU_ASSERT_EQUAL(uniCmp,true);
    if(!uniCmp)
        push_info_msg(_("Your structure isn't the expected one"));
    else{
        realloc(city,8);
        realloc(name,8);
        if(!city || !name){
            if(city) 
                free(city);
            if(name) 
                free(name);
            
            free_a(ucl);
            
            CU_FAIL(_("Internal error while allocating memory"));
        }
        
        strcpy(city,"changed");
        strcpy(name,"changed");
        
        int cmpChange = uniEquals(ret,ucl);
        
        if(cmpChange){
            push_info_msg(_("You didn't copy the strings ! You should use strcpy or copy it yourself"));
            CU_FAIL();
        }
    }
    
    free_a(ucl);
}

void test_init_first_malloc_fails(){
    set_test_metadata("init_all", _("Testing the behaviour of the function when first malloc fails"),1);

    university_t* ucl = init_new();
    char* city = malloc(sizeof(char)*17);
    char* name = malloc(sizeof(char)*16);

    if(!ucl || !city || !name){
        if(ucl) free_a(ucl);
        if(city) free(city);
        if(name) free(name);
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(city,"Louvain-la-Neuve");
    strcpy(name,"Vincent Blondel");

    failures.malloc = FAIL_FIRST;
    int begin = logs.malloc.n;
    monitored.free = true;
    monitored.malloc = true;
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    university_t* ret = init_all(city, creation, rectname, age, salary);
    SANDBOX_END;
    
    monitored.malloc = false;
    monitored.free = false;


    CU_ASSERT_EQUAL(stats.free.called, 0);
    if(stats.free.called)
        push_info_msg(_("Why did you use free ?"));

    CU_ASSERT_EQUAL(stats.malloc.called, 1);
    if(stats.malloc.called != 1){
        push_info_msg(_("You should call malloc once !"));
        return;
    }

    if(ret){
        CU_FAIL();
        push_info_msg(_("You should return NULL in this case"));
        return;
    }
    
    int notFreed = start - stats.memory.used;
    if(notFreed){
        CU_FAIL();
        push_info_msg(_("You didn't free all the memory you allocated"));
    }

}

void test_init_second_malloc_fails(){
    set_test_metadata("init_all", _("Testing the behaviour of the function when second malloc fails"),1);

    university_t* ucl = init_new();
    char* city = malloc(sizeof(char)*17);
    char* name = malloc(sizeof(char)*16);

    if(!ucl || !city || !name){
        if(ucl) 
            free_a(ucl);
        if(city) 
            free(city);
        if(name) 
            free(name);
        
        CU_FAIL(_("Internal error while allocating memory")); 
        return;
    }

    strcpy(city,"Louvain-la-Neuve");
    strcpy(name,"Vincent Blondel");
    
    failures.malloc = FAIL_SECOND;
    
    monitored.free = true;
    monitored.malloc = true;
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    university_t* ret = init_all(city, creation, rectname, age, salary);
    SANDBOX_END;

    monitored.malloc = false;
    monitored.free = false;


    CU_ASSERT_EQUAL(stats.free.called, 1);
    if(stats.free.called)
        push_info_msg(_("You should call free once in this case"));

    CU_ASSERT_EQUAL(stats.malloc.called, 2);
    if(stats.malloc.called != 2){
        push_info_msg(_("You should call malloc twice in this case"));
        return;
    }
    
    int notFreed = start - stats.memory.used;
    if(notFreed){
        CU_FAIL();
        push_info_msg(_("You didn't free all the memory you allocated"));
    }
    
    if(ret){
        CU_FAIL();
        push_info_msg(_("You should return NULL in this case"));
        return;
    }

}

void test_init_third_malloc_fails(){
    set_test_metadata("init_all", _("Testing the behaviour of the function when third malloc fails"),1);

    university_t* ucl = init_new();
    char* city = malloc(sizeof(char)*17);
    char* name = malloc(sizeof(char)*16);

    if(!ucl || !city || !name){
        if(ucl) 
            free_a(ucl);
        if(city) 
            free(city);
        if(name) 
            free(name);
        
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(city,"Louvain-la-Neuve");
    strcpy(name,"Vincent Blondel");
    
    failures.malloc = FAIL_THIRD;
    
    monitored.free = true;
    monitored.malloc = true;
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    university_t* ret = init_all(city, creation, rectname, age, salary);
    SANDBOX_END;
    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.free.called, 2);
    if(stats.free.called)
        push_info_msg(_("You should call free twice in this case"));

    CU_ASSERT_EQUAL(stats.malloc.called, 3);
    if(stats.malloc.called != 3){
        push_info_msg(_("You should call malloc 3 times in this case"));
        return;
    }
    
    int notFreed = start - stats.memory.used;
    if(notFreed){
        CU_FAIL();
        push_info_msg(_("You didn't free all the memory you allocated"));
    }
    
    if(ret){
        CU_FAIL();
        push_info_msg(_("You should return NULL in this case"));
        return;
    }

}

void test_init_fourth_malloc_fails(){
    set_test_metadata("init_all", _("Testing the behaviour of the function when third malloc fails"),1);

    university_t* ucl = init_new();
    char* city = malloc(sizeof(char)*17);
    char* name = malloc(sizeof(char)*16);

    if(!ucl || !city || !name){
        if(ucl) 
            free_a(ucl);
        if(city) 
            free(city);
        if(name) 
            free(name);
        
        CU_FAIL(_("Internal error while allocating memory"));
        return;
    }

    strcpy(city,"Louvain-la-Neuve");
    strcpy(name,"Vincent Blondel");
    
    failures.malloc = 1 << 4; // should make the fourth malloc fail.
    
    monitored.free = true;
    monitored.malloc = true;
    int start = stats.memory.used;
    
    SANDBOX_BEGIN;
    university_t* ret = init_all(city, creation, rectname, age, salary);
    SANDBOX_END;
    monitored.malloc = false;
    monitored.free = false;

    CU_ASSERT_EQUAL(stats.free.called, 3);
    if(stats.free.called)
        push_info_msg(_("You should call free twice in this case"));

    CU_ASSERT_EQUAL(stats.malloc.called, 4);
    if(stats.malloc.called != 4){
        push_info_msg(_("You should call malloc 4 times in this case"));
        return;
    }
    
    int notFreed = start - stats.memory.used;
    if(notFreed){
        CU_FAIL();
        push_info_msg(_("You didn't free all the memory you allocated"));
    }
    
    if(ret){
        CU_FAIL();
        push_info_msg(_("You should return NULL in this case"));
        return;
    }

}

/*
 * Fonction permettant de creer le fichier dot pour afficher
 * les zones memoires
 * En commentaire en attendant l'installation de graphviz
 * Louis Navarre
int compute_graphic(university_t* u){
    Agraph_t *g;
    Agnode_t *n;
    GVC_t *gvc;

    // set up a graphviz context
    gvc = gvContext();

    // Create a simple digraph 

    g = agopen("g", Agdirected, 0);

    //struct node *ptr=stack;
    n = agnode(g, "variable u", 1);
    agsafeset(n, "color", "white", "");


    char ptr_addr[20];

    snprintf(ptr_addr, 20, "%p", u);
    Agnode_t *u_ptr = agnode(g, ptr_addr, 1);
    Agnode_t *u_v = agnode(g, "u", 1);
    agsafeset(u_v, "shape", "rectangle", "");


    snprintf(ptr_addr, 20, "%p", u->city);
    Agnode_t *city = agnode(g, u->city, 1);
    agsafeset(city, "shape", "rectangle", "");
    Agnode_t *city_ptr = agnode(g, ptr_addr, 1);

    snprintf(ptr_addr, 20, "%p", u->rector);
    Agnode_t *rector = agnode(g, "rector", 1);
    agsafeset(rector, "shape", "rectangle", "");
    Agnode_t *rector_ptr = agnode(g, ptr_addr, 1);

    snprintf(ptr_addr, 20, "%p", u->rector->name);
    Agnode_t *name = agnode(g, u->rector->name, 1);
    agsafeset(name, "shape", "rectangle", "");
    Agnode_t *name_ptr = agnode(g, ptr_addr, 1);

    Agedge_t *e1 = agedge(g, n, u_ptr, 0, 1);
    e1 = agedge(g, u_ptr, u_v, 0, 1);
    e1 = agedge(g, u_v, city_ptr, 0, 1);
    e1 = agedge(g, city_ptr, city, 0 ,1);
    e1 = agedge(g, u_v, rector_ptr, 0, 1);
    e1 = agedge(g, rector_ptr, rector, 0, 1);
    e1 = agedge(g, rector, name_ptr, 0 ,1);
    e1 = agedge(g, name_ptr, name, 0, 1);
    e1 = NULL;

    // compute graph
    gvLayout(gvc, g, "dot");
    // save graph in png file for feedback
    gvRenderFilename(gvc, g, "png", "t.png");


    gvFreeLayout(gvc, g);
    agclose(g);
    return (gvFreeContext(gvc));
}
*/

int main(int argc,char* argv[])
{
    BAN_FUNCS(calloc);
    RUN(test_free_success, test_free_rector_null, test_free_strings_null, test_init_normal_case, test_init_first_malloc_fails, test_init_second_malloc_fails, test_init_third_malloc_fails, test_init_fourth_malloc_fails);
}
