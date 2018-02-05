/* Tests unitaires pour la question sur le tail-add des arraylist */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>
#include <semaphore.h>

#include "student_code.h"
#include "lecture.h"

/* wrappers pour les appels systeme, afin de verifier que les
 * etudiants les utilisent bien
 */

int global_sem_wait=0; // counter pour appel sem_wait
int global_sem_post=0; // counter pour appel sem_post

int global_malloc=0;
int global_realloc=0;

int fail_realloc=0; // don't fail

/* create pointers for real glibc functions */

int __real_sem_wait(sem_t *sem);
int __real_sem_post(sem_t *sem);
void*  __real_realloc(void *ptr, size_t size);

/* wrapping  functions */

int __wrap_sem_wait(sem_t *sem) {
  global_sem_wait++;
  return __real_sem_wait(sem);
}

int __wrap_sem_post(sem_t *sem) {
  global_sem_post++;
  return __real_sem_post(sem);
}

void* __wrap_realloc (void *ptr, size_t size) {
  global_realloc++;
  if(fail_realloc)
    return NULL;
  return __real_realloc(ptr,size);
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



// @ArrayList:test_sem => [Ajout d'un element => sem_wait et sem_post]
void test_sem(void) {

  int data=1252;
  struct array_list *head = arraylist_init((size_t) 1, (size_t) sizeof(int));
  set_element(head, 0, (void *)&data);
  global_sem_wait=0;
  global_sem_post=0;
  int data2=17;
  int ret=add_tail(head, (void *) &data2);
  CU_ASSERT_EQUAL(ret, 0);
  CU_ASSERT_EQUAL(global_sem_wait, global_sem_post);
  CU_ASSERT_TRUE(global_sem_wait >0);
  global_sem_wait=0;
  global_sem_post=0;
  array_list_destroy(head);
}


// @ArrayList:test_valeurs => [Ajout d'un element => valeurs correctes dans Alist]
void test_valeurs(void) {

  int data=1252;
  struct array_list *head = arraylist_init((size_t) 1, (size_t) sizeof(int));
  set_element(head, 0, (void *)&data);
  int data2=17;
  add_tail(head, (void *) &data2);
  int elem0;
  int elem1;
  get_element(head,0,(void*) &elem0);
  CU_ASSERT_EQUAL(elem0,1252);
  get_element(head,1,(void*) &elem1);
  CU_ASSERT_EQUAL(elem1,17);
  array_list_destroy(head);		 
}


// @ArrayList:test_realloc => [Ajout d'un element => autre pointeur]
void test_realloc(void) {

  int data=1252;
  struct array_list *head = arraylist_init((size_t) 1, (size_t) sizeof(int));
  set_element(head, 0, (void *)&data);
  global_realloc=0;
  int data2=17;
  char *content_before=head->content;
  add_tail(head, (void *) &data2);
  char *content_after=head->content;
  CU_ASSERT_PTR_EQUAL(content_before,content_after);
  CU_ASSERT_EQUAL(global_realloc,1);
  global_realloc=0;
  array_list_destroy(head);
		 
}


// @ArrayList:test_reallocfail => [Ajout d'un element => realloc erreur]
void test_reallocfail(void) {

  int data=1252;
  struct array_list *head = arraylist_init((size_t) 1, (size_t) sizeof(int));
  set_element(head, 0, (void *)&data);
  fail_realloc=1;
  global_sem_post=0;
  global_sem_wait=0;
  global_realloc=0;
  int data2=17;
  int ret=add_tail(head, (void *) &data2);
  CU_ASSERT_EQUAL(ret,-1);
  CU_ASSERT_EQUAL(global_realloc,1);
  CU_ASSERT_EQUAL(global_sem_post,1);
  CU_ASSERT_EQUAL(global_sem_wait,1);
  global_realloc=0;
  global_sem_post=0;
  global_sem_wait=0;
  fail_realloc=0;
  array_list_destroy(head);
		 
}


// @ArrayList:test_meta => [Ajout d'un element => meta data correcte]
void test_meta(void) {

  int data=1252;
  struct array_list *head = arraylist_init((size_t) 1, (size_t) sizeof(int));
  set_element(head, 0, (void *)&data);
  int data2=17;
  int size_before=head->size;
  int elem_before=head->elem_size;
  add_tail(head, (void *) &data2);
  int size_after=head->size;
  int elem_after=head->elem_size;
  CU_ASSERT_NOT_EQUAL(size_before,size_after);
  CU_ASSERT_EQUAL(elem_before,elem_after);
  array_list_destroy(head);
		 
}


int main (int argc, char** argv)
{
  montest();

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
  if(NULL == CU_add_test(pSuite, "test_sem", test_sem) ||
     NULL == CU_add_test(pSuite, "test_meta", test_meta) ||
     NULL == CU_add_test(pSuite, "test_realloc", test_realloc) ||
     NULL == CU_add_test(pSuite, "test_reallocfail", test_reallocfail) ||
     NULL == CU_add_test(pSuite, "test_valeurs", test_valeurs))  
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
