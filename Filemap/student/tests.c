#include<stdio.h>
#include<stdlib.h>
#include "CTester/CTester.h"

#include "student_code.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


/*
 * Test avec un fichier normal
 */
void test_count_1(){

    set_test_metadata("count", _("Test 1"), 1);

	int ret = -2;

	char* filename = "f1.dat";
	char the_char = 'a';

	SANDBOX_BEGIN;
	ret = count(filename, the_char);
	SANDBOX_END;

	CU_ASSERT_EQUAL(ret, 3);
	if(ret != 3){
		push_info_msg(_("Votre fonction ne renvoit pas le nombre correct d'occurences"));
	}
}

/*
 * Test avec un fichier vide
 */
void test_count_2(){
	
	set_test_metadata("count", _("Test 2"), 1);

	int ret = -2;

	char* filename = "f0.dat";
	char the_char = 'a';

	SANDBOX_BEGIN;
	ret = count(filename, the_char);
	SANDBOX_END;

	CU_ASSERT_EQUAL(ret, 0);
	if(ret != 0){
		push_info_msg(_("Votre fonction fonctionne pas correctement avec un fichier vide"));
	}
}

/*
 * Test avec un fichier inexistant;
 */
void test_count_3(){

    set_test_metadata("count", _("Test 3"), 1);

    int ret = -2;

    char* filename = "nofile";
    char the_char = 'a';

    SANDBOX_BEGIN;
    ret = count(filename, the_char);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret, -1);
    if(ret != -1){
    	push_info_msg("Votre fonction ne fonctionne pas pour des fichiers inexistants");
    }
}

int main(int argc, char* argv[]){
	BAN_FUNCS(pread, read, readv);
	RUN(test_count_1, test_count_2, test_count_3);
}