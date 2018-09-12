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
    
    university_t* u_c = u;
    char* city_c = u->city;
    person_t* p_c = u->rector;
    char* name_c = u->rector->name;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(stats.free.called, 4);
    if(stats.free.called != 4){
        push_info_msg(_("You did not free all the memory"));
    }
    
    int verif_u = malloced(u_c);
    int verif_city = malloced(city_c);
    int verif_rector = malloced(p_c);
    int verif_name = malloced(name_c);
    
    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_city, false);
    CU_ASSERT_EQUAL(verif_rector, false);
    CU_ASSERT_EQUAL(verif_name, false);
    
    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
    }
    if(verif_city != false){
        push_info_msg(_("You did not free the memory associated with the city"));
    }
    if(verif_rector != false){
        push_info_msg(_("You did not free the memory associated with the rector"));
    }
    if(verif_name != false){
        push_info_msg(_("You did not free the memory associated with the rector's name"));
    }
    
    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
    push_info_msg(_("The last free you should do is on the struct"));
}

void test_rector_null(){
    set_test_metadata("free_all", _("Testing when there is no rector"), 1);
    
    university_t* u = init_u(NULL, "Ottawa", 1848);
    if(u == NULL)
        return;
    
    university_t* u_c = u;
    char* city_c = u->city;
    
    monitored.free = true;
    
    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(stats.free.called, 2);
    if(stats.free.called != 2){
        push_info_msg(_("You did not free all the memory"));
    }
    
    int verif_u = malloced(u_c);
    int verif_city = malloced(city_c);
    
    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_city, false);
    
    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
    }
    if(verif_city != false){
        push_info_msg(_("You did not free the memory associated with the city"));
    }
    
    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
    push_info_msg(_("The last free you should do is on the struct"));
}

void test_strings_null(){
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
    
    SANDBOX_BEGIN;
    free_all(u);
    SANDBOX_END;
    
    monitored.free = false;
    
    CU_ASSERT_EQUAL(stats.free.called,2);
    if(stats.free.called > 2){
        push_info_msg(_("Your code does not work correctly when the strings are NULL"));
    }
    if(stats.free.called < 2){
        push_info_msg(_("You did not free all the memory"));
    }
    
    int verif_u = malloced(u_c);
    int verif_rector = malloced(p_c);
    
    CU_ASSERT_EQUAL(verif_u, false);
    CU_ASSERT_EQUAL(verif_rector, false);
    
    if(verif_u != false){
        push_info_msg(_("You did not free the memory associated with the university"));
    }
    if(verif_rector != false){
        push_info_msg(_("You did not free the memory associated with the rector"));
    }
    
    void *last_ptr = (void*) stats.free.last_params.ptr;
    void *ptr = (void*) u_c;
    CU_ASSERT_EQUAL(ptr, last_ptr);
    if (ptr != last_ptr)
    push_info_msg(_("The last free you should do is on the struct"));
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
    BAN_FUNCS(malloc, calloc);
    RUN(test_success, test_rector_null, test_strings_null);
}
