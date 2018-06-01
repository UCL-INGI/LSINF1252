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
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "student_code.h"

/* wrappers pour les appels systeme, afin de verifier que les
 * etudiants les utilisent bien
 */

int fail_open=0; // don't fail

int global_fork=0; // counter pour appels fork
int global_waitpid=0; // counter for wait
int global_open=0;
int global_dup2=0;


char *outfile="output.txt";
char *dup2file="dup2file.txt";

/* create pointers for real glibc functions */

pid_t __real_system(const char *);
pid_t __real_fork(void);
pid_t __real_waitpid(pid_t pid, int *status, int options);

int __real_open(const char *path, int oflag, mode_t mode);
int __real_dup2(int fildes, int fildes2);
FILE* __real_fopen(const char *restrict pathname, const char *restrict mode);

/* wrapping write function */

int __wrap_system(const char *command) {
  CU_FAIL("system used");
  return __real_system(command);

}

int __wrap_open(const char *path, int oflag, mode_t mode) {

  if(fail_open) {
    return -1;
  }
  global_open++;
  return __real_open(path,oflag, mode);
}

FILE* __wrap_fopen(const char *restrict pathname, const char *restrict mode) {

  if(fail_open) {
    return NULL;
  }
  global_open++;
  return __real_fopen(pathname, mode);
}

// pour compter dup2, on doit passer par un fichier car dup2 est exécuté
// dans un autre processus que celui qui exécute les tests.

int __wrap_dup2(int fd1, int fd2) {
  global_dup2++;
  int fd=__real_open(dup2file,O_CREAT|O_WRONLY|O_TRUNC,00666);
  write(fd,&global_dup2,sizeof(int));
  close(fd);
  return __real_dup2(fd1,fd2);
}

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
  __real_system("gcc -o prog -std=gnu99 myprog.c");

  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
    
    return 0;
}


// @run_redir:test_run1 => [Lancement d'un processus qui n'affiche rien => fork et waitpid sont executés]
// impossible de tester execve et dup2 car ils sont utilisés dans un autre processus, il faudrait une variable partagée ou passer par un fichier
void test_run1(void) {
  
  char *argv0[]={"prog",(char *)0};
  // reset fork counter
  global_fork=0;
  global_waitpid=0;
  int ret=run_redir("prog",argv0,outfile);
  CU_ASSERT_EQUAL(ret, 0);
  CU_ASSERT_EQUAL(global_fork, 1);
  CU_ASSERT_EQUAL(global_waitpid, 1);
  unlink(outfile);
  global_fork=0;
  global_waitpid=0;
}

// @run_redir:test_run2 => [Un fichier de sortie qui n'est pas utilisable => retourne -1] 
void test_run2(void) {
  fail_open=1;
  char *argv2[]={"noprog","a","bc",(char *)0};
  //printf("run(code,a,bc)=%d\n",run("code",argv2)); // -2
  int ret=run_redir("prog",argv2,outfile);
  CU_ASSERT_EQUAL(ret, -1);
  fail_open=0;
  unlink(outfile);
}


// @run_redir:test_run3 => [Lorsque le programme retourne correctement => doit retourner 0 et dup2 doit avoir été utilisé 2 fois]
void test_run3(void) {

  char *argv3[]={"prog", "a", (char *)0};
  global_dup2 = 0;
  unlink(dup2file);
  CU_ASSERT_EQUAL(run_redir("prog",argv3,outfile), 0);
  int fd=__real_open(dup2file,O_RDONLY,00644);
  if(fd==-1) {
    CU_FAIL("no dup2 file");
  } else {
    int err=read(fd,&global_dup2,sizeof(int));
    if(err!=sizeof(int)) {
      CU_FAIL("incomplete dup2 file");
    } 
    close(fd);
    CU_ASSERT_EQUAL(global_dup2,2);
    global_dup2=0;
  }
  unlink(outfile);
  unlink(dup2file);
}

// @run_redir:test_run4 => [Lancement d'un programme qui sort uniquement sur stdout => doit écrire dans le fichier]
void test_run4(void) {
  char *argv4[]={"prog","abcdef",(char *)0};
  unlink(outfile);
  CU_ASSERT_EQUAL(run_redir("prog",argv4,outfile),0);
  int fd=__real_open(outfile,O_RDONLY,0);
  char buf[8];
  buf[7]='\0';
  int err=read(fd,buf,7);
  CU_ASSERT_EQUAL(err,7);
  CU_ASSERT_STRING_EQUAL(buf,"abcdef-");
  err=close(fd);
  unlink(outfile);
}

// @run_redir:test_run5 => [Lancement d'un programme qui sort sur stdout et stderr => doit écrire dans le fichier]
void test_run5(void) {
  char *argv4[]={"prog","abc","def", (char *)0};
  unlink(outfile);
  CU_ASSERT_EQUAL(run_redir("prog",argv4,outfile),0);
  int fd=__real_open(outfile,O_RDONLY,0);
  char buf[8];
  buf[7]='\0';
  int err=read(fd,buf,7);
  CU_ASSERT_EQUAL(err,7);
  CU_ASSERT_STRING_EQUAL(buf,"abc-def");
  err=close(fd);
  unlink(outfile);
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
       NULL == CU_add_test(pSuite, "test_run5", test_run5) ||
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
