/* Tests unitaires pour l'exercice Producteur Consommateurs
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
#include <semaphore.h>

int global_sem_wait=0;
int global_sem_post=0;
sem_t * last_sem_wait;

int __real_sem_wait(sem_t *sem);
int __real_sem_post(sem_t *sem);

/* wrapping  functions */

int __wrap_sem_wait (sem_t *sem) {
  global_sem_wait++;
  last_sem_wait=sem;
  return __real_sem_wait(sem);
}

int __wrap_sem_post (sem_t *sem) {
  global_sem_post++;
  return __real_sem_post(sem);
}


/* les fonctions correctes */

/* 
 * @pre sp!=NULL, n>0
 * @post a construit un buffer partagé contenant n slots
 */
void _sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = calloc(n, sizeof(int)); 
    sp->n = n;                       /* Buffer content les entiers */
    sp->front = sp->rear = 0;        /* Buffer vide si front == rear */
    sem_init(&sp->mutex, 0, 1);      /* Exclusion mutuelle */
    sem_init(&sp->slots, 0, n);      /* Au début, n slots vides */
    sem_init(&sp->items, 0, 0);      /* Au début, rien à consommer */
}

/*
 * @pre sp!=NULL
 * @post libère le buffer
 */
void _sbuf_clean(sbuf_t *sp)
{
    free(sp->buf);
}

/* @pre sp!=NULL
 * @post ajoute item à la fin du buffer partagé. Ce buffer est géré
 *       comme une queue FIFO
 */
void _sbuf_insert(sbuf_t *sp, int item)
{
    sem_wait(&sp->slots);                          /* Wait for available slot */
    sem_wait(&sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;   /* Insert the item */
    sem_post(&sp->mutex);                          /* Unlock the buffer */
    sem_post(&sp->items);                          /* Announce available item */
}

/* @pre sbuf!=NULL
 * @post retire le dernier item du buffer partagé
 */
int _sbuf_remove(sbuf_t *sp)
{
    int item;
    sem_wait(&sp->items);                          /* Wait for available item */
    sem_wait(&sp->mutex);                          /* Lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];  /* Remove the item */
    sem_post(&sp->mutex);                          /* Unlock the buffer */
    sem_post(&sp->slots);                          /* Announce available slot */
    return item;
}

 
/* les fonctions de manipulation de la liste */


int init_suite1(void) {
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  return 0;
}


// @mp:test_pc1 => [vérification initalisation]
void test_pc1()
{
  sbuf_t buf;
  int semval;
  _sbuf_init(&buf,2);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,2);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,0);
  _sbuf_clean(&buf);
}

// @mp:test_pc2 => [ajout d'un element]
void test_pc2()
{
  sbuf_t buf;
  int semval;
  int val;
  _sbuf_init(&buf,2);
  sbuf_insert(&buf,17);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,1);
  val=_sbuf_remove(&buf);
  CU_ASSERT_EQUAL(val,17);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,2);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,0);
  _sbuf_clean(&buf);
}

// @mp:test_pc3 => [buffer rempli]
void test_pc3()
{
  sbuf_t buf;
  int semval;
  _sbuf_init(&buf,2);
  sbuf_insert(&buf,17);
  sbuf_insert(&buf,18);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,0);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,2);
  _sbuf_clean(&buf);
}

// @mp:test_pc4 => [sem_wait pour mutex apres sem_wait pour slots]
void test_pc4()
{
  sbuf_t buf;
  int semval;
  _sbuf_init(&buf,2);
  global_sem_wait=0;
  global_sem_post=0;
  sbuf_insert(&buf,17);
  CU_ASSERT_EQUAL(global_sem_wait,2);
  CU_ASSERT_EQUAL(global_sem_post,2);
  CU_ASSERT_PTR_EQUAL(&(buf.mutex),last_sem_wait);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,1);
  _sbuf_clean(&buf);
}

// @mp_remove:test_pc5 => [retrait d'un element dans un buffer d'un seul élement]
void test_pc5()
{
  sbuf_t buf;
  int semval;
  int val;
  _sbuf_init(&buf,2);
  _sbuf_insert(&buf,17);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,1);
  val=sbuf_remove(&buf);
  CU_ASSERT_EQUAL(val,17);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,2);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,0);
  _sbuf_clean(&buf);
}

// @mp_remove:test_pc6 => [retrait d'un élement dans un buffer rempli]
void test_pc6()
{
  sbuf_t buf;
  int semval;
  int val;
  _sbuf_init(&buf,2);
  _sbuf_insert(&buf,17);
  _sbuf_insert(&buf,18);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,0);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,2);
  val=sbuf_remove(&buf);
  CU_ASSERT_EQUAL(val,17);
  sem_getvalue(&(buf.mutex),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.slots),&semval);
  CU_ASSERT_EQUAL(semval,1);
  sem_getvalue(&(buf.items),&semval);
  CU_ASSERT_EQUAL(semval,1);  
  _sbuf_clean(&buf);
}

int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  montest();

  // on commence par la fonction de l'etudiant
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}


    CU_pSuite suiteM = CU_add_suite("Tests de Producteurs/Consommateurs",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_pc1", test_pc1) ||
       NULL==CU_add_test(suiteM, "test_pc2", test_pc2) ||
       NULL==CU_add_test(suiteM, "test_pc3", test_pc3) ||
       NULL==CU_add_test(suiteM, "test_pc4", test_pc4) ||
       NULL==CU_add_test(suiteM, "test_pc5", test_pc5) ||
       NULL==CU_add_test(suiteM, "test_pc6", test_pc6)
       ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
    
    return 0;
}
