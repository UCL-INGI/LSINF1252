/* Tests unitaires pour l'examen juin 2016 - majeures */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>
#include "lecture.h"

#include "student_code.h"


// @asmfct:mTest1 => [-2 == -1]
void mTest1()
{
  int result = fct(-2);
  CU_ASSERT_EQUAL(result, -1);
}

// @asmfct:mTest2 => [0 == -1]
void mTest2()
{
  int result = fct(0);
  CU_ASSERT_EQUAL(result, -1);
}


// @asmfct:mTest3 => [1 == 2]
void mTest3()
{
  int result = fct(1);
  CU_ASSERT_EQUAL(result, 2);
}

// @asmfct:mTest4 => [3 == 12]
void mTest4()
{
  int result = fct(3);
  CU_ASSERT_EQUAL(result, 12);
}

// @asmfct:mTest5 => [4 == 20]
void mTest5()
{
  int result = fct(4);
  CU_ASSERT_EQUAL(result, 20);
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
       NULL==CU_add_test(suiteM, "mTest4", mTest4) ) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();


}
