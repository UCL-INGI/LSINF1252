/* Tests unitaires pour le mini-projet-string
Copyright (C) 2016 Mathieu Xhonneux

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
//#include <CUnit/Basic.h>

#include "student_code.h"

char *globStr = NULL;

/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {
    //On cherche à allouer un bloc mémoire qui sera utilisé dans nos tests
    globStr = malloc(sizeof(char)*20);
    if (globStr == NULL)
        return -1;

    return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
    free(globStr); //on libère la mémoire allouée lors de l'initialisation de la suite
    return 0;
}


// @strlen:test_strlen1 => [Lorsque strlen est appelé avec comme argument la chaine \"foo bar\", strlen ne renvoie pas le bon nombre de caractères.]
void test_strlen1(void) {
    char *testString = "foo bar";

    //un test unitaire peut contenir plusieurs assertions
    //ici on s'assure que la fonction strlen renvoie bien le bon nombre de caractères présents dans testString
    CU_ASSERT_EQUAL(strlen(testString), 7); 
}

// @strlen:test_strlen2 => [strlen ne renvoie pas 0 lorsque la chaîne est vide.]
void test_strlen2(void) {
    char *testString = "";
    size_t r = strlen(testString);
    CU_ASSERT_EQUAL(r, 0);
}

// @strcat:test_strcat1 => [strcat ne concatène pas correctement les chaînes "foo" et "bar"]
void test_strcat1(void) {
    char str1[80]; //allocation sur la stack
    strcpy(str1, "foo");
    char *str2 = "bar";

    strcat(str1, str2);
    CU_ASSERT_STRING_EQUAL(str1, "foobar"); //pour comparer 2 chaines de caractères, utiliser cette fonction
}

// @strcat:test_strcat2 => [strcat ne concatène pas correctement la chaîne "foo" et la chaîne vide.]
void test_strcat2(void) {
    char *str1 = malloc(sizeof(char) * 80); //allocation sur la heap
    if (str1 == NULL) { //si malloc n'a pas pu allouer la mémoire, on le dit à CUnit
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_strcat2.");
        return;
    }

    strcpy(str1, "foo");
    char *str2 = "";

    strcat(str1, str2);
    CU_ASSERT_STRING_EQUAL(str1, "foo");
    free(str1);
}

// @strcat:test_strcat3 => [strcat ne renvoie pas le pointeur vers la chaîne modifiée.]
void test_strcat3(void) {
    char str1[80];
    strcpy(str1, "foo");
    char *str2 = "bar";

    char *copy = strcat(str1, str2);
    CU_ASSERT_PTR_EQUAL(str1, copy); //fonction pour comparer des pointeurs
}

// @strcasecmp:test_strcasecmp1 => [strcasecmp ne renvoie pas 0 lorsqu'on lui passe deux chaînes identiques.]
void test_strcasecmp1(void) {
    char *str1 = "foo";
    strcpy(globStr, "foo"); //on utilise ici la string globale allouée lors de l'initialisation

    CU_ASSERT_EQUAL(strcasecmp(str1, globStr), 0);
}

// @strcasecmp:test_strcasecmp2 => [strcasecmp ne renvoie pas un entier positif pour l'appel strcasecmp("foo", "bar").]
void test_strcasecmp2(void) {
    char *str1 = "foo";
    char *str2 = "bar";

    CU_ASSERT_TRUE(strcasecmp(str1, str2) > 0);
}

// @strcasecmp:test_strcasecmp3 => [strcasecmp considère que les chaînes "FooBar" et "foobar" sont différentes. La fonction doit être insensible à la casse.]
void test_strcasecmp3(void) {
    char *str1 = "FooBar";
    char *str2 = "foobar";

    CU_ASSERT_EQUAL(strcasecmp(str1, str2), 0);
}

jmp_buf label_test_strcasecmp4;

void sig_handler(int signo) {
    //Le signal handler a été exécuté, on a donc reçu un SIGSEGV
    //On provoque alors un jump vers le label avec le paramètre 1
    siglongjmp(label_test_strcasecmp4,1);
}

// Dans ce test, on va s'assurer que l'utilisateur n'accède pas à la mémoire située après la fin de la chaine de caractères
// @strcasecmp:test_strcasecmp4 => [strcasecmp accède à une adresse mémoire à droite de la zone mémoire de la chaine de caractères passée en argument.]
void test_strcasecmp4(void) {
    char *str1 = "sinf1252";
    char *str2;
    
    //On cherche à allouer 2 pages de la mémoire, la première avec le droit d'écriture et de lecture
    void *ptr = mmap(NULL, getpagesize()*2, PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_strcasecmp4.");
        return;
    }

    // On protège ensuite la deuxième page mémoire en enlevant les droits de lecture et écriture
    mprotect(ptr+getpagesize(), getpagesize(), PROT_NONE);

    // On écrit à la fin de la première page mémoire la chaine "sinf\0"
    str2 = (char*) ptr+getpagesize()-5;
    strcpy(str2, "sinf");

    /* Si le code de l'utilisateur accède à de la mémoire située après le caractère de fin \0,
     * autrement dit la mémoire protégée de la seconde page, un segfault sera envoyé.
     * La mécanique utilisée ici permet d'"attraper" un segfault sans que tout le programme ne plante */

    //On enregistre un signal handler. Cette fonction sera exécutée par le programme lorsque
    //le code produira une segmentation fault (ce qui lance le signal SIGSEGV).
    if (signal(SIGSEGV, sig_handler) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;
    }

    //On définit ici un jump avec le label label_test_strcasecmp4 qui attend le paramètre 0 (par défaut)
    if(setjmp(label_test_strcasecmp4)==0) {
        CU_ASSERT_TRUE(strcasecmp(str1, str2) != 0);
    }
    else { //On a reçu un autre paramètre que 0, autrement dit le code a exécuté sig_handler
        //On a donc intercepté une segmentation fault, donc le code de l'utilisateur est fautif.
        CU_FAIL("Un signal SIGSEGV a été provoqué par le code de l'utilisateur.");
    }
        
    //On enlève le signal handler précédemment assigné à SIGSEGV
    signal(SIGSEGV, SIG_DFL);

    //On libère la mémoire précédemment allouée
    munmap(ptr, getpagesize()*2);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour le mini-projet", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test_strlen1", test_strlen1) ||
       NULL == CU_add_test(pSuite, "test_strlen2", test_strlen2) ||
       NULL == CU_add_test(pSuite, "test_strcat1", test_strcat1) ||
       NULL == CU_add_test(pSuite, "test_strcat2", test_strcat2) ||
       NULL == CU_add_test(pSuite, "test_strcat3", test_strcat3) ||
       NULL == CU_add_test(pSuite, "test_strcasecmp1", test_strcasecmp1) ||
       NULL == CU_add_test(pSuite, "test_strcasecmp2", test_strcasecmp2) ||
       NULL == CU_add_test(pSuite, "test_strcasecmp3", test_strcasecmp3) ||
       NULL == CU_add_test(pSuite, "test_strcasecmp4", test_strcasecmp4)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
