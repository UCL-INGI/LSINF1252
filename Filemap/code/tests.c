/* Tests unitaires pour l'exercice count
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
}


// @count:test_count1 => [fichier contenant trois a]
void test_count1()
{
  int result = count("f1.dat",'a');
  CU_ASSERT_EQUAL(result, 3);
}


// @count:test_count2 => [fichier vide]
void test_count2()
{
  int result = count("f0.dat",'a');
  CU_ASSERT_EQUAL(result, 0);
}

// @count:test_count3 => [fichier absent]
void test_count3()
{
  int result = count("nofile.dat",'a');
  CU_ASSERT_EQUAL(result, -1);
}


int main (int argc, char** argv)
{
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}

    CU_pSuite suiteM = CU_add_suite("Tests de count",NULL,NULL);
    if(NULL== suiteM) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "test_count1", test_count1) ||
       NULL==CU_add_test(suiteM, "test_count2", test_count2) || 
       NULL==CU_add_test(suiteM, "test_count3", test_count3)) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}
