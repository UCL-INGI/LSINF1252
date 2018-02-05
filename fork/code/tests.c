/* Tests unitaires pour la question run */

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

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>

#include "student_code.h"

/* wrappers pour les appels systeme, afin de verifier que les
 * etudiants les utilisent bien
 */

int global_fork=0; // counter pour appels fork
int global_waitpid=0; // counter for wait

/* create pointers for real glibc functions */

pid_t __real_fork(void);
pid_t __real_waitpid(pid_t pid, int *status, int options);



/* wrapping write function */


pid_t __wrap_fork(void)
{
  /* printing out the number of characters */
  global_fork++;

  /* call the real glibc function and return the result */
  pid_t result = __real_fork();
  return result;
}


pid_t __wrap_waitpid(pid_t pid, int *status, int options){
  global_waitpid++;
  pid_t result=__real_waitpid(pid, status, options);
  return result;
    
}

/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {
  // compile the process
  system("gcc -o prog code.c");
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
    
    return 0;
}


// @fork:test_run1 => [Lancement d'un processus qui se termine => retourne 0 et appele fork et waitpid ]
void test_run1(void) {

  char *argv0[]={"prog","a",(char *)0};

  // reset fork counter
  global_fork=0;
  global_waitpid=0;
  int ret=run("prog",argv0);
  CU_ASSERT_EQUAL(ret, 0);
  CU_ASSERT_EQUAL(global_fork, 1);
  CU_ASSERT_EQUAL(global_waitpid, 1);
  global_fork=0;
  global_waitpid=0;
}

// @fork:test_run2 => [Lorsque le programme est interrompu par signal => ne retourne pas -2]
void test_run2(void) {

  char *argv2[]={"prog","a","bc",(char *)0};
  //printf("run(code,a,bc)=%d\n",run("code",argv2)); // -2
  int ret=run("prog",argv2);
  CU_ASSERT_EQUAL(ret, -2); 
}


// @fork:test_run3 => [Lorsque le programme retourne correctement => doit retourner 0]
void test_run3(void) {

  char *argv3[]={"prog", "a", (char *)0};

  //  printf("run(code)=%d\n",run("code",argv3)); // 0

  CU_ASSERT_EQUAL(run("prog",argv3), 0); 
}

// @fork:test_run4 => [Lancement d'un programme qui n'est pas executable => doit retourner -1]
void test_run4(void) {
  char *argv4[]={"code.c",(char *)0};

  CU_ASSERT_EQUAL(run("code.c",argv4),-1); 
}

int main() {
  montest();

    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour la question run", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     */
    if(NULL == CU_add_test(pSuite, "test_run1", test_run1) ||
       NULL == CU_add_test(pSuite, "test_run2", test_run2) ||
       NULL == CU_add_test(pSuite, "test_run3", test_run3) ||
       NULL == CU_add_test(pSuite, "test_run4", test_run4) 
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    

    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
