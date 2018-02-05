/* Tests unitaires pour l'examen juin 2016b - mineures */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>


#include "student_code.h"


// @mp:mTest1 => [0,0 == 0]
void mTest1()
{
  int result = mp(0,0);
  CU_ASSERT_EQUAL(result, 0);
}

// @mp:mTest2 => [2,4 == 4]
void mTest2()
{
  int result = mp(2,4);
  CU_ASSERT_EQUAL(result, 4);
}


// @mp:mTest3 => [3,5 == 5]
void mTest3()
{
  int result = mp(3,5);
  CU_ASSERT_EQUAL(result, 5);
}

// @mp:mTest4 => [1,0 == 0]
void mTest4()
{
  int result = mp(1,0);
  CU_ASSERT_EQUAL(result, 0);
}

// @mp:mTest5 => [0,1 == 0]
void mTest5()
{
  int result = mp(0,1);
  CU_ASSERT_EQUAL(result, 0);
}

// @mp:mTest6 => [-1,-1 == -2]
void mTest6()
{
  int result = mp(-1,-1);
  CU_ASSERT_EQUAL(result, -2);
}

// @mp:mTest7 => [1,3 == 2]
void mTest7()
{
  int result = mp(1,3);
  CU_ASSERT_EQUAL(result, 2);
}

int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  montest();


    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}

    CU_pSuite suiteM = CU_add_suite("Tests de la traduction de la fonction assembleur",NULL,NULL);
    if(NULL== suiteM) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if(NULL==CU_add_test(suiteM, "mTest1", mTest1) ||
       NULL==CU_add_test(suiteM, "mTest2", mTest2) || 
       NULL==CU_add_test(suiteM, "mTest3", mTest3) ||
       NULL==CU_add_test(suiteM, "mTest4", mTest4) ||
       NULL==CU_add_test(suiteM, "mTest5", mTest5) ||
       NULL==CU_add_test(suiteM, "mTest6", mTest6) ||
       NULL==CU_add_test(suiteM, "mTest7", mTest7) ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();


}
