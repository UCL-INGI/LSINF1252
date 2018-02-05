#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>

#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "student_code.h"

int malloc_fail = 0;
int free_count = 0;

// @build_index:test_build_index_1 => [L'index est mal construit pour le corpus "lorem".]
void test_build_index_1(void)
{
	char s[6]="lorem";
	char *str=s;
	
    Entry *first = build_index(str);
	
    CU_ASSERT_PTR_NOT_NULL(first);
    if(first != NULL) {
        CU_ASSERT_STRING_EQUAL(first->word,"lorem");
        CU_ASSERT_EQUAL(first->count,1);
        CU_ASSERT_PTR_NULL(first->next);


        free(first);
    }
	
}

// @build_index:test_build_index_2 => [Dans un corpus avec deux mots différents, la variable next du premier indexEntry ne pointe pas vers le deuxième mot.]
void test_build_index_2(void)
{
	char s[12]="lorem ipsum";
	char *str=s;
	
    Entry *first = build_index(str);
	CU_ASSERT_PTR_NOT_NULL(first);
    if (first == NULL)
        return;
	
	Entry *second = first->next;
    free(first);

	CU_ASSERT_PTR_NOT_NULL(second);
    if (second != NULL) {
        CU_ASSERT_STRING_EQUAL(second->word,"ipsum");
        free(second);
    }
}

// @build_index:test_build_index_3 => [La variable count d'un mot présent cinq fois ne vaut pas 5.]
void test_build_index_3(void)
{
	char s[50]="lorem lorem lorem lorem lorem";
	char *str=s;
	
    Entry *first = build_index(str);
	
	CU_ASSERT_PTR_NOT_NULL(first);
    if (first != NULL) {
        CU_ASSERT_EQUAL(first->count,5);
        free(first);
    }
}

jmp_buf label_test_build_index_4;

void sig_handler(int signo)
{
    //Le signal handler a été exécuté, on a donc reçu un SIGSEGV
    //On provoque alors un jump vers le label avec le paramètre 1
    longjmp(label_test_build_index_4,1);
}

// @build_index:test_build_index_4 => [La fonction build_index ne prend pas en compte le fait que malloc puisse échouer (càd renvoyer NULL).]
void test_build_index_4(void) {
	malloc_fail = 1; //On indique qu'on veut que malloc utilisé par build_index échoue
	char s[12]="lorem ipsum";
	char *str=s;

    if(signal(SIGSEGV, sig_handler) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;
    }
	
    if(setjmp(label_test_build_index_4)==0) {
        build_index(str);
    }
    else
	{
        // On remet malloc_fail à 0 pour que CUnit puisse réutiliser malloc par la suite.
        malloc_fail = 0; 
        CU_ASSERT_TRUE(0);
    }
    
    // On remet malloc_fail à 0 pour que CUnit puisse réutiliser malloc par la suite.
    malloc_fail = 0;
    signal(SIGSEGV, SIG_DFL);
}

jmp_buf label_test_build_index_5;

void sig_handler2(int signo)
{
    //Le signal handler a été exécuté, on a donc reçu un SIGSEGV
    //On provoque alors un jump vers le label avec le paramètre 1
    longjmp(label_test_build_index_5,1);
}

// @build_index:test_buid_index_5 => [build_index accède à une adresse mémoire à droite de la zone mémoire de la chaine de caractères passée en argument.]
void test_build_index_5(void)
{
    char *str;
    
    //On cherche à allouer 2 pages de la mémoire, la première avec le droit d'écriture et de lecture
    void *ptr = mmap(NULL, getpagesize()*2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_build_index_5.");
        return;
    }

    // On protège ensuite la deuxième page mémoire en enlevant les droits de lecture et écriture
    mprotect(ptr+getpagesize(), getpagesize(), PROT_NONE);

    // On écrit à la fin de la première page mémoire la chaine "lorem\0"
    str = (char*) ptr+getpagesize()-7;
    strcpy(str, "lorem");

    /* Si le code de l'utilisateur accède à de la mémoire située après le caractère de fin \0,
     * autrement dit la mémoire protégée de la seconde page, un segfault sera envoyé.
     * La mécanique utilisée ici permet d'"attraper" un segfault sans que tout le programme ne plante */

    //On enregistre un signal handler. Cette fonction sera exécutée par le programme lorsque
    //le code produira une segmentation fault (ce qui lance le signal SIGSEGV).
    if (signal(SIGSEGV, sig_handler2) == SIG_ERR) {
        CU_FAIL("Impossible d'enregistrer un signal handler.");
        return;
    }
    
    //On définit ici un jump avec le label label_test_build_index_5 qui attend le paramètre 0 (par défaut)
    if(setjmp(label_test_build_index_5)==0) {
		build_index(str);
    }
    else{ //On a reçu un autre paramètre que 0, autrement dit le code a exécuté sig_handler
        //On a donc intercepté une segmentation fault, donc le code de l'utilisateur est fautif.
        CU_ASSERT_TRUE(0);
    }
        
    //On enlève le signal handler précédemment assigné à SIGSEGV
    signal(SIGSEGV, SIG_DFL);

    //On libère la mémoire précédemment allouée
    munmap(ptr, getpagesize()*2);
}

// @filter_index:test_filter_index_1 => [filter_index ne supprime pas les bons éléments de l'index.]
void test_filter_index_1(void)
{	
	Entry *first=(Entry*) malloc(sizeof(Entry));
	Entry *second=(Entry*) malloc(sizeof(Entry));
	Entry *third=(Entry*) malloc(sizeof(Entry));

    if (first == NULL || second == NULL || third == NULL) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_filter_index_1.");
        return;
    }

    *first = (Entry){"lorem", 2, second};
	*second = (Entry){"ipsum", 2, third};
	*third = (Entry){"dolor", 1, NULL};
	
	filter_index(&first,2);
	
	CU_ASSERT_STRING_EQUAL(first->word,"lorem");
	CU_ASSERT_STRING_EQUAL(second->word,"ipsum");
	CU_ASSERT_PTR_NULL(second->next);
	
	free(first);
	free(second);
}

// @filter_index:test_filter_index_2 => [filter_index ne libère pas la mémoire des éléments qu'elle supprime.]
void test_filter_index_2(void)
{
	Entry *ent1 = (Entry*) malloc(sizeof(Entry));
	Entry *ent2 = (Entry*) malloc(sizeof(Entry));
	Entry *ent3 = (Entry*) malloc(sizeof(Entry));
	Entry *ent4 = (Entry*) malloc(sizeof(Entry));
	Entry *ent5 = (Entry*) malloc(sizeof(Entry));

    if (ent1 == NULL || ent2 == NULL || ent3 == NULL || ent4 == NULL || ent5 == NULL) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_filter_index_2.");
        return;
    }

    *ent1 = (Entry){"lorem", 2, ent2};
	*ent2 = (Entry){"ipsum", 2, ent3};
	*ent3 = (Entry){"dolor", 1, ent4};
	*ent4 = (Entry){"sit", 1, ent5};
	*ent5 = (Entry){"amet", 1, NULL};
	
	free_count = 0;
	filter_index(&ent1,2);
	
	CU_ASSERT_EQUAL(free_count,3);
	free(ent1);
	free(ent2);
}

// @filter_index:test_filter_index_3 => [filter_index ne met pas `index_head` à NULL lorsque toute la liste est supprimée.]
void test_filter_index_3(void) {
	Entry *ent1 = (Entry*) malloc(sizeof(Entry));
	Entry *ent2 = (Entry*) malloc(sizeof(Entry));
	Entry *ent3 = (Entry*) malloc(sizeof(Entry));
	Entry *ent4 = (Entry*) malloc(sizeof(Entry));
	Entry *ent5 = (Entry*) malloc(sizeof(Entry));

    if (ent1 == NULL || ent2 == NULL || ent3 == NULL || ent4 == NULL || ent5 == NULL) {
        CU_FAIL("La mémoire n'a pas pu être allouée pour le test test_filter_index_3.");
        return;
    }


    *ent1 = (Entry){"lorem", 2, ent2};
	*ent2 = (Entry){"ipsum", 2, ent3};
	*ent3 = (Entry){"dolor", 1, ent4};
	*ent4 = (Entry){"sit", 1, ent5};
	*ent5 = (Entry){"amet", 1, NULL};
	
	filter_index(&ent1, 5);

    CU_ASSERT_PTR_NULL(ent1);
	
}

int main() {
    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Tests pour build_index et filter_index", NULL, NULL);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test_build_index_1", test_build_index_1) ||
	   NULL == CU_add_test(pSuite, "test_build_index_2", test_build_index_2) ||
	   NULL == CU_add_test(pSuite, "test_build_index_3", test_build_index_3) ||
	   NULL == CU_add_test(pSuite, "test_build_index_4", test_build_index_4) ||
	   NULL == CU_add_test(pSuite, "test_build_index_5", test_build_index_5) ||
	   NULL == CU_add_test(pSuite, "test_filter_index_1", test_filter_index_1) ||
	   NULL == CU_add_test(pSuite, "test_filter_index_2", test_filter_index_2) ||
	   NULL == CU_add_test(pSuite, "test_filter_index_3", test_filter_index_3))

	{
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_automated_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
