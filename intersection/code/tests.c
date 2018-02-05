/* Tests unitaires pour l'exercice Intersection
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
#include <sys/mman.h>


int global_open=0; // counter pour appel open
int global_close=0; // counter pour appel close
int global_read=0;
int global_write=0;
int global_mmap=0; // counter pour mmap
int global_munmap=0;
int fail_read=0;
int fail_write=0;
int fail_open=0;
int fail_mmap=0;


int __real_open(const char *path, int oflag, mode_t mode);
int __real_close(int fd);
ssize_t __real_read(int fildes, void *buf, size_t nbyte);
ssize_t __real_write(int fildes, const void *buf, size_t nbyte);
void *__real_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
int __real_munmap(void *addr, size_t length);
//void*  __real_malloc(size_t size);
//void __real_free(void *ptr);

/* wrapping  functions */
void *__wrap_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset) {
  global_mmap++;
  if(fail_mmap) {
    return (void *) MAP_FAILED ;
  }
  return __real_mmap(addr, length, prot, flags, fd, offset);
}

int __wrap_munmap(void *addr, size_t length) {
  global_munmap++;
  return __real_munmap(addr,length);

}
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

  if(fail_read)
    return -1;

  return __real_read(fildes,buf,nbyte);
}

ssize_t __wrap_write(int fildes, void *buf, size_t nbyte) {
  global_write+=nbyte;
  if (fail_write)
    return -1;

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
         * @post construit un fichier contenant size nombre complexes contenant les
         *       valeurs 1+i, 1+2i, 1+3i, ... 1+size*i 
         *       retourne size en cas de succès et -1 en cas d'erreur
         */
        int _create(char *filename, int size) {

          int err;
          int fd=open(filename,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
          if(fd==-1) {
            return -1;
          }
          for(int i=1; i<=size; i++) {
            complex val;
            val.r=1;
            val.i=i;
            err=write(fd,(void *) &val, sizeof(complex));
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
int _set(char *filename, int index, complex * val) {
   int err;
   complex t;
   int fd=open(filename,O_RDWR);
   if(fd==-1) {
     return -1;
   }
   for(int i=0; i<index; i++) {
     err=read(fd,(void *) &t, sizeof(complex));
     if(err<0) {
       err=close(fd);
       return(-1);
     }
   }
   err=write(fd,val, sizeof(complex));
   err=close(fd);
   if(err==-1)
     return err;
   else
     return 0;

 }
// non fourni
int _get(char *filename, int index, complex * val) {
   int err;
   complex t;
   int fd=open(filename,O_RDONLY);
   if(fd==-1) {
     return -1;
   }
   for(int i=0; i<index; i++) {
     err=read(fd,(void *) &t, sizeof(complex));
     if(err<0) {
       err=close(fd);
       return(-1);
     }
   }
   err=read(fd,(void *) val, sizeof(complex));
   err=close(fd);
   if(err==-1)
     return err;
   else
     return 0;

 }


        /*
         * @pre filename1!=NULL, filename2!=NULL
         *      Les deux fichiers contiennent un nombre entier de complexes,
         *      Tous les nombres complexes se trouvant dans un fichier sont
         *      différents.
         *
         * @post retourne le nombre de complexes qui se trouvent à
         *       la fois dans le fichier filename1 et le fichier filename2
         *       -1 si erreur
         *
         * Contrainte: Vous pouvez uniquement utiliser les fonctions open,
         *             read, write, close, mmap et munmap pour accéder aux
         *             fichiers
         */

        int _count_same(char *filename1, char *filename2) {
          int err;
          complex t1;
          int count=0;
          
          int fd1=open(filename1,O_RDONLY);
           if(fd1==-1) {
             return -1;
           }

           while( (err=read(fd1,(void *) &t1, sizeof(complex)))!=0) {
             complex t2;
             if(err==-1) {
               err=close(fd1);
               return -1;
             }
             int fd2=open(filename2,O_RDONLY);
             if(fd2==-1) {
               close(fd2); //test
               return -1;
             }
             while((err=read(fd2,(void *) &t2, sizeof(complex)))!=0) {
               if(err==-1) {
                 err=close(fd2);
                 err=close(fd1);
                 return -1;
               }
               int ret=memcmp(&t1,&t2,sizeof(complex));
               if(ret==0) {
                 count++;
               }
             }
             err=close(fd2);
           }
           err=close(fd1);
           return(count);
        }

// a retirer de la version finale

/*int count_same(char *filename1, char *filename2) {
  return _count_same(filename1,filename2);
}*/
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


// @intersection:test_1 => [fichiers identiques contenant trois complexes]
void test_1()
{
  int ret;
  int err  __attribute__ ((unused));

  err=_create("test1.dat",3);
  err=_create("test2.dat",3);
  global_open=0; 
  global_close=0;
  ret=count_same("test1.dat","test2.dat");
  CU_ASSERT_EQUAL(ret,3);
  CU_ASSERT(global_open>=2);
  CU_ASSERT(global_close>=2);
  CU_ASSERT_EQUAL(global_open,global_close);
  unlink("test1.dat");
  unlink("test2.dat");

}

// @intersection:test_2 => [fichiers différents contenant chacun trois complexes]
void test_2()
{
  int ret;
  int err __attribute__ ((unused));

  err=_create("test1.dat",3);
  err=_create("test2.dat",3);
  complex c1,c2,c3;
  c1.r=7;
  c1.i=8;
  c2.r=7;
  c2.i=9;
  c3.r=3;
  c3.i=8;
  err=_set("test2.dat",0,&c1);
  err=_set("test2.dat",1,&c2);
  err=_set("test2.dat",2,&c3);
  global_open=0; 
  global_close=0;
  global_read=0;
  global_write=0;
  ret=count_same("test1.dat","test2.dat");
  CU_ASSERT_EQUAL(ret,0);
  CU_ASSERT(global_open>=2);
  CU_ASSERT(global_close>=2);
  CU_ASSERT_EQUAL(global_open,global_close);
  unlink("test1.dat");
  unlink("test2.dat");

}


// @intersection:test_3 => [fichiers de taille différentes contenant un complexe commun]
void test_3()
{
  int ret;
  int err __attribute__ ((unused));

  err=_create("test1.dat",3);
  err=_create("test2.dat",4);
  complex c1,c2,c3;
  c1.r=7;
  c1.i=8;
  c2.r=7;
  c2.i=9;
  c3.r=1;
  c3.i=2;
  err=_set("test2.dat",0,&c1);
  err=_set("test2.dat",1,&c2);
  err=_set("test2.dat",2,&c3);
  global_open=0; 
  global_close=0;
  global_read=0;
  global_write=0;
  ret=count_same("test1.dat","test2.dat");
  CU_ASSERT(global_open>=2);
  CU_ASSERT(global_close>=2);
  CU_ASSERT_EQUAL(global_open,global_close);
  CU_ASSERT_EQUAL(ret,1);
  unlink("test1.dat");
  unlink("test2.dat");

}

// @intersection:test_4 => [fichiers de taille différentes contenant deux complexes communs]
void test_4()
{
  int ret;
  int err __attribute__ ((unused));
  err=_create("test1.dat",3);
  err=_create("test2.dat",4);
  complex c1,c2,c3;
  c1.r=7;
  c1.i=8;
  c2.r=1;
  c2.i=1;
  c3.r=1;
  c3.i=2;
  err=_set("test2.dat",0,&c1);
  err=_set("test2.dat",1,&c2);
  err=_set("test2.dat",2,&c3);
  global_open=0; 
  global_close=0;
  global_read=0;
  global_write=0;
  ret=count_same("test1.dat","test2.dat");
  CU_ASSERT_EQUAL(ret,2);
  CU_ASSERT(global_open>=2);
  CU_ASSERT(global_close>=2);
  CU_ASSERT_EQUAL(global_open,global_close);
  unlink("test1.dat");
  unlink("test2.dat");
}

// @intersection:test_5 => [fichiers de taille différentes contenant un complexe commun, avec erreur sur read ou mmap]
void test_5()
{
  int ret;
  int err __attribute__ ((unused));
  err=_create("test1.dat",3);
  err=_create("test2.dat",4);
  complex c1,c2,c3;
  c1.r=7;
  c1.i=8;
  c2.r=7;
  c2.i=9;
  c3.r=1;
  c3.i=2;
  err=_set("test2.dat",0,&c1);
  err=_set("test2.dat",1,&c2);
  err=_set("test2.dat",2,&c3);
  fail_read=1;
  fail_mmap=1;
  global_open=0; 
  global_close=0;
  global_read=0;
  ret=count_same("test1.dat","test2.dat");
  fail_read=0;
  fail_mmap=0;
  CU_ASSERT_EQUAL(ret,-1);
  CU_ASSERT(global_read>0 || global_mmap>0); // un des deux utilisé
  CU_ASSERT(global_open>=1); // au moins un fichier ouvert
  unlink("test1.dat");
  unlink("test2.dat");

}

int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  //montest();
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}


    CU_pSuite suiteM = CU_add_suite("Tests de count_same",init_suite1,clean_suite1);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_1", test_1) ||
       NULL==CU_add_test(suiteM, "test_2", test_2) || 
       NULL==CU_add_test(suiteM, "test_3", test_3) || 
       NULL==CU_add_test(suiteM, "test_4", test_4) || 
       NULL==CU_add_test(suiteM, "test_5", test_5) ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}
