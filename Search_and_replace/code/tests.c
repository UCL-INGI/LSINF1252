/* Tests unitaires pour l'exercice sr
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


#include <fcntl.h>

extern unsigned long crc32(char *);

// fonctions interdites
ssize_t pread(int d, void *buf, size_t nbyte, off_t offset) {
  CU_FAIL("pread used");
  return(-1);
}
void * mmap(void *addr, size_t len, int prot, int flags, int fd,
	    off_t offset) {
  CU_FAIL("mmap used");
  return(NULL);
}

int global_open=0; // counter pour appel open
int global_close=0; // counter pour appel close
int global_read=0;
int global_write=0;

int global_malloc=0;
int global_free=0;

int fail_malloc=0; // don't fail
int fail_open=0; // don't fail

/* create pointers for real glibc functions */

int __real_open(const char *path, int oflag, mode_t mode);
int __real_close(int fd);
ssize_t __real_read(int fildes, void *buf, size_t nbyte);
ssize_t __real_write(int fildes, const void *buf, size_t nbyte);
void*  __real_malloc(size_t size);
void __real_free(void *ptr);

/* wrapping  functions */

int __wrap_open(const char *path, int oflag, mode_t mode) {

  if(fail_open) {
    return -1;
  }
  global_open++;
  return __real_open(path,oflag, mode);
}

int __wrap_close(int fd) {
  global_close++;
  return __real_close(fd);
}

ssize_t __wrap_read(int fildes, void *buf, size_t nbyte) {
  global_read+=nbyte;
  return __real_read(fildes,buf,nbyte);
}

ssize_t __wrap_write(int fildes, void *buf, size_t nbyte) {
  global_write+=nbyte;
  return __real_write(fildes,buf,nbyte);
}

void* __wrap_malloc (size_t size) {
  global_malloc++;
  if(fail_malloc)
    return NULL;
  return __real_malloc(size);
}

void __wrap_free(void *ptr) {
  global_free++;
  return __real_free(ptr);
}

/* La fonction d'initialisation de la suite de tests.
 * Retourne 0 en cas de succ<C3><A8>s, n'importe quelle autre valeur indique un <C3>
<A9>chec.
*/
int init_suite1(void) {
  int fd=__real_open("abc.txt",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"abc",3);
  __real_close(fd);
  // remplacement de a par z
  fd=__real_open("abc.res",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"zbc",3);
  __real_close(fd);
  fd=__real_open("aba.txt",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"aba",3);
  __real_close(fd);
  // remplacement de a par z
  fd=__real_open("aba.res",O_CREAT|O_RDWR|O_TRUNC,00644);
  write(fd,"zbz",3);
  __real_close(fd);

  fd=__real_open("int.txt",O_RDWR|O_CREAT|O_TRUNC,00644);
  int num1=1252;
  int num2=17;
  __real_write(fd,&num1,sizeof(int));
  __real_write(fd,&num2,sizeof(int));
  __real_write(fd,&num2,sizeof(int));
  __real_write(fd,&num1,sizeof(int));
  __real_close(fd);
  // remplace de 17 par 2141
  fd=__real_open("int.res",O_RDWR|O_CREAT|O_TRUNC,00644);
  num1=1252;
  num2=2141;
  __real_write(fd,&num1,sizeof(int));
  __real_write(fd,&num2,sizeof(int));
  __real_write(fd,&num2,sizeof(int));
  __real_write(fd,&num1,sizeof(int));
  __real_close(fd);
  fd=__real_open("vide.txt",O_RDWR|O_CREAT|O_TRUNC,00644);
  __real_close(fd);
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  unlink("abc.txt");
  unlink("abc.res");
  unlink("aba.txt");
  unlink("aba.res");
  unlink("int.txt");
  unlink("int.res");
  unlink("vide.txt");
  return 0;
}


// @sr:test_sr1 => [fichier abc remplacement a == 1 remplacement]
void test_sr1()
{
  unsigned long crc1, crc2;
  crc1=crc32("abc.res");
  char a='a';
  char z='z';
  int result = sr("abc.txt",&a,&z,1);
  crc2=crc32("abc.txt");
  CU_ASSERT_EQUAL(result, 1);
  CU_ASSERT_EQUAL(crc1,crc2);

}


// @sr:test_sr2 => [fichier vide == 0]
void test_sr2()
{
  char a='a';
  int result = sr("vide.txt",&a,&a,1);
  CU_ASSERT_EQUAL(result, 0);
}

// @sr:test_sr3 => [erreur open == -1]
void test_sr3()
{
  fail_open=1;
  global_malloc=0;
  global_free=0;
  char a='a';
  char z='z';
  int result =sr("nonexistent.txt",&a,&z,1);
  CU_ASSERT_EQUAL(result, -1);
  CU_ASSERT_EQUAL(global_malloc,global_free);
  global_malloc=0;
  global_free=0;
  fail_open=0;
}

// @sr:test_sr4 => [erreur malloc == -1 et close]
void test_sr4()
{
  fail_malloc=1;
  global_open=0;
  global_close=0;
  char a='a';
  char z='z';
  int result = sr("abc.txt",&a,&z,1);
  CU_ASSERT_EQUAL(result, -1);
  global_malloc=0;
  global_close=0;
  global_open=0;
  fail_malloc=0;
  
}


// @sr:test_sr5 => [fichier aba == 2]
void test_sr5()
{
  long crc1, crc2;
  crc1=crc32("aba.res");
  char a='a';
  char z='z';
  int result = sr("aba.txt",&a,&z,1);
  crc2=crc32("aba.txt");
  CU_ASSERT_EQUAL(result, 2);
  CU_ASSERT_EQUAL(crc1,crc2);
  
}


// @sr:test_sr6 => [fichier int == 2]
void test_sr6()
{
  long crc1, crc2;
  crc1=crc32("int.res");
  int num2=17;
  int num3=2141;
  int result = sr("int.txt",(char *) &num2,(char *)&num3,sizeof(int));
  crc2=crc32("int.txt");
  CU_ASSERT_EQUAL(result, 2);
  CU_ASSERT_EQUAL(crc1,crc2);
  
}

// @sr:test_sr7 => [fichier abc remplacement y == 0 remplacement]
void test_sr7()
{
  unsigned long crc1, crc2;
  crc1=crc32("abc.txt");
  char a='y';
  char z='!';
  int result = sr("abc.txt",&a,&z,1);
  crc2=crc32("abc.txt");
  CU_ASSERT_EQUAL(result, 0);
  CU_ASSERT_EQUAL(crc1,crc2);

}

int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  montest();
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}


    CU_pSuite suiteM = CU_add_suite("Tests de sr",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_sr1", test_sr1) ||
       NULL==CU_add_test(suiteM, "test_sr2", test_sr2) || 
       NULL==CU_add_test(suiteM, "test_sr3", test_sr3) || 
       NULL==CU_add_test(suiteM, "test_sr4", test_sr4) || 
       NULL==CU_add_test(suiteM, "test_sr5", test_sr5) || 
       NULL==CU_add_test(suiteM, "test_sr7", test_sr7) || 
       NULL==CU_add_test(suiteM, "test_sr6", test_sr6)) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}
