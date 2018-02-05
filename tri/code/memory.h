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
 
/* Ensemble des fonctions utilisees pour verifier la bonne utilisation de la memoire de la part des etudiants. Il est de bon usage de faire un initTest() avant chaque test et un freememory() a la fin de chaque test. Attention initTest() libere toute la memoire allouee a l'aide de malloc (y compris celle allouee par le testeur). Nous traitons les cas de pointeurs NULL à l'aide de fonctions internes définies dans memory.c et lancées automatiquement dans initTest(). En cas de signal sigSEGV, le programme affiche un message d'erreur et interrompt le test en cours.
 * Attention vos allocations et desallocations de memoire sont aussi comptablilisees dans les variables globales ce qui doit etre pris en compte lors des tests.
 */
#include<stdio.h>
#include<stdlib.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>

//Permet de terminer le test, d'afficher un message d'erreur et de liberer la memoire utilisee dans le cas ou l'appel a CU_ASSERT echoue
#define NEW_CU_ASSERT(value, args...) do {	\
    int a = value;				\
    if(!a) {printf(args);			\
    freememory(); CU_FAIL(); return;}	\
  } while(0)


int global_free;		//le nombre de free utilises (par defaut = 0)
int global_malloc;		//le nombre de malloc utilises (par defaut = 0)
int global_size_malloc; //la taille totale de la memoire allouee (par defaut = 0)
int global_size_free;	//la taille de la memoire desallouee (par defaut = 0)

int fail_malloc; 		// 0=> appel a malloc reussit (valeur par defaut)
				// 1=> appel a malloc echoue de maniere temporaire
				// 2=> appel a malloc echoue de maniere definitive
int nb_fail_malloc;		// le nombre total d'appel a malloc ayant echoue (par defaut = 0)
int nb_max_malloc;		// apres nb_max_malloc appels a malloc, met fail_malloc a la valeur de type_fail (par defaut -1 => ne s'active pas)
int type_fail; 			//determine le type d'erreur de malloc (voir fail_malloc. Par defaut = 0)



//Remet l'ensemble des parametres par defaut et libere la memoire allouee precedement a l'aide de malloc. initTest() met par defaut une alarme a 1 seconde. Si on veut changer cela, il faut ajouter un appel à alarm avec comme argument le nombre de secondes desirees ou 0 si on ne veut pas de timer apres l'appel a initTest().
void initTest() ;

//Permet de liberer l'ensemble de la memoire allouee par l'etudiant et par le testeur et arrete l'eventuel timer enclenché.
void freememory();

/*Permet d'intercepter les signaux de type SIGUSR1 (trop de tentatives avortees d'acces a la memoire). 
*En cas d'interception, arrete le test en cours et envoie un message d'erreur. 
*Par defaut, le signal est lance apres 100 tentatives infructueuses. Pour modifier ce nombre, il suffit de modifier nb_fail_malloc (ainsi si nb_fail_malloc = 50, il n'y aura plus que 50 allocations ratees possibles)
*Si vous voulez utiliser cette fonction en permanence, inserez-la au debut de chaque fonction de test. 
*/
void initSignalUSR1();


//Remet le traitement par defaut des signaux de type SIGUSR1. Attention, si on utilise initSignalUSR1() dans un test mais pas dans les autres, il faut quand-même faire un appel a endSignalUSR1() a la fin du test en cours.
void endSignalUSR1();


/*
 * Petit exemple de création d'un test unitaire en utilisant les fonctions ci-dessus
 */
/*
void testExemple(){

  // fonction mettant les parametres de test a leurs valeurs par defaut
  initTest();
  
  // appel a la fonction a tester
  int i = fonctionATester();
  
  // verification de la valeur de retour
  NEW_CU_ASSERT(i == 0, "erreur : votre fonction devrait retourner 0\n");
  
  // verification de la gestion de la memoire
  NEW_CU_ASSERT(global_size_malloc == global_size_free, "erreur : vous allouez plus de memoire que vous n'en liberez\n");
  
  // libere toute la memoire allouee 
  freememory();
}
*/

