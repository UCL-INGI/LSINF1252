/* Tests unitaires pour l'exercice rpn-calc
Copyright (C) 2016 Margerie Huet

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

void push(double value);
double pop();
double rpn(char *expr);
extern int stack_height;
extern double stack[10];

/* Check if the size of the stack is well incremented when push is called*/
// @push:pushTest1 => [Lorsque push est appelé, la taille du stack n'est pas incrémentée correctement.]
void pushTest1()
{
    stack_height=0;
    push(3.5);
    CU_ASSERT_EQUAL(stack_height, 1);
}

/* Check if push puts the good value on top of the stack*/
// @push:pushTest2 => [Lorsque push est appelé, la valeur passée en paramètre n'est pas placée en haut de la stack.]
void pushTest2()
{
    stack_height=0;
    push(3.5);
    CU_ASSERT_EQUAL(stack[0], 3.5);
}

/* Check if the size of the stack is well decremented when pop is called*/
//@pop:popTest1 => [Lorsque pop est appelé, la taille de la stack n'est pas décrementée correctement.]
void popTest1()
{
    stack_height=1;
    pop();
    CU_ASSERT_EQUAL(stack_height, 0);
}

/* Check if pop return the good value*/
// @pop:popTest2 => [La valeur retournée par pop n'est pas correcte.]
void popTest2()
{
    stack_height=1;
    stack[0]=2.5;
    double res=pop();
    CU_ASSERT_EQUAL(res, 2.5);
}

/* Check a simple addition*/
// @rpn:simpleAdd => [rpn ne retourne pas le bon résultat pour l'entrée : 2 3 +]
void simpleAdd()
{
    char str[6] = "2 3 +";
    CU_ASSERT_EQUAL(rpn(str), 5);
}

/* Check a simple soustraction*/
// @rpn:simpleDiff => [rpn ne retourne pas le bon résultat pour l'entrée : 5 3 -]
void simpleDiff()
{
    char str[6] = "5 3 -";
    CU_ASSERT_EQUAL(rpn(str), 2);
}

/* Check a simple multiplication*/
// @rpn:simpleMult => [rpn ne retourne pas le bon résultat pour l'entrée: 5 3 *]
void simpleMult()
{
    char str[6] = "5 3 *";
    CU_ASSERT_EQUAL(rpn(str), 15);
}

/* Check a simple division*/
// @rpn:simpleDiv => [rpn ne retourne pas le bon résultat pour l'entrée: 5 3 /]
void simpleDiv()
{
    char str[7] = "15 3 /";
    CU_ASSERT_EQUAL(rpn(str), 5);
}

/* Check a simple division*/
// @rpn:addDiv => [rpn ne retourne pas le bon résultat pour l'entrée: 2 3 + 2 /]
void addDiv()
{
    char str[10] = "2 3 + 2 /";
    CU_ASSERT_EQUAL(rpn(str), 2.5);
}

/* Check a simple division*/
// @rpn:divMinus => [rpn ne retourne pas le bon résultat pour l'entrée: 6 3 / 1 -]
void divMinus()
{
    char str[10] = "6 3 / 1 -";
    CU_ASSERT_EQUAL(rpn(str), 1);
}

/* Check a simple division*/
// @rpn:minusTimes => [rpn ne retourne pas le bon résultat pour l'entrée: 2 3 - 2 *]
void minusTimes()
{
    char str[10] = "2 3 - 2 *";
    CU_ASSERT_EQUAL(rpn(str), -2);
}

/* Check a simple division*/
// @rpn:timesAdd => [rpn ne retourne pas le bon résultat pour l'entrée: 2 3 * 2 +]
void timesAdd()
{
    char str[10] = "2 3 * 2 +";
    CU_ASSERT_EQUAL(rpn(str), 8);
}

int main (int argc, char** argv)
{
    if(CUE_SUCCESS != CU_initialize_registry())
        {return CU_get_error();}

    CU_pSuite suitePushPop = CU_add_suite("Tests de Push et Pop",NULL,NULL);
    if(NULL== suitePushPop) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suitePushPop, "pushTest1", pushTest1) ||
       NULL==CU_add_test(suitePushPop, "pushTest2", pushTest2) || 
       NULL==CU_add_test(suitePushPop, "popTest1", popTest1) ||
       NULL==CU_add_test(suitePushPop, "popTest2", popTest2)) {
    
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite suiteSimple = CU_add_suite("Simple Tests",NULL,NULL);

    if(NULL== suiteSimple) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteSimple, "simpleAdd", simpleAdd) ||
       NULL==CU_add_test(suiteSimple, "simpleDiff", simpleDiff) ||
       NULL==CU_add_test(suiteSimple, "simpleMult", simpleMult) ||
       NULL==CU_add_test(suiteSimple, "simpleDiv", simpleDiv)) {
       
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite suiteDouble = CU_add_suite("Double Tests",NULL,NULL);

    if(NULL== suiteDouble) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if(NULL==CU_add_test(suiteDouble, "addDiv", addDiv) ||
       NULL==CU_add_test(suiteDouble, "divMinus", divMinus) ||
       NULL==CU_add_test(suiteDouble, "minusTimes", minusTimes) ||
       NULL==CU_add_test(suiteDouble, "timesAdd", timesAdd)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_automated_run_tests();
}
