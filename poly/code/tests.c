/* Tests unitaires pour poly
Copyright (C) 2016 Maxime Dimidschstein
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>
#include <sys/mman.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "student_code.h"

// Variables globales utilisees dans les fonctions de tests
poly * intPol;
double intCoeff[]={1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
poly * nulPol;
double nulCoeff[]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
poly * realPol;
double realCoeff[]={42.42, 19.78, 4.1, 0.098, 7.77, 9.1111, 3.33, 13.31, 3.29, 1.2345};
poly * deg2Pol;
double deg2Coeff[]={-1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

double tol = 0.01; // Tolerance entre le resultat exact et le resultat renvoye


int setup(void)
{
  intPol=(poly *) malloc(sizeof(poly));
  nulPol=(poly *) malloc(sizeof(poly));
  realPol=(poly *) malloc(sizeof(poly));
  deg2Pol=(poly *) malloc(sizeof(poly));

  if((intPol==NULL) || (nulPol==NULL) || (realPol==NULL) || (deg2Pol==NULL))
  {
    CU_FAIL("Erreur : impossible d'initialiser les tests.");
    return -1;
  }

  int i;
  for(i=0 ; i<10 ; i++)
  {
    (intPol->coeff)[i]=intCoeff[i];
    (nulPol->coeff)[i]=nulCoeff[i];
    (realPol->coeff)[i]=realCoeff[i];
    (deg2Pol->coeff)[i]=deg2Coeff[i];
  }
  return 0;
}

int teardown(void)
{
  free(intPol);
  free(nulPol);
  free(realPol);
  free(deg2Pol);
  intPol=NULL;
  nulPol=NULL;
  realPol=NULL;
  deg2Pol=NULL;
  return 0;
}

/*********************** Tests eval ***********************/

// @eval:test_eval1 => [Votre fonction eval ne calcule pas correctement la valeur d'un polynome de degré 9.]
void test_eval1(void)
{
  double x = 2.0;
  double solution = 9217.0;
  double answer = eval(intPol,x);

  CU_ASSERT_EQUAL(solution, answer);

}

// @eval:test_eval2 => [Votre fonction eval ne calcule pas correctement la valeur d'un polynome nul.]
void test_eval2(void)
{
  double x = 2.0;
  double solution = 0.0;
  double answer = eval(nulPol,x);

  CU_ASSERT_EQUAL(solution, answer);

}

// @eval:test_eval3 => [Votre fonction eval ne calcule pas correctement la valeur d'un polynome à coefficients non-entiers.]
void test_eval3(void)
{
  double x = 1.742;
  double solution = 1509.8487;
  double answer = eval(realPol,x);

  CU_ASSERT_TRUE(answer < solution+tol && answer > solution-tol);

}

/*********************** Tests derivee ***********************/

// @derivee:test_derivee1 => [Votre fonction derivee ne dérive pas correctement un polynome de degré 9.]
void test_derivee1(void)
{
  double coeffSol[] = {2.0, 6.0, 12.0, 20.0, 30.0, 42.0, 56.0, 72.0, 90.0, 0.0};
  poly answer;
  derivee(intPol, &answer);

  int i;
  for (i=0 ; i<10 ; i++)
  {
    if (coeffSol[i] != (answer.coeff)[i]) {
        CU_ASSERT_EQUAL(coeffSol[i], (answer.coeff)[i]);
        break;
    }
  }
}

// @derivee:test_derivee2 => [Votre fonction derivee ne dérive pas correctement un polynome nul.]
void test_derivee2(void)
{
  poly answer;
  derivee(nulPol, &answer);

  int i;
  for (i=0 ; i<10 ; i++)
  {
    if ((nulPol->coeff)[i] != (answer.coeff)[i]) {
        CU_ASSERT_EQUAL((nulPol->coeff)[i], (answer.coeff)[i]);
        break;
    }
  }
}

// @derivee:test_derivee3 => [Votre fonction derivee ne dérive pas correctement un polynome à coefficients non-entiers.]
void test_derivee3(void)
{
  double coeffSol[] = {19.78, 8.2, 0.294, 31.08, 45.5555, 19.98, 93.17, 26.32, 11.1105, 0.0};

  poly answer;
  derivee(realPol, &answer);

  int i;
  for (i=0 ; i<10 ; i++)
  {

    if (! ((answer.coeff)[i] < coeffSol[i]+tol && (answer.coeff)[i] > coeffSol[i]-tol)) {
        CU_ASSERT_TRUE((answer.coeff)[i] < coeffSol[i]+tol &&
                       (answer.coeff)[i] > coeffSol[i]-tol);
        break;
    }
  }

}

/*********************** Tests racine ***********************/

// @racine:test_racine1 => [Votre fonction racine n'obtient pas le résultat attendu pour un polynome de degré 9.]
void test_racine1(void)
{
  double x = 5.0;
  double solution = -0.7471;
  double answer = racine(intPol,x);

  CU_ASSERT_TRUE(answer < solution+tol && answer > solution-tol);

}

// @racine:test_racine2 => [Votre fonction racine n'obtient pas le résultat attendu pour un polynome nul.]
void test_racine2(void)
{
  double x = 3.5789;
  double answer = racine(nulPol,x);

  CU_ASSERT_EQUAL(answer,x);

}

// @racine:test_racine3 => [Votre fonction racine n'obtient pas le résultat attendu pour un polynome à coefficients non-entiers.]
void test_racine3(void)
{
  double x = 21.42;
  double solution = -1.1401;
  double answer = racine(realPol,x);

  CU_ASSERT_TRUE(answer < solution+tol && answer > solution-tol);

}

// @racine:test_racine4 => [Votre fonction racine ne converge pas vers la solution attendue pour un polynome de degré 2.]
void test_racine4(void)
{
  double x = 15.0;
  double solution = 1.0;
  double answer = racine(deg2Pol,x);

  CU_ASSERT_TRUE(answer < solution+tol && answer > solution-tol);

}

// @racine:test_racine5 => [Votre fonction racine ne converge pas vers la solution attendue pour un polynome de degré 2.]
void test_racine5(void)
{
  double x = -15.0;
  double solution = -1.0;
  double answer = racine(deg2Pol,x);

  CU_ASSERT_TRUE(answer < solution+tol && answer > solution-tol);

}

/*********************** Lancement des tests ***********************/

int main()
{

  // Fonction eval
  CU_pSuite evalSuite = NULL;

  if(CUE_SUCCESS != CU_initialize_registry())
  {
    return CU_get_error();
  }

  evalSuite = CU_add_suite("Suite de tests pour eval", setup, teardown);
  if(NULL == evalSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(NULL == CU_add_test(evalSuite, "test_eval1", test_eval1) ||
     NULL == CU_add_test(evalSuite, "test_eval2", test_eval2) ||
     NULL == CU_add_test(evalSuite, "test_eval3", test_eval3) )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Fonction derivee
  CU_pSuite deriveeSuite = NULL;

  deriveeSuite = CU_add_suite("Suite de tests pour derivee", setup, teardown);
  if(NULL == deriveeSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(NULL == CU_add_test(deriveeSuite, "test_derivee1", test_derivee1) ||
     NULL == CU_add_test(deriveeSuite, "test_derivee2", test_derivee2) ||
     NULL == CU_add_test(deriveeSuite, "test_derivee3", test_derivee3) )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  
  // Fonction racine
  CU_pSuite racineSuite = NULL;

  racineSuite = CU_add_suite("Suite de tests pour racine", setup, teardown);
  if(NULL == racineSuite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(NULL == CU_add_test(racineSuite, "test_racine1", test_racine1) ||
     NULL == CU_add_test(racineSuite, "test_racine2", test_racine2) ||
     NULL == CU_add_test(racineSuite, "test_racine3", test_racine3) ||
     NULL == CU_add_test(racineSuite, "test_racine4", test_racine4) ||
     NULL == CU_add_test(racineSuite, "test_racine5", test_racine5) )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  
  CU_automated_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
