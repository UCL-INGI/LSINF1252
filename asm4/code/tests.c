/* Tests unitaires pour l'examen asm - sept 2016b */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<CUnit/CUnit.h>
#include<CUnit/Automated.h>


#include "student_code.h"

/* extern int _f(int a, int b, int c); */

/* /\*f(int a, int b, int c) *\/ */
/* __asm__( */
/* "_f:\n" */
/* "   subl $8, %esp\n" */
/* "   movl 20(%esp), %edx\n" */
/* "   movl 16(%esp), %ecx\n" */
/* "   movl 12(%esp), %ebx\n" */
/* "   addl %ebx,%ecx\n" */
/* "   cmpl %edx,%ecx\n" */
/* "   jle m31\n" */
/* "   movl %edx, %eax\n" */
/* "   addl $8, %esp\n" */
/* "   ret\n" */
/* "m31: movl %ecx, %eax\n" */
/* "   addl $8, %esp\n" */
/* "   ret\n" */
/* 	); */
/* // a retirer */
/* int f(int a, int b, int c) { */
/*   return _f(a,b,c); */

/* } */

int init_suite1(void) { 
  return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
 * Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
  return 0;
}

// @asm:mTest1 => [1,2,4 == 3]
void mTest1()
{
  
  int result = f(1,2,4);
  CU_ASSERT_EQUAL(result, 3);
}

// @asm:mTest2 => [4,2,3 == 3]
void mTest2()
{
  int result = f(4,2,3);

 CU_ASSERT_EQUAL(result, 3);
}


// @asm:mTest3 => [6,4,3 == 3]
void mTest3()
{
  int result = f(6,4,3);
  
  CU_ASSERT_EQUAL(result, 3);
}

// @asm:mTest4 => [1,1,6 == 2]
void mTest4()
{
  int result = f(1,1,6);
 
 CU_ASSERT_EQUAL(result, 2);

}


int main (int argc, char** argv)
{
  // on commence par la fonction de l'etudiant
  //montest();
  //printf("_f(1,2,4)=%d\n",_f(1,2,4)); // 3
  //printf("_f(4,2,3)=%d\n",_f(4,2,3)); // 3
  //printf("_f(6,4,3)=%d\n",_f(6,4,3)); // 3
  //printf("_f(1,1,6)=%d\n",_f(1,1,6)); // 2

  

    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}

    CU_pSuite suiteM = CU_add_suite("Tests de la traduction de la fonction assembleur",init_suite1,clean_suite1);
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
