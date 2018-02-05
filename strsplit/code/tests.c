/* Tests unitaires pour l'exercice strsplit
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



// fonctions interdites

char * strcat(char *restrict s1, const char *restrict s2) {
  CU_FAIL("strcat used");
  return(NULL);
}
char * strncat(char *restrict s1, const char *restrict s2, size_t n) {
  CU_FAIL("strncat used");
  return(NULL);
}

int global_malloc_count=0; // counter pour appels malloc
int global_malloc_bytes=0; // counter pour bytes allouées malloc
int global_free_count=0; // counter nombre d'appels à free
int global_malloc_fail=-1; // -1 pas de panne, 1 une erreur, >0 décremente  à chaque apple
int global_malloc_nonzero=0;
void * last_malloc=NULL;
void * last_free=NULL;

void*  __real_malloc(size_t size);
void __real_free(void *ptr);
void * __real_calloc(size_t n, size_t size);

void* __wrap_malloc (size_t size) {
  global_malloc_count++;
  global_malloc_bytes+=size;
  if(global_malloc_fail==1) {
    return NULL;
  }
  if(global_malloc_fail>0)
    global_malloc_fail--;
  last_malloc=__real_malloc(size);
  if(global_malloc_nonzero==1) {
    char *c=(char *) last_malloc;
    for(int i=0;i<size;i++) {
      c[i]='A';
    }
  }
  return last_malloc;
}

void* __wrap_calloc (size_t n, size_t size) {
  global_malloc_count++;
  global_malloc_bytes+=size;
  if(global_malloc_fail==1) {
    return NULL;
  }
  if(global_malloc_fail>0)
    global_malloc_fail--;
  last_malloc=__real_malloc(size);

  return last_malloc;
}


void __wrap_free(void *ptr) {
  global_free_count++;
  last_free=ptr;
  return __real_free(ptr);
}


/* les fonctions de manipulation du string */

/*
 * @pre 0<n<strlen(str), str!=NULL, str se termine par '\0'
 * @post dans un tableau deux pointeurs vers deux chaînes de caractères
 *       terminées par '\0'. La première (indice 0) contient
 *       les caractères en positions 0 à n-1 dans str.
 *       La seconde le reste de la chaîne str. 
 *       En cas d'erreur, retourne NULL en ayant libéré correctement
 *       tout mémoire allouée avant l'erreur.
 *
 * Contrainte : la seule fonction de manipulation des chaînes de caractères
 *              que vous pouvez utiliser est strlen. Les autres (e.g. 
 *              strcat, strncat, strstr, ...) sont interdites. Vous pouvez
 *              bien entendu utiliser malloc et free.
 */
char** _strsplit(const char *str, int n) {
  char ** ret;
  int len=strlen(str);
  ret=(char **)malloc(2*sizeof(char *));
  if(ret==NULL) {
    return NULL;
  }
  ret[0]=(char *)malloc((n+1)*sizeof(char));
  if(ret[0]==NULL) {
    free((void *)ret);
    return NULL;
  }
  ret[1]=malloc((len-n+1)*sizeof(char));
  if(ret[1]==NULL) {
    free(ret[0]);
    free(ret);
    return NULL;
  }
  char *c=ret[0];
  for(int i=0;i<n;i++) {
    c[i]=str[i];
  }              
  //c[n]='\0';

  char *d=ret[1];
  for(int i=n;i<len;i++) {
    d[i-n]=str[i];
  }
  //d[len-n]='\0';
  return ret;

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


// @split:test_1 => [string coupé en deux au milieu, pas d'erreur malloc (normalement trois malloc sont nécessaires, mais il est possible de le faire en deux malloc]
void test_1()
{
  char * str="Olivier";
  char **ret;
  global_malloc_fail=-1;
  global_malloc_count=0;
  global_malloc_bytes=0;
  ret=strsplit(str,3);
  CU_ASSERT_STRING_EQUAL("Oli",ret[0]);
  CU_ASSERT_STRING_EQUAL("vier",ret[1]);
  CU_ASSERT_EQUAL(global_malloc_bytes,2*sizeof(char *)+(strlen("Oli")+1)*sizeof(char)+(strlen("vier")+1)*sizeof(char)); // pas trop de bytes allouées
  CU_ASSERT(global_malloc_count>=2);

}
// @split:test_2 => [string coupé en deux au milieu, erreur au premier malloc]
void test_2()
{
  char * str="Olivier";
  char **ret;
  global_malloc_fail=1;
  global_malloc_count=0;
  global_malloc_bytes=0;
  ret=strsplit(str,3);
  CU_ASSERT_PTR_EQUAL(ret,NULL);
  CU_ASSERT_EQUAL(global_malloc_count,1);


}

// @split:test_3 => [string coupé en deux au milieu, erreur au deuxième malloc]
void test_3()
{
  char * str="Olivier";
  char **ret;
  global_malloc_fail=2;
  global_malloc_count=0;
  global_malloc_bytes=0;
  ret=strsplit(str,3);
  CU_ASSERT_PTR_EQUAL(ret,NULL);
  CU_ASSERT_EQUAL(global_malloc_count,2);
  CU_ASSERT_EQUAL(global_free_count,1); // au moins un free
  CU_ASSERT_PTR_EQUAL(last_free,last_malloc); // free doit être le malloc qui a réussi


}

// @split:test_4 => [string coupé en deux à la fin, pas d'erreur malloc (normalement trois malloc sont nécessaires, mais il est possible de le faire en deux malloc]
void test_4()
{
  char * str="Olivier";
  char **ret;
  global_malloc_fail=-1;
  global_malloc_count=0;
  global_malloc_bytes=0;
  ret=strsplit(str,6);
  CU_ASSERT_STRING_EQUAL("Olivie",ret[0]);
  CU_ASSERT_STRING_EQUAL("r",ret[1]);
  CU_ASSERT_EQUAL(global_malloc_bytes,2*sizeof(char *)+(strlen("Olivie")+1)*sizeof(char)+(strlen("r")+1)*sizeof(char)); // pas trop de bytes allouées
  CU_ASSERT(global_malloc_count>=2);

}

// @split:test_5 => [string coupé en deux au début, malloc n'alloue pas de la mémoire à zero]
void test_5()
{
  char * str="Olivier";
  char **ret;
  global_malloc_nonzero=1;
  ret=strsplit(str,1);
  CU_ASSERT_EQUAL(strlen(ret[0]),1);
  CU_ASSERT_EQUAL(strlen(ret[1]),6);
  CU_ASSERT_STRING_EQUAL("O",ret[0]);
  CU_ASSERT_STRING_EQUAL("livier",ret[1]);

}


int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  //montest();
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}


    CU_pSuite suiteM = CU_add_suite("Tests de strsplit",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_1", test_1) ||
       NULL==CU_add_test(suiteM, "test_2", test_2) || 
       NULL==CU_add_test(suiteM, "test_3", test_3) ||
       NULL==CU_add_test(suiteM, "test_4", test_4) || 
       NULL==CU_add_test(suiteM, "test_5", test_5)  
       ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}