/* Tests unitaires pour l'exercice dl.c
Copyright (C) 2016 Olivier Bonaventure

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include<stdio.h>
#include<stdlib.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>

#include "student_code.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


int global_malloc=0; 
int global_free=0; // counter pour appel free
int fail_malloc=0;
void * last_malloc; // dernier appel à malloc
void * last_free; // dernier appel à free

void*  __real_malloc(size_t size);
void __real_free(void *ptr);

/* wrapping  functions */

void* __wrap_malloc (size_t size) {
  global_malloc++;
  if(fail_malloc)
    return NULL;
  void * ptr=__real_malloc(size);
  last_malloc=ptr;
  return ptr;
}

void __wrap_free(void *ptr) {
  global_free++;
  last_free=ptr;
  return __real_free(ptr);
}


/* les fonctions de manipulation de la liste */

/*
 * @pre l!=NULL
 * @post a initialisé la liste doublement chaînée l. Une liste vide
 *       contient toujours un premier noeud qui est vide
 *       et dont les pointeurs next et prev pointent vers NULL
 *       retourne 0 en cas de succès, -1 en cas d'erreur
 */

int _list_init(list *l) {

  l->start=malloc(sizeof(node));
  if(l->start==NULL) 
    return -1;
  l->start->next=NULL;
  l->start->prev=NULL;
  l->start->data=-1;
  l->tail=l->start;
  return 0;
}

/*
 * @pre l!=NULL
 * @post insert à la fin de la liste un noeud content la donnée data
 *       retourne -1 en cas d'erreur, 0 sinon
 */
int _insert(list *l, int data)
{
  node *pointer;
  pointer = (node *)malloc(sizeof(node));
  if(pointer==NULL)
    return -1;
  l->tail->next=pointer;
  pointer->prev=l->tail;
  l->tail=pointer;
  pointer->data = data;
  pointer->next = NULL;
  return 0;
}

// non founi
int _count(list *list, int key)
{
  int count=0;
  node *pointer;
  pointer=list->start;
  pointer =  pointer -> next; //First node is dummy node.
  /* Iterate through the entire linked list and search for the key. */
  while(pointer!=NULL)
    {
      if(pointer->data == key) //key is found.
        {
          count++;
        }
      pointer = pointer -> next;//Search in the next node.
    }
  /*Key is not found */
  return count;
}

// verifie la structure de la liste, 0 si correct, -1 en cas d'erreur
int _verify(list *l) {
  // liste vide
  if(l->start->next==NULL && l->start->prev==NULL) {
    return 0;
  }
  // liste non-vide
  // parcours en avant
  node * pointer, * previous;
  previous=l->start;
  pointer=l->start->next;
  while(pointer!=NULL) {
    if(pointer->prev != previous) {
      // error
    printf("error prev\n");
    return -1;
    }
    previous= pointer;
    pointer = pointer -> next;
  }
  // verifie que c'est tail
  if(previous!=l->tail) {
    // error
    printf("error tail\n");
    return -1;
  }
  return 0;
}

/* La fonction d'initialisation de la suite de tests.
 * Retourne 0 en cas de succ<C3><A8>s, n'importe quelle autre valeur indique un <C3>
<A9>chec.
*/


int _delete(list *l, int key)
{
  int found=0; // nombre d'occurences trouvées

  node * pointer=l->start;
  while(pointer->next!=NULL) {
    /* Go to the node for which the node next to it has to be deleted */
    while(pointer->next!=NULL && (pointer->next)->data != key) {
      pointer = pointer -> next;
    }
    if(pointer->next==NULL) {
      // printf("Element %d is not present in the list\n",key);
      return found;
    }
    /* Now pointer points to a node and the node next to it has to be removed */
    found++;
    node *temp;
    temp = pointer -> next;
    // mise à jour tail
    if(temp==l->tail) {
      pointer->next=NULL;
      l->tail=pointer;
    } else {
      // temp doit être retiré
      pointer->next = temp->next;
      temp->prev =  pointer;
      pointer->next->prev=pointer;
      free(temp);
    }
  }
  return found;
}

/*int delete(list *l, int key) {
  return _delete(l,key);
}*/


int init_suite1(void) {
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  return 0;
}


// @dl:test_dl1 => [liste vide, aucun appel à malloc ou free, retourne 0]
void test_dl1()
{
  list l;
  int err __attribute__ ((unused));
  err=_list_init(&l);
  int ret;
  global_malloc=0;
  global_free=0;
  ret=delete(&l, 1234);
  CU_ASSERT_EQUAL(ret,0);
  CU_ASSERT_EQUAL( global_free,  0);
  CU_ASSERT_EQUAL( global_malloc,  0);

}
// @dl:test_dl2 => [liste avec 1 element au milieu a retirer]
void test_dl2()
{
  list l;
  int err __attribute__ ((unused));
  err=_list_init(&l);
  err=_insert(&l,123);
  err=_insert(&l,456);
  void * ptr=last_malloc;
  err=_insert(&l,789);

  int ret;
  ret=delete(&l,456);
  void *freed_ptr=last_free;
  CU_ASSERT_EQUAL(ret,1);
  CU_ASSERT_PTR_EQUAL(ptr,freed_ptr);
  // structure correcte
  CU_ASSERT_EQUAL(_verify(&l),0);
  
}

// @dl:test_dl3 => [liste avec 2 elements au milieu a retirer]
void test_dl3()
{
  list l;
  int err __attribute__ ((unused));
  err=_list_init(&l);
  err=_insert(&l,123);
  err=_insert(&l,456);
  err=_insert(&l,456);
  err=_insert(&l,789);

  int ret;
  ret=delete(&l,456);
  CU_ASSERT_EQUAL(ret,2);
  CU_ASSERT_EQUAL(_verify(&l),0);
  
}

// @dl:test_dl4 => [liste avec 3 elements  a retirer]
void test_dl4()
{
  list l;
  int err __attribute__ ((unused));
  err=_list_init(&l);
  err=_insert(&l,123);
  err=_insert(&l,123);
  err=_insert(&l,123);
  err=_insert(&l,789);

  int ret;
  ret=delete(&l,123);
  CU_ASSERT_EQUAL(ret,3);
  CU_ASSERT_EQUAL(_verify(&l),0);
  
}

// @dl:test_dl5 => [liste avec dernier element  a retirer]
void test_dl5()
{
  list l;
  int err __attribute__ ((unused));
  err=_list_init(&l);
  err=_insert(&l,123);
  err=_insert(&l,123);
  err=_insert(&l,123);
  err=_insert(&l,789);

  int ret;
  ret=delete(&l,789);
  CU_ASSERT_EQUAL(ret,1);
  CU_ASSERT_EQUAL(_verify(&l),0);
  
}


int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  montest();

  // on commence par la fonction de l'etudiant
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}

    CU_pSuite suiteM = CU_add_suite("Tests de Double Linked List",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_dl1", test_dl1) ||
       NULL==CU_add_test(suiteM, "test_dl2", test_dl2) || 
       NULL==CU_add_test(suiteM, "test_dl3", test_dl3) || 
       NULL==CU_add_test(suiteM, "test_dl4", test_dl4) || 
       NULL==CU_add_test(suiteM, "test_dl5", test_dl5) ) {

        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
