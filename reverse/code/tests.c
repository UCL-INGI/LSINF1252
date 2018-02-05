/* Tests unitaires pour la question reverse */


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>

#include "student_code.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include <fcntl.h>


extern unsigned long crc32(char *);

// fonctions interdites

/*int fgetc(FILE *stream) {
  CU_FAIL("fgetc used");
  return(-1);
}
int getc(FILE *stream) {
  CU_FAIL("fgetc used");
  return(-1);
}

ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset) {
  CU_FAIL("pwrite used");
  return(-1);
}

ssize_t write(int fildes, const void *buf, size_t nbyte) {
  CU_FAIL("write used");
  return(-1);

}

ssize_t pread(int d, void *buf, size_t nbyte, off_t offset) {
  CU_FAIL("pread used");
  return(-1);
}
ssize_t read(int fildes, void *buf, size_t nbyte) {
  CU_FAIL("read used");
  return(-1);
}
ssize_t readv(int d, const struct iovec *iov, int iovcnt) {
  CU_FAIL("readv used");
  return(-1);
}*/

/* wrappers pour les appels systeme, afin de verifier que les
 * etudiants les utilisent bien
 */

int global_mmap=0; // counter pour appel mmap
int global_munmap=0; // counter pour munmap
int global_open=0; //
int global_close=0;

int global_forbidden=0; // fonctions interdites

int fail_open=0; // don't fail
int fail_mmap=0; // don't fail

/* create pointers for real glibc functions */


void * __real_mmap(void *addr, size_t length, int prot, int flags,
	   int fd, off_t offset);
int __real_munmap(void *addr, size_t length);

int __real_open(const char *pathname, int flags, mode_t mode);

int __real_close(int fd);

ssize_t __real_pread(int d, void *buf, size_t nbyte, off_t offset);

ssize_t __real_read(int fildes, void *buf, size_t nbyte);

FILE * __real_fopen(const char *restrict filename, const char *restrict mode);


/* wrapping  functions */

ssize_t __wrap_pread(int d, void *buf, size_t nbyte, off_t offset) {
  global_forbidden++;
  return __real_pread(d, buf, nbyte, offset);
}

ssize_t __wrap_read(int fildes, void *buf, size_t nbyte) {
  global_forbidden++;
  return __real_read(fildes, buf, nbyte);
}
FILE * __wrap_fopen(const char *restrict filename, const char *restrict mode) {
  global_forbidden++;
  return __real_fopen(filename, mode);
}


int __wrap_open(const char *pathname, int flags, mode_t mode) {
  if(fail_open) {
    return -1;
  }
  global_open++;
  return __real_open(pathname,flags, mode);

}

int __wrap_close(int fd) {
  global_close++;
  return __real_close(fd);

}

void * __wrap_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
  if (fail_mmap)
  	return NULL;
  global_mmap+=length;
  return __real_mmap(addr, length, prot, flags, fd, offset);
}
int  __wrap_munmap(void *addr, size_t length){
  global_munmap+=length;
  return __real_munmap(addr, length);
}


/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {
  //system("./dataset.sh"); // construit les fichiers de test
  int fd=__real_open("abc.txt",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"abc",3);
  __real_close(fd);
  fd=__real_open("aba.txt",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"aba",3);
  __real_close(fd);
  fd=__real_open("abc.rev",O_RDWR|O_CREAT|O_TRUNC,00644);
  write(fd,"cba",3);
  __real_close(fd);
  fd=__real_open("abcdef.rev",O_RDWR|O_CREAT|O_TRUNC,00644);
  write(fd,"fedcba",6);
  __real_close(fd);
  fd=__real_open("abcdef.txt",O_RDWR|O_CREAT|O_TRUNC,00644);
  write(fd,"abcdef",6);
  __real_close(fd);

  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  unlink("abc.txt");
  unlink("abc.rev");
  unlink("abcdef.txt");
  unlink("abcdef.rev");
    return 0;
}


// @reverse:test_reverse1 => [Lorsque mmap echoue => retourne -1 sans utiliser read, pread et fopen]
void test_reverse1(void) {
  fail_mmap=1;
  global_forbidden=0;
  int ret=reverse("aba.txt");
  fail_mmap=0;
  CU_ASSERT_EQUAL(ret, -1);
  CU_ASSERT_EQUAL(global_forbidden, 0);
  global_forbidden=0;
}

// @reverse:test_reverse2 => [Lorsque le fichier n'existe pas => ne retourne pas -1]
void test_reverse2(void) {
  int ret=reverse("nofile");
  CU_ASSERT_EQUAL(ret, -1); 
}


// @reverse:test_reverse3 => [Lorsque le fichier contient abc => retourne 0 et modifie correctement le fichier]
void test_reverse3(void) {
  unsigned long crc1,crc2;

  global_mmap=0;
  global_munmap=0;
  global_close=0;
  global_open=0;
  crc1=crc32("aba.txt");
  int ret=reverse("aba.txt");
  crc2=crc32("aba.txt");

  CU_ASSERT_EQUAL(ret, 0); 
  CU_ASSERT_EQUAL(crc1,crc2); 
  // on doit liberer le meme nombre de bytes que ce qu'on a mappe
  CU_ASSERT_EQUAL(global_mmap,global_munmap);
  CU_ASSERT_TRUE(global_mmap>0);
  CU_ASSERT_EQUAL(global_close,1);
  CU_ASSERT_EQUAL(global_close,1);
  global_mmap=0;
  global_munmap=0;
  global_open=0;
  global_close=0;
}

// @reverse:test_reverse4 => [Lorsque le fichier contient abcdef => retourne 0 et modifie correctement le fichier]
void test_reverse4(void) {
  unsigned long crc1,crc2;
  crc1=crc32("abcdef.rev");
  int ret=reverse("abcdef.txt");
  crc2=crc32("abcdef.txt");

  CU_ASSERT_EQUAL(ret, 0); 
  CU_ASSERT_EQUAL(crc1,crc2); 
}

// @reverse:test_reverse5 => [open retourne une erreur => retourne -1]
void test_reverse5(void) {
  fail_open=1;
  int ret=reverse("aba.txt");
  CU_ASSERT_EQUAL(ret, -1); 
  fail_open=0;
}

int main() {

  montest();

    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour la fonction reverse", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test_reverse1", test_reverse1) ||
       NULL == CU_add_test(pSuite, "test_reverse2", test_reverse2) ||
       NULL == CU_add_test(pSuite, "test_reverse3", test_reverse3) ||
       NULL == CU_add_test(pSuite, "test_reverse4", test_reverse4) ||
       NULL == CU_add_test(pSuite, "test_reverse5", test_reverse5)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    

    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
