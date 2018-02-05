/*   Copyright (c) 2015, 2016 Massart Aurélie & Hautecoeur Cécile
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

#if !__USE_POSIX
	#define __USE_POSIX
#endif
#define _XOPEN_SOURCE 600
#include <signal.h>
#include <setjmp.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>



/* wrappers pour les appels systeme, afin de verifier que les
 * etudiants les utilisent bien
 */

int global_free=0;
int global_malloc=0;
int global_size_malloc=0;
int global_size_free=0;
int fail_malloc = 0;
int type_fail = 0;
int nb_fail_malloc = 0;
int nb_max_malloc = -1;


/* create pointers for real glibc functions */

void __real_free(void *ptr);
void * __real_malloc(size_t size);

/* HELPERS for the wrapping functions */

//gestion de la taille de la memoire
struct nodeMemory {
	size_t elem;
	void *ptr;
	struct nodeMemory *next;
};
struct memory {
	struct nodeMemory * head;
};

struct memory *global_memory = NULL; 

/*
* Un appel a malloc a ete lance : on sauvegarde la zone memoire ainsi que sa taille dans 
* global_memory.
*/
void addMemory(size_t size, void *ptr){
	struct nodeMemory * node = (struct nodeMemory *)__real_malloc(sizeof(struct nodeMemory));
	if(node==NULL) return;

	node->elem = (int)size;
	node->ptr = ptr;
	if (global_memory == NULL) { 
		global_memory = (struct memory *)__real_malloc(sizeof(struct memory));
		if(global_memory==NULL){
			__real_free(node);
			return;
		}
		global_memory->head = NULL;
	}
	node->next = global_memory->head;
	global_memory->head=node; 
}

/*
* Un appel a free a ete lance : on supprime le noeud correspondant a la memoire 
* liberee dans global_memory et en retourne la taille. 
* Si on tente de relibere une zone deja libre, retourne -1
*/
int delMemory(void *ptr){
	if(global_memory == NULL) return -1;

	struct nodeMemory *prev = global_memory->head;
	if(prev==NULL){return -1;}

	int elem = -1;
	if(prev->ptr==ptr){
		global_memory->head = prev->next;
		elem = prev->elem; 
		__real_free(prev); prev=NULL;
		return elem;}

	struct nodeMemory *current = prev->next;
	while(current!=NULL){
		if(current->ptr == ptr) {
			prev->next=current->next;
			elem = prev->elem;
			__real_free(current); current=NULL;
			return elem;
		}
		prev = current;
		current=current->next;
	}
	return-1;
}

/*
* sous-fonction de freememory qui libere la memoire associe au noeud courrant ainsi qu'aux suivants
*/
void freeNodeM(struct nodeMemory* myNode){
	if(myNode != NULL){
		__real_free(myNode->ptr);
		myNode->ptr = NULL;
		freeNodeM(myNode->next);
	__real_free(myNode);
	myNode = NULL;}
}  

/*
* Permet de reinitialiser global_memory 
* et de desallouer les eventuels pointeurs oublies
* Arrete l'eventuel timer en cours
*/
void freememory(){
	alarm(0);
	if(global_memory==NULL){return;}
	if(global_memory->head!=NULL){
		freeNodeM(global_memory->head);}
	__real_free(global_memory);
	global_memory=NULL;
}

//gestion du temps

struct timeval *tv = NULL;
/*
* Verifie qu'il s'est passe plus de 1 seconde depuis la derniere erreur de malloc
* retourne 1 si oui et 0 sinon
*/
int isMorethan1sec(){
	struct timeval tv2;
	gettimeofday(&tv2,NULL);
	if(fail_malloc==2) return 0; //il n'y a vraiment plus de place de stockage 
	int time = (tv2.tv_sec-tv->tv_sec)*pow(10,6)+tv2.tv_usec-tv->tv_usec;
	if(time >= pow(10,6)){ //il s'est passe plus d'une seconde
		tv->tv_sec = 0; 
		fail_malloc=0; //malloc est à nouveau permis
		return 1;
	}		
	return 0;
}

//traitement des signaux
sigjmp_buf buf;
sigjmp_buf buf1;
sigjmp_buf buf2;

void sig_handler(int signum) {
	if(signum==SIGSEGV) { siglongjmp(buf,1);}
	if(signum==SIGALRM) siglongjmp(buf1,2); 
	if(signum==SIGUSR1) siglongjmp(buf2,3);
	freememory(); CU_FAIL_FATAL("erreur inatendue \n"); //jamais execute
}

//Permet d'intercepter les signaux de type SIGSEGV (erreur d'acces a la memoire). En cas d'interception, arrete le test en cours et envoie un message d'erreur. 
void initSignalSEGV(){   
	if(signal(SIGSEGV,sig_handler)==SIG_ERR){
		printf("Erreur dans le traitement des signaux");
		freememory(); return;
	} 
	int r = sigsetjmp(buf,1);
    if (r!=0) {printf("erreur dans l'acces de la memoire, geres-tu bien les pointeurs NULL ?\n"); freememory(); CU_FAIL_FATAL(); return;}
}

//Permet d'intercepter les signaux de type SIGALRM (timer expire). En cas d'interception, arrete le test en cours et envoie un message d'erreur. 
void initSignalALRM(){   
	if(signal(SIGALRM,sig_handler)==SIG_ERR){
		printf("Erreur dans le traitement des signaux");
		freememory(); return;
	} 
	if(siginterrupt(SIGALRM,1)<0){
		printf("Erreur dans le traitement des signaux");
		freememory(); return;
	}
	int r = sigsetjmp(buf1,2);
    if (r!=0) {printf("Le timer a expire. Ne fais-tu pas de boucles infinies ?\n"); freememory(); CU_FAIL_FATAL(); return;}
}

// indique si le signal USR1 est traite ou non
int is_sigUSR1 = 0;

//Permet d'intercepter les signaux de type SIGUSR1 (trop de tentatives avortees d'acces a la memoire). En cas d'interception, arrete le test en cours et envoie un message d'erreur. 
void initSignalUSR1(){   
	if(signal(SIGUSR1,sig_handler)==SIG_ERR){
        is_sigUSR1 = 1;
		printf("Erreur dans le traitement des signaux");
		freememory(); return;
	} 
	int r = sigsetjmp(buf2,3);
    if (r!=0) {printf("Il arrive aussi qu'il n'y ait tout simplement plus de place diponible, inutile de faire malloc en permanence\n"); freememory(); CU_FAIL(); return;}
}

//Remet le traitement par defaut des signaux de type SIGUSR1
void endSignalUSR1(){
	if(signal(SIGUSR1,SIG_DFL)==SIG_ERR){
        is_sigUSR1 = 0;
		printf("Erreur dans le traitement des signaux");
		freememory(); return;
	} 
}

/* wrapping  functions */

void __wrap_free(void *ptr) {
  if(ptr == NULL) {return;}

  int size = delMemory(ptr);
  if (size==-1) {printf("Vous essayez de libere une memoire non-allouee, ça peut causer des erreurs inatendues! \n");freememory(); CU_FAIL(); return;}
  else{	
	global_free ++;
  	global_size_free = global_size_free+size;
  	__real_free(ptr);
  }
}

void* __wrap_malloc(size_t size){
	nb_max_malloc--;
	if(nb_max_malloc==-1) fail_malloc=type_fail; //l'erreur de malloc est de type permanente (type_fail=2) ou provisoire (type_fail = 1)

	if(fail_malloc!=0){
		nb_fail_malloc++;
		if(nb_fail_malloc > 100) { if(is_sigUSR1 == 1) kill(0,SIGUSR1); }//on tente trop d'appels a malloc qui echouent
		if(tv->tv_sec==0){
			gettimeofday(tv, NULL); return NULL;} //premiere erreur 

		if(!isMorethan1sec()) return NULL; 
	}
	global_size_malloc = global_size_malloc+size;
	void* ptr = __real_malloc(size);
	if (ptr != NULL) addMemory(size, ptr);
	global_malloc ++;
	return ptr;
}

/*
* Fonction qui initialise les parametres de test
*/ 
void initTest() {
    initSignalSEGV();
    initSignalALRM();

	fail_malloc = 0;
	nb_fail_malloc = 0;
    nb_max_malloc = -1; //pas de limitation
	freememory();

	global_memory = (struct memory *)__real_malloc(sizeof(struct memory));
	if(global_memory == NULL) {CU_FAIL("Pas assez de memoire"); return;}
	global_memory->head = NULL;
	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if(tv == NULL) { CU_FAIL("Pas assez de memoire"); return;}
	tv->tv_sec = 0; // indique qu'il n'y a pas encore eu d'erreur de malloc
	global_malloc = 0;
	global_free=0;
	global_size_malloc = 0;
	global_size_free = 0;
	type_fail = 0;
    alarm(1);
}


