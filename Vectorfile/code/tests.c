/* Tests unitaires pour l'exercice Vectorfile
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
off_t lseek(int fildes, off_t offset, int whence) {
  CU_FAIL("lessk used");
  return(-1);

}

int global_open=0; // counter pour appel open
int global_close=0; // counter pour appel close
int global_read=0;
int global_write=0;
int fail_read=0;
int fail_write=0;
int fail_open=0;


int __real_open(const char *path, int oflag, mode_t mode);
int __real_close(int fd);
ssize_t __real_read(int fildes, void *buf, size_t nbyte);
ssize_t __real_write(int fildes, const void *buf, size_t nbyte);
//void*  __real_malloc(size_t size);
//void __real_free(void *ptr);

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
  if(fail_read)
    return -1;

  global_read+=nbyte;
  return __real_read(fildes,buf,nbyte);
}

ssize_t __wrap_write(int fildes, void *buf, size_t nbyte) {
  if (fail_write)
    return -1;
  global_write+=nbyte;
  return __real_write(fildes,buf,nbyte);
}
/*
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
*/

/* les fonctions de manipulation du fichier */

/*
 * @pre *filename!=NULL, size>0
 * @post construit un fichier contenant size double à la valeur val
 *       retourne le nombre de données écrites, -1 en cas d'erreur
 */
// fourni
int _create(char *filename, int size, double val) {

  int err;
  int fd=open(filename,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
  if(fd==-1) {
    return -1;
  }
  for(int i=0; i<size; i++) {
    err=write(fd,(void *) &val, sizeof(double));
    if(err<0) {
       err=close(fd);
       return(-1);
     }
   }
   err=close(fd);
   if(err==-1)
     return err;
   else
     return size;
 }

// non fourni
int _set(char *filename, int index, double val) {
   int err;
   double t __attribute__ ((unused));
   int fd=open(filename,O_RDWR);
   if(fd==-1) {
     return -1;
   }
   for(int i=0; i<index; i++) {
     err=read(fd,(void *) &t, sizeof(double));
     if(err<0) {
       err=close(fd);
       return(-1);
     }
   }
   err=write(fd,(void *) &val, sizeof(double));
   if(err==-1) {
     err=close(fd);
     return -1;
   }
   err=close(fd);
   if(err==-1)
     return err;
   else
     return 0;

 }
// non fourni
int _get(char *filename, int index, double * val) {
   int err;
   double t __attribute__ ((unused));
   int fd=__real_open(filename,O_RDONLY,0);
   if(fd==-1) {
     return -1;
   }
   for(int i=0; i<index; i++) {
     err=__real_read(fd,(void *) &t, sizeof(double));
     if(err<0) {
       err=__real_close(fd);
       return(-1);
     }
   }
   err=__real_read(fd,(void *) val, sizeof(double));
   err=__real_close(fd);
   if(err==-1)
     return err;
   else
     return 0;

 }

// la fonction demandée aux étudiants
int _swap(char *filename, int i, int j) {
  int err;
  if(i==j) 
    return 0;

  int index1, index2;
  if(i<j) {
    index1=i;
    index2=j;
  } else {
    index1=j;
    index2=i;
  }
  double v1, v2;
  err=_get(filename, index1, &v1);
  if(err<0) 
    return err;

  err=_get(filename, index2, &v2);
  if(err<0) 
    return err;

  err=_set(filename, index1, v2);
  if(err<0) 
    return err;

  err=_set(filename, index2, v1);
  if(err<0) 
    return err;
  return 0;
}

// a retirer de la version finale
/*int swap(char *filename, int i, int j) {
  return _swap(filename,i,j);
}*/


/* La fonction d'initialisation de la suite de tests.
 * Retourne 0 en cas de succ<C3><A8>s, n'importe quelle autre valeur indique un <C3>
<A9>chec.
*/

#define TEST_SIZE 12
int init_suite1(void) {
  int err __attribute__ ((unused));
  err=_create("test1.dat",TEST_SIZE,3.123);
  err=_create("test2.dat",TEST_SIZE,3.123);
  err=_set("test2.dat",2,1.123);
  err=_create("test3.dat",TEST_SIZE,3.123);
  err=_set("test3.dat",3,1.123);
  err=_create("test4.dat",TEST_SIZE,3.123);
  err=_set("test4.dat",11,1.123);
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  unlink("test1.dat");
  unlink("test2.dat");
  unlink("test3.dat");
  unlink("test4.dat");
  return 0;
}


// @mp:test_vec1 => [fichier swap 1,1 -> pas de modification, read, write, open]
void test_vec1()
{
  int ret;
  global_open=0; 
  global_close=0;
  global_read=0;
  global_write=0;
  ret=swap("test1.dat",1,1);
  CU_ASSERT_EQUAL(ret,0);
  CU_ASSERT_EQUAL( global_open,  global_close);


  for(int i=0;i< TEST_SIZE; i++) {
    double val;
    int err=_get("test1.dat",i,&val);
    CU_ASSERT_DOUBLE_EQUAL(val,3.123,0.001);
    CU_ASSERT_EQUAL(err,0);
  }

}
// @mp:test_vec2 => [fichier swap 7,2 -> si read ou write fail, retourne -1]
void test_vec2()
{
  int ret;
  fail_read=1;
  ret=swap("test1.dat",7,2);
  CU_ASSERT_EQUAL(ret,-1);
  fail_read=0;
  fail_write=1;
  ret=swap("test1.dat",7,2);
  CU_ASSERT_EQUAL(ret,-1);
  fail_write=0;

}

// @mp:test_vec3 => [fichier swap 2,6]
void test_vec3()
{
  double val;
  int err,ret;
  ret=swap("test2.dat",2,6);
  CU_ASSERT_EQUAL(ret,0);
  err=_get("test2.dat",2,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,3.123,0.001);
  err=_get("test2.dat",6,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,1.123,0.001);

}

// @mp:test_vec4 => [fichier swap 9,3]
void test_vec4()
{

  double val;
  int err,ret;
  ret=swap("test3.dat",9,3);
  CU_ASSERT_EQUAL(ret,0);
  err=_get("test3.dat",3,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,3.123,0.001);
  err=_get("test3.dat",9,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,1.123,0.001);

}

// @mp:test_vec5 => [fichier swap 0,11]
void test_vec5()
{

  double val;
  int err,ret;
  ret=swap("test4.dat",0,11);
  CU_ASSERT_EQUAL(ret,0);
  err=_get("test4.dat",11,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,3.123,0.001);
  err=_get("test4.dat",0,&val);
  CU_ASSERT_EQUAL(err,0);
  CU_ASSERT_DOUBLE_EQUAL(val,1.123,0.001);

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

    if(NULL==CU_add_test(suiteM, "test_vec1", test_vec1) ||
       NULL==CU_add_test(suiteM, "test_vec2", test_vec2) || 
       NULL==CU_add_test(suiteM, "test_vec3", test_vec3) || 
       NULL==CU_add_test(suiteM, "test_vec4", test_vec4) || 
       NULL==CU_add_test(suiteM, "test_vec5", test_vec5) ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
