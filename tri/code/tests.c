/*   Copyright (c) 2015, 2016 Hautecoeur Cécile & Massart Aurélie
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 

/* Tests unitaires pour de petites questions sur les listes chainees (tri d'une liste)*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>

#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>

#include "student_code.h"
#include "memory.h"


//@q3:test_sortList_1 => [Tri d'une liste nulle ou vide]
void test_sortList_1(){
	initTest();
	sortList(NULL);
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	myList->head = NULL;
	sortList(myList);
	CU_ASSERT(myList!=NULL);
	NEW_CU_ASSERT(myList->head==NULL,"Trier une liste vide laisse la liste vide");
	freememory();
}

//@q3:test_sortList_2 => [Tri d'une liste a un element]
void test_sortList_2(){
    initTest();
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	struct node* node1 = (struct node*)malloc(sizeof(struct node));
	if(node1==NULL){free(myList); return;}
	node1->elem = 1;
	node1->next = NULL;
	myList->head = node1;

	sortList(myList);
	NEW_CU_ASSERT(global_malloc-2==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!", global_size_malloc-(int)sizeof(struct list) - (int)sizeof(struct node)-global_size_free);
	NEW_CU_ASSERT(myList->head->elem==1 && myList->head->next==NULL,"trier une liste a un element ne modifie pas la liste!");
	freememory();
}

//@q3:test_sortList_3 => [Tri d'une liste deja triee]
void test_sortList_3(){
    initTest();
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	struct node* node1 = (struct node*)malloc(sizeof(struct node));
	if(node1==NULL){free(myList); return;} 
	myList->head = node1; node1->elem = 1;
	int i = 1;
	while (i<100) {
		i++;
		struct node *node2 = (struct node *)malloc(sizeof(struct node));
		node2->elem = i;
		node1->next = node2;
	    	node1 = node2;
	}
	node1->next = NULL;
	sortList(myList);
	NEW_CU_ASSERT(global_malloc-101==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!", global_size_malloc-(int)sizeof(struct list) - 100*(int)sizeof(struct node)-global_size_free);
	CU_ASSERT(myList!=NULL);
	i=1; node1=myList->head;
	while(i<=100){
		NEW_CU_ASSERT(node1!=NULL,"La liste n'a plus assez d'elements");
		NEW_CU_ASSERT(node1->elem == i,"La liste n'est plus ordonnee");
		node1=node1->next; i++;
	}
	NEW_CU_ASSERT(node1==NULL,"La liste contient trop d'elements");	

	freememory();
}
	
//@q3:test_sortList_4 => [Tri d'une liste avec de nombreuses fois le meme element]
void test_sortList_4(){
	initTest();
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	struct node* node1 = (struct node*)malloc(sizeof(struct node));
	if(node1==NULL){free(myList); return;}
	int i = 1; 
    myList->head = node1; node1->elem = -1;
	while (i<100) {
		i++;
		struct node *node2 = (struct node *)malloc(sizeof(struct node));
		node2->elem = -1;
		node1->next = node2;
	    	node1 = node2;
	}
	node1->next = NULL;

	sortList(myList);
	NEW_CU_ASSERT(global_malloc-101==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!", global_size_malloc-(int)sizeof(struct list) - 100*(int)sizeof(struct node)-global_size_free);
	CU_ASSERT(myList!=NULL);
	i=1; node1=myList->head;
	while(i<=100){
		NEW_CU_ASSERT(node1!=NULL,"La liste n'a plus assez d'elements");
		NEW_CU_ASSERT(node1->elem == -1,"La liste n'est plus ordonnee");
		node1=node1->next; i++;
	}
	NEW_CU_ASSERT(node1==NULL,"La liste contient trop d'elements");

	freememory();
}

//@q3:test_sortList_5 => [Tri d'une liste triee en sens inverse]
void test_sortList_5(){
	initTest();
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	struct node* node1 = (struct node*)malloc(sizeof(struct node));
	if(node1==NULL){free(myList); return;}
	int i = 1; myList->head = node1;
	node1->elem = 100;
	while (i<100) {
		struct node *node2 = (struct node *)malloc(sizeof(struct node));
		node2->elem = 100-i;
		node1->next = node2;
	    	node1 = node2; i++;
	}
	node1->next= NULL;

	sortList(myList);

	NEW_CU_ASSERT(global_malloc-101==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!",  global_size_malloc-(int)sizeof(struct list) - 100*(int)sizeof(struct node)-global_size_free);
	CU_ASSERT(myList!=NULL);
	i=1; node1=myList->head;
	while(i<=100){
		NEW_CU_ASSERT(node1!=NULL,"La liste n'a plus assez d'elements");
		NEW_CU_ASSERT(node1->elem == i,"La liste n'est pas ordonnee \n");
		node1=node1->next; i++;
	}
	NEW_CU_ASSERT(node1==NULL,"La liste contient trop d'elements");

	freememory();
}

//@q3:test_sortList_6 => [Tri d'une petite liste quelconque]
void test_sortList_6(){
	initTest();
	struct list * myList2 = (struct list *)malloc(sizeof(struct list));
	struct node * node12 = (struct node *)malloc(sizeof(struct node));
	struct node * node2 = (struct node *)malloc(sizeof(struct node));
	struct node * node3 = (struct node *)malloc(sizeof(struct node));
	struct node * node4 = (struct node *)malloc(sizeof(struct node));
	node12->elem = 5;
	node12->next=node2; node2->elem = 3;
	node2->next=node3; node3->elem = 7;
	node3->next=node4; node4->elem = 3;
	node4->next = NULL; myList2->head = node12;

	sortList(myList2);

	NEW_CU_ASSERT(global_malloc-5==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!", global_size_malloc-(int)sizeof(struct list) - 4*(int)sizeof(struct node)-global_size_free);
	CU_ASSERT(myList2!=NULL);

	struct node *current = myList2->head; CU_ASSERT(current!=NULL);
	NEW_CU_ASSERT(current->elem == 3,"Erreur lors du tri de la liste [5 3 7 3] le premier element est %d plutot que 3",current->elem); current=current->next; CU_ASSERT(current!=NULL);
	NEW_CU_ASSERT(current->elem == 3,"Erreur lors du tri de la liste [5 3 7 3] le second element est %d plutot que 3",current->elem); current=current->next; CU_ASSERT(current!=NULL);
	NEW_CU_ASSERT(current->elem == 5,"Erreur lors du tri de la liste [5 3 7 3] le 3e element est %d plutot que 5",current->elem); current=current->next; CU_ASSERT(current!=NULL);
	NEW_CU_ASSERT(current->elem == 7,"Erreur lors du tri de la liste [5 3 7 3] le dernier element est %d plutot que 7",current->elem); current=current->next; 
	NEW_CU_ASSERT(current==NULL,"La liste contient trop d'éléments");
	
	freememory();
}

//calcule la somme des elements de myList
int somme(struct list* myList){
	int s = 0;
	if (myList==NULL) return 0;
	struct node* current = myList->head;
	while(current!=NULL){
		s=s+current->elem;
		current=current->next;
	}
	return s;
}

//@q3:test_sortList_7 => [Tri d'une grande liste quelconque]
void test_sortList_7(){
	initTest();
	struct list *myList = (struct list *)malloc(sizeof(struct list));
	if (myList==NULL) return;
	struct node* node1 = (struct node*)malloc(sizeof(struct node));
	if(node1==NULL){free(myList); return;}
	int i = 1; 
	myList->head=node1; node1->elem = rand();
	while (i<100) {
		struct node *node2 = (struct node *)malloc(sizeof(struct node));
		node2->elem = rand();
		node1->next = node2;
	   	node1 = node2; i++;
	}
	node1->next = NULL;
	int sum = somme(myList);

	global_malloc = 0;
	global_free=0;
	sortList(myList);
	if(global_malloc!=0 || global_free!=0) 
		printf("Il est possible de faire cette question sans appel a malloc ni free \n");

	NEW_CU_ASSERT(global_malloc==global_free,"Attention aux pertes de memoire! tu as perdu %d bytes!", global_size_malloc-(int)sizeof(struct list) - 100*(int)sizeof(struct node)-global_size_free);
	CU_ASSERT(myList!=NULL);
	NEW_CU_ASSERT(somme(myList)==sum,"la somme des elements de la liste a ete modifiee");
	i=1; node1=myList->head;
	int prev = node1->elem;
	while(i<100){
		node1=node1->next; i++;
		NEW_CU_ASSERT(node1!=NULL,"La liste n'a plus assez d'elements");
		NEW_CU_ASSERT(node1->elem >= prev,"La liste n'est pas ordonnee");
	}
	NEW_CU_ASSERT(node1->next==NULL,"La liste contient trop d'elements");
	
	freememory();
}

/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {


  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  //  array_list_destroy(head);
  return 0;
}


int main ()
{
  CU_pSuite pSuite = NULL;

  /* On initialise la suite de tests */
  if(CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* On ajoute la suite au registre */
  pSuite = CU_add_suite("Suite de tests pour la question ArrayList", init_suite1, clean_suite1);
  if(NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* On ajoute les tests à la suite. L'ordre est important !
   * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
  if(NULL == CU_add_test(pSuite, "test_sortList_1", test_sortList_1) ||
     NULL == CU_add_test(pSuite, "test_sortList_2", test_sortList_2) ||
     NULL == CU_add_test(pSuite, "test_sortList_3", test_sortList_3) ||
     NULL == CU_add_test(pSuite, "test_sortList_4", test_sortList_4) ||
     NULL == CU_add_test(pSuite, "test_sortList_5", test_sortList_5) ||
     NULL == CU_add_test(pSuite, "test_sortList_6", test_sortList_6) ||
     NULL == CU_add_test(pSuite, "test_sortList_7", test_sortList_7))  
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
  
  
  /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit 
   */
  CU_automated_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
