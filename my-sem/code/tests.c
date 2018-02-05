/* Tests unitaires pour my-sem
Copyright (C) 2016 Dubray Alexandre, Mathieu Xhonneux

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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <pthread.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "student_code.h"

int nb_times_destroy_used = 0;
int count = 0;

int init_suite1(void){
	return 0;
}

int clean_suite1(void){
	return 0;
}

int mysem_init(mysem_t *sem, unsigned int value){
	sem->value = value;
	sem->capacity = value;
	sem->blocked_procs = NULL;
	pthread_mutex_init(&(sem->mutex),NULL);
	return 0;
}

int working_mysem_wait(mysem_t *sem){
	if(sem->value == 0){
		/* Si value < 0 on crée un nouveau processus et on le met dans la queue */
		sem_process_t *process = (sem_process_t *)malloc(sizeof(sem_process_t));
		if(process == NULL)
			return -1;

		process->next = NULL;
		pthread_mutex_init(&(process->mutex),NULL);
		/* On lock le mutex pour pouvoir bloquer le process quand on l'aura ajouté à la queue */
		pthread_mutex_lock(&(process->mutex));
		/* Accès à la queue */
		pthread_mutex_lock(&(sem->mutex));

		if(sem->blocked_procs == NULL){
			sem->blocked_procs = process;
			pthread_mutex_unlock(&(sem->mutex));
			/* On bloque le process tant que personne ne fait unlock */
			pthread_mutex_lock(&(process->mutex));
		}
		else{
			sem_process_t *iterateur = sem->blocked_procs;
			while(iterateur->next != NULL)
				iterateur = iterateur->next;
			iterateur->next = process;
			pthread_mutex_unlock(&(sem->mutex));
			pthread_mutex_lock(&(process->mutex));
		}
		return 0;
	}

	sem->value--;
	return 0;
}

int working_mysem_post(mysem_t *sem){
	if(sem->blocked_procs == NULL) {
        if (sem->value < sem->capacity)
            sem->value++;
		return 0;
	}
	/* On bloque l'accès à la liste */
	pthread_mutex_lock(&(sem->mutex));

	sem_process_t *processToFree = sem->blocked_procs;
	sem->blocked_procs = (sem->blocked_procs)->next;

	pthread_mutex_unlock(&(processToFree->mutex));
	pthread_mutex_unlock(&(sem->mutex));

    pthread_mutex_destroy(&(processToFree->mutex));

	return 0;
}



int mysem_close(mysem_t *sem){
	if(sem->blocked_procs == NULL){
		return 0;
	}
	sem_process_t *iterator = sem->blocked_procs;
	while(iterator != NULL){
		pthread_mutex_unlock(&(iterator->mutex));
		sem_process_t *next = iterator->next;
		free(iterator);
		iterator = next;
	}

	return 0;
}


/* Méthode auxilaire permettant de calculer la taille d'une liste chaîneé */
int length_list(sem_process_t *blocked_procs){
	int length;
	for(length = 0; blocked_procs != NULL; length ++)
		blocked_procs = blocked_procs->next;
	return length;
}

/* Permet de compter le nombre d'appel réussis à sem_wait */
int countAppelReussis;

void *auxiWaitBloquant(void *arg){
	mysem_t * sem = (mysem_t *) arg;
	int i;
	for(i=0;i<3;i++){
		int err = mysem_wait(sem);
		if(err == 0){
			countAppelReussis ++;
		}
	}
	pthread_exit(NULL);
}

// @mysem_wait:test_wait_bloquant => [mysem_wait ne bloque pas le processus quand sem->value = 0]
void test_wait_bloquant(void){
	mysem_t *sem=(mysem_t *)malloc(sizeof(mysem_t));
	mysem_init(sem,2);
	countAppelReussis = 0;

	pthread_t th;
	pthread_create(&th,NULL,&auxiWaitBloquant,(void *)sem);
	usleep(200);
	/* On test le nombre d'appel réussis */
	CU_ASSERT_EQUAL(countAppelReussis,2);
	working_mysem_post(sem);
	usleep(200);
	CU_ASSERT_EQUAL(countAppelReussis,3);

	//mysem_close(sem);
}
/* Threads pour le test test_wait_add_end */
void *auxi_test_wait_end1(void *arg){
	mysem_t *sem = (mysem_t *)arg;
	mysem_wait(sem);
	mysem_wait(sem);
	mysem_wait(sem);
	pthread_exit(NULL);
}

void *auxi_test_wait_end2(void *arg){
	mysem_t *sem = (mysem_t *)arg;
	mysem_wait(sem);
	pthread_exit(NULL);
}
// @mysem_wait:test_wait_add_end => [mysem_wait n'ajoute pas les processus à la fin de blocked_procs]
void test_wait_add_end(void){
	mysem_t *sem=(mysem_t *)malloc(sizeof(mysem_t));
	mysem_init(sem,2);
	pthread_t th[2];
	/* On crée le premier thread qui va mettre la sémaphore à 0 et ajouter un processus dans la queue */
	pthread_create(&(th[0]),NULL,&auxi_test_wait_end1,(void *)sem);
	usleep(200);
	sem_process_t *firstProcess = sem->blocked_procs;
	/* On crée le deuxième thread qui va être directement bloquer et ajouter un processus dans la queue */
	pthread_create(&(th[1]),NULL,&auxi_test_wait_end2,(void *)sem);
	usleep(200);
	/* On check que la tête de la queue est toujours le premier processus */
	CU_ASSERT_PTR_EQUAL(firstProcess,sem->blocked_procs);
	/* On check qu'il y a bien un deuxième élément dans la queue */
    CU_ASSERT_PTR_NOT_NULL(firstProcess);
    if (firstProcess != NULL)
        CU_ASSERT_PTR_NOT_NULL(firstProcess->next);
	//mysem_close(sem);
}

void *auxi_test_post_count(void *arg){
	mysem_t *sem = (mysem_t *)arg;
	working_mysem_wait(sem);
    count++;
	pthread_exit(NULL);
}
// @mysem_post:test_post_count => [mysem_post ne débloque pas le processus]
void test_post_count(void){
	mysem_t *sem=(mysem_t *)malloc(sizeof(mysem_t));
	mysem_init(sem,1);
	working_mysem_wait(sem);

	pthread_t th[3];
	for(int i=0;i<3;i++)
		pthread_create(&(th[i]),NULL,&auxi_test_post_count,(void *)sem);

    usleep(500); //on passe la main aux threads pour qu'ils fassent leur wait
	mysem_post(sem);
	mysem_post(sem);
    usleep(500);

    CU_ASSERT_EQUAL(count, 2);

	//mysem_close(sem);
}


// @mysem_post:test_post_take_first => [mysem_post ne retire pas le premier processus de la liste]
void test_post_take_first(void){
	mysem_t *sem=(mysem_t *)malloc(sizeof(mysem_t));
	mysem_init(sem,1);
	working_mysem_wait(sem);
	pthread_t th[3];

	/* On réutilise la fonction auxiliaire du test précédent */
	int i;
	for(i=0;i<3;i++)
		pthread_create(&(th[i]),NULL,&auxi_test_post_count,(void *)sem);
	usleep(200);

    sem_process_t *following = sem->blocked_procs->next;
    mysem_post(sem);
    CU_ASSERT_PTR_EQUAL(sem->blocked_procs,following);
	CU_ASSERT_EQUAL(length_list(sem->blocked_procs),2);

	//mysem_close(sem);
}

// @mysem_post:test_post_exceed_capacity => [mysem_post : value > capacity]
void test_post_exceed_capacity(void){
	mysem_t *sem=(mysem_t *)malloc(sizeof(mysem_t));
    sem->value = 2;
    sem->capacity = 2;
    sem->blocked_procs = NULL;
	pthread_mutex_init(&(sem->mutex),NULL);

	CU_ASSERT_EQUAL(mysem_post(sem), 0);
	CU_ASSERT_EQUAL(sem->value,2);
	working_mysem_wait(sem);
	CU_ASSERT_EQUAL(mysem_post(sem), 0);
	CU_ASSERT_EQUAL(mysem_post(sem), 0);
	CU_ASSERT_EQUAL(sem->value,2);

    pthread_mutex_destroy(&(sem->mutex));
    free(sem);
}


/* Fonction qui ajoute un élément à la queue */
void *auxi_test_post_destroy(void *arg){
	mysem_t *sem = (mysem_t *)arg;
	working_mysem_wait(sem);
	pthread_exit(NULL);
}

// @mysem_post:test_post_destroy => [La fonction post ne détruit pas le mutex utilisé.]
void test_post_destroy(void){
	nb_times_destroy_used = 0;
	mysem_t *sem = malloc(sizeof(mysem_t));
	mysem_init(sem,1);
	working_mysem_wait(sem);
	pthread_t th[3];
	int i;
	for(i=0;i<3;i++)
		pthread_create(&(th[i]),NULL,&auxi_test_post_destroy,(void *)sem);
	usleep(200);
	mysem_post(sem);
	mysem_post(sem);
	mysem_post(sem);
	CU_ASSERT_EQUAL(nb_times_destroy_used,3);
}
int main(){
	CU_pSuite pSuite = NULL;

	if(CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	pSuite = CU_add_suite("Suite de tests pour my-sem",init_suite1,clean_suite1);
	if(pSuite == NULL){
		CU_cleanup_registry();
		return CU_get_error();
	}
	if(CU_add_test(pSuite, "test_wait_bloquant",test_wait_bloquant) == NULL ||
       CU_add_test(pSuite,"test_wait_add_end",test_wait_add_end) == NULL || 
       CU_add_test(pSuite,"test_post_count",test_post_count) == NULL||
       CU_add_test(pSuite,"test_post_take_first",test_post_take_first) == NULL ||
       CU_add_test(pSuite,"test_post_destroy",test_post_destroy) == NULL ||
       CU_add_test(pSuite,"test_post_exceed_capacity",test_post_exceed_capacity) == NULL) {

	    CU_cleanup_registry();
		return CU_get_error();
	}

	CU_automated_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}




