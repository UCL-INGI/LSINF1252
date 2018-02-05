/* Tests unitaires pour insertion-sort
 *
Copyright (C) 2016 Henri Devillez
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
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>


#include "student_code.h"

int setup(void) 
{

    return 0;
}

int teardown(void) 
{
    
    return 0;   
}


// @insertionSort:testTriBasic => [Votre fonction n'arrive pas à trier un simple tableau de 10 éléments.]
void testTriBasic(void) 
{

    int tab[] = {1, 4, 3, 5, 9, 0, 7, 2, 6, 8};
    int tabSolution[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int len = 10;

    tri(tab, len);
     
    int i;  
    for(i= 0; i < len; i++) {
        CU_ASSERT_EQUAL(tab[i], tabSolution[i]);    
    }
    
}

// @tri:testTabInverse => [Votre fonction n'arrive pas à trier un tableau trié dans l'autre sens.]
void testTabInverse(void) 
{


    int tab[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int tabSolution[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int len = 10;

    tri(tab, len);
     
    int i;  
    for(i= 0; i < len; i++) {
        CU_ASSERT_EQUAL(tab[i], tabSolution[i]);    
    }
    
}

// @tri:testWithDuplication => [Votre fonction n'arrive pas à trier un tableau où un même élément est présent plusieurs fois.]
void testWithDuplication(void) 
{
 
    int tab[] = {4, 2, 1, 8, 4, 2};
    int tabSolution[] = {1, 2, 2, 4, 4, 8};
    int len = 6;

    tri(tab, len);
     
    int i;  
    for(i= 0; i < len; i++) {
        CU_ASSERT_EQUAL(tab[i], tabSolution[i]);    
    }
 
}

// @tri:testWithOneElement => [Votre fonction n'arrive pas à trier un tableau d'un élément.]
void testWithOneElement(void) 
{
    int tab[] = {1};
    int tabSolution[] = {1};
    int len = 1;

    tri(tab, len);
     
    CU_ASSERT_EQUAL(tab[0], tabSolution[0]);    
}

jmp_buf labelTestOverflow;

void sig_handler(int signo) {
    
    siglongjmp(labelTestOverflow, 1);    
}

// @tri:testOverflow => [Votre fonction accède à une zone mémoire à droite du tableau.]
void testOverflow(void) {
    
    int *tab;
    int tabSolution[4] = {0,1,2,3};
    int len = 4;

    void *ptr = mmap(NULL, getpagesize()*2, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    if(ptr == MAP_FAILED) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test testOverflow.");
        return ;    
    }

    mprotect(ptr+getpagesize(), getpagesize(), PROT_NONE);
    
    tab = (int *) (ptr+getpagesize()-4*sizeof(int));
   
    int i;
    for(i = 0; i < len; i++) {
        *(tab + i) = (i+2)%4;    
    }

    if(signal(SIGSEGV, sig_handler) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;    
    }

    
    if(setjmp(labelTestOverflow) == 0) {

        tri(tab, len);
        
        for(i = 0; i < len; i++) {
            CU_ASSERT_EQUAL(tab[i], tabSolution[i]);    
        }    
    }
    else {
        CU_ASSERT_TRUE(0); // Segmentation fault intercepted  
    }

    signal(SIGSEGV, SIG_DFL);
    
    munmap(ptr, getpagesize()*2); 
    
}

int main(int argc, char * argv[]) 
{
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();    

    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("mySuite", setup, teardown);
    
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error(); 
    }
       
    if(NULL == CU_add_test(pSuite, "testTriBasic", testTriBasic) ||
       NULL == CU_add_test(pSuite, "testTabInverse", testTabInverse) ||
       NULL == CU_add_test(pSuite, "testWithDuplication", testWithDuplication) ||
       NULL == CU_add_test(pSuite, "testOverflow", testOverflow) ||
       NULL == CU_add_test(pSuite, "testWithOneElement", testWithOneElement)) {
        
        CU_cleanup_registry();
        return CU_get_error();    
    }

    CU_automated_run_tests();
    CU_cleanup_registry();
    return 0;
}
