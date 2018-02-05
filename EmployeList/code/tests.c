/* Tests unitaires pour l'exercice EmployeList
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
#include<string.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>

#include "student_code.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


#include <fcntl.h>



int global_count_free=0; // counter pour appel free
int global_count_malloc=0; // counter malloc

void * last_free; // dernier appel à free
void * last_malloc; // dernier appel à malloc

void *__real_malloc(size_t size);
void __real_free(void *ptr);

/* wrapping  functions */

void* __wrap_malloc (size_t size) {
  global_count_malloc++;
  void *ptr=__real_malloc(size);
  last_malloc=ptr;
  return ptr;
}

void __wrap_free(void *ptr) {
  global_count_free++;
  last_free=ptr;
  return __real_free(ptr);
}


/* les fonctions de manipulation de la liste */
void _displayEmploye(Employe *e) {
  printf("%s\t%d\n", e->name, e->age);
}
/*
 * @pre list!=NULL
 * @post initialise la liste chaînée
 */
void _initializeList(LinkedList *list) {
  list->head=NULL;
  list->tail=NULL;
}

/*
 * @pre list!=NULL, data!=NULL, data pointe vers Employe
 * @post Crée un noeud contenant un pointeur vers data
 *       et l'ajoute en début de liste
 *       retourne un pointeur vers le noeud créé, NULL en cas d'erreur
 */
Node * _addHead(LinkedList *list, void *data) {
  Node *node= (Node *) malloc(sizeof(Node));
  if(node==NULL)
    return NULL;
  node->data=data;
  if(list->head==NULL) {
    list->tail=node;
    node->next=NULL;
  } else {
    node->next=list->head;
  }
  list->head=node;
  return node;
}

// non fourni
void _printList(LinkedList *list) { 
  Node *node=list->head;
  while(node!=NULL) {
    Employe *e= (Employe *)node->data;
    printf("%s [%d]\n",e->name, e->age);
    node=node->next;
  }

}
/* 
 * @pre list!=NULL, node!=NULL
 * @post retourne le nombre d'occurences du noeud node
 */
int _count(LinkedList *list, Node *n) {
  int count=0;
  Node *node=list->head;
  while(node!=NULL) {
    if(node==n)
      count++;
    node=node->next;
  }
  return count;

}
/*
 * @pre list!=NULL
 * @post retourne 0 si elle est correcte, -1 en cas d'erreur
 */
int _verifyList(LinkedList *list) {
  if(list->head==NULL && list->tail==NULL) {
    return 0;
  }
  Node *node=list->head;
  while(node!=NULL) {
    //Employe *e= (Employe *)node->data;
    if(node->next==NULL && node==list->tail) {
      return 0;
    }
    node=node->next;
  }
  return -1;



}
// non fourni
void _delete(LinkedList *list, Node * node) {
  if(node==list->head) {
    if(list->head->next==NULL) {
      list->head=list->tail=NULL;
    } else {
      list->head=list->head->next;
    }
  }
  else {
    Node *tmp=list->head;
    while(tmp!=NULL && tmp->next!=node) {
      tmp=tmp->next;
    }
    if(tmp!=NULL) {
      tmp->next=node->next;
      if(tmp->next==NULL) { // dernier
        list->tail=tmp;
      }
    }
  }
  free(node);
}
/*
 * @pre list!=NULL, age>0
 * @post retire de la liste tous les employés strictement plus jeunes
 *       que age. Retourne le nombre d'employés retirés de la
 *       liste, -1 en cas d'erreur.
 */
int _deleteAllYounger(LinkedList *list, int age) {

  int count=0;
  Node *node;
  node=list->head;
  if (node==NULL) {
    return 0;
  }
  while(node!=NULL) {
    Employe *e= (Employe *)node->data;
    if(e->age<age) {
      //printf("Removing %s [%d]\n",e->name, e->age);
      _delete(list,node);
      count++;
    }
    node=node->next;
  }
  return count;
}


/* La fonction d'initialisation de la suite de tests.
 * Retourne 0 en cas de succ<C3><A8>s, n'importe quelle autre valeur indique un <C3>
<A9>chec.
*/
int init_suite1(void) {
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  return 0;
}


// @deleteAllYounger:test_el1 => [liste de quatre éléments, rien à retirer, retourne 0 et ne fait aucun free]
void test_el1()
{
  int ret;

  LinkedList l;
  Employe e={ .name="Simon",.age=17};
  Employe e1={ .name="Laurine",.age=16};
  Employe e2={ .name="Jules",.age=30};
  Employe e3={ .name="Olivier",.age=47};
  _initializeList(&l);
  //_printList(&l);
  Node* n1=_addHead(&l,&e);
  Node* n2=_addHead(&l,&e1);
  Node* n3=_addHead(&l,&e2);
  Node* n4=_addHead(&l,&e3);
  global_count_malloc=0; 
  global_count_free=0;
  ret=deleteAllYounger(&l,1);
  CU_ASSERT_EQUAL(ret,0);
  CU_ASSERT_EQUAL( global_count_free, 0);
  CU_ASSERT_EQUAL( global_count_malloc, 0);
  CU_ASSERT_EQUAL(_verifyList(&l),0);
  CU_ASSERT_EQUAL(_count(&l,n1),1);
  CU_ASSERT_EQUAL(_count(&l,n2),1);
  CU_ASSERT_EQUAL(_count(&l,n3),1);
  CU_ASSERT_EQUAL(_count(&l,n4),1);
  _delete(&l,n1);
  _delete(&l,n2);
  _delete(&l,n3);
  _delete(&l,n4);


}
// @deleteAllYounger:test_el2 => [Liste de quatre elements, en retire 2]
void test_el2()
{

  int ret;

  LinkedList l;
  Employe e={ .name="Simon",.age=17};
  Employe e1={ .name="Laurine",.age=16};
  Employe e2={ .name="Jules",.age=30};
  Employe e3={ .name="Olivier",.age=47};
  _initializeList(&l);
  Node* n1=_addHead(&l,&e);
  Node* n2=_addHead(&l,&e1);
  Node* n3=_addHead(&l,&e2);
  Node* n4=_addHead(&l,&e3);
  global_count_malloc=0; 
  global_count_free=0;
  ret=deleteAllYounger(&l,18);
  CU_ASSERT_EQUAL(ret,2);
  CU_ASSERT_EQUAL( global_count_free, 2);
  CU_ASSERT_EQUAL( global_count_malloc, 0);
  CU_ASSERT_EQUAL(_verifyList(&l),0);
  CU_ASSERT_EQUAL(_count(&l,n1),0);
  CU_ASSERT_EQUAL(_count(&l,n2),0);
  CU_ASSERT_EQUAL(_count(&l,n3),1);
  CU_ASSERT_EQUAL(_count(&l,n4),1);
  _delete(&l,n3);
  _delete(&l,n4);

}

// @deleteAllYounger:test_el3 => [liste avec retrait uniquement du premier]
void test_el3()
{
   int ret;

  LinkedList l;
  Employe e={ .name="Simon",.age=17};
  Employe e1={ .name="Laurine",.age=16};
  Employe e2={ .name="Jules",.age=30};
  Employe e3={ .name="Olivier",.age=12};
  _initializeList(&l);
  Node* n1=_addHead(&l,&e);
  Node* n2=_addHead(&l,&e1);
  Node* n3=_addHead(&l,&e2);
  Node* n4=_addHead(&l,&e3);
  global_count_malloc=0; 
  global_count_free=0;
  ret=deleteAllYounger(&l,13);
  CU_ASSERT_EQUAL(ret,1);
  CU_ASSERT_EQUAL( global_count_free, 1);
  CU_ASSERT_EQUAL( global_count_malloc, 0);
  CU_ASSERT_PTR_EQUAL(last_free,n4);
  CU_ASSERT_EQUAL(_verifyList(&l),0);
  CU_ASSERT_EQUAL(_count(&l,n1),1);
  CU_ASSERT_EQUAL(_count(&l,n2),1);
  CU_ASSERT_EQUAL(_count(&l,n3),1);
  CU_ASSERT_EQUAL(_count(&l,n4),0);
  _delete(&l,n3);
  _delete(&l,n1);
  _delete(&l,n2);

}

// @deleteAllYounger:test_el4 => [liste avec retrait uniquement du dernier element]
void test_el4()
{
   int ret;

  LinkedList l;
  Employe e={ .name="Simon",.age=7};
  Employe e1={ .name="Laurine",.age=16};
  Employe e2={ .name="Jules",.age=30};
  Employe e3={ .name="Olivier",.age=12};
  _initializeList(&l);
  Node* n1=_addHead(&l,&e);
  Node* n2=_addHead(&l,&e1);
  Node* n3=_addHead(&l,&e2);
  Node* n4=_addHead(&l,&e3);
  global_count_malloc=0; 
  global_count_free=0;
  ret=deleteAllYounger(&l,8);
  CU_ASSERT_EQUAL(ret,1);
  CU_ASSERT_EQUAL( global_count_free, 1);
  CU_ASSERT_EQUAL( global_count_malloc, 0);
  CU_ASSERT_PTR_EQUAL(last_free,n1);
  CU_ASSERT_EQUAL(_verifyList(&l),0);
  CU_ASSERT_EQUAL(_count(&l,n1),0);
  CU_ASSERT_EQUAL(_count(&l,n2),1);
  CU_ASSERT_EQUAL(_count(&l,n3),1);
  CU_ASSERT_EQUAL(_count(&l,n4),1);
  _delete(&l,n3);
  _delete(&l,n2);
  _delete(&l,n4);

}

// @deleteAllYounger:test_el5 => [retrait de tous les éléments]
void test_el5()
{
   int ret;

  LinkedList l;
  Employe e={ .name="Simon",.age=7};
  Employe e1={ .name="Laurine",.age=16};
  Employe e2={ .name="Jules",.age=30};
  Employe e3={ .name="Olivier",.age=12};
  _initializeList(&l);
  Node* n1=_addHead(&l,&e);
  Node* n2=_addHead(&l,&e1);
  Node* n3=_addHead(&l,&e2);
  Node* n4=_addHead(&l,&e3);
  global_count_malloc=0; 
  global_count_free=0;
  ret=deleteAllYounger(&l,99);
  CU_ASSERT_EQUAL(ret,4);
  CU_ASSERT_EQUAL( global_count_free, 4);
  CU_ASSERT_EQUAL( global_count_malloc, 0);
  CU_ASSERT_EQUAL(_verifyList(&l),0);
  CU_ASSERT_EQUAL(_count(&l,n1),0);
  CU_ASSERT_EQUAL(_count(&l,n2),0);
  CU_ASSERT_EQUAL(_count(&l,n3),0);
  CU_ASSERT_EQUAL(_count(&l,n4),0);

}


int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  //montest();
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}


    CU_pSuite suiteM = CU_add_suite("Tests de EmployeList",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_el1", test_el1) ||
       NULL==CU_add_test(suiteM, "test_el2", test_el2) || 
       NULL==CU_add_test(suiteM, "test_el3", test_el3) || 
       NULL==CU_add_test(suiteM, "test_el4", test_el4) || 
       NULL==CU_add_test(suiteM, "test_el5", test_el5) ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}
