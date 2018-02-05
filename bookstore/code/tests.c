/* Tests unitaires pour gcd
Copyright (C) 2016 Mabille Florent, Wirtgen Thomas

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

#include <stdlib.h>
#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stddef.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#include "student_code.h"

cellAuteur *lib = NULL; //variable globale qui contiendra un liste d'auteurs avec des livres pour tester la fonction existe()
cellAuteur *lib1 = NULL; //variable globale qui contiendra un liste d'auteurs avec des livres pour tester la fonction compteOuvrage()
cellAuteur *lib2 = NULL; //variable globale qui contiendra un liste d'auteurs avec des livres pour tester la fonction add() et supprimer()

int nbUseOfFree = 0; //contiendra le nombre d'appel à free

/**
 *  Fonction d'initialisation. Chacunes vont créer des listes qui
 *  serviront à tester si les différentes focntions à implémenter sont correctes
 */
void setup_lib(void) {
  lib = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  cellLivre *collection2 = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection2b = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3 = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellAuteur *second = (cellAuteur *)(malloc(sizeof(cellAuteur)));
  cellAuteur *third = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  if(collection2 == NULL || collection3 == NULL ||
     lib ==  NULL || second == NULL || third == NULL || collection2b == NULL
    )
    CU_FAIL("La mémoire demandée pour créer une librairie d'auteur n'a pas été allouée");


  collection2->titre = "Two days";
  collection2->suiv = collection2b;

  collection2b->titre = "Beyond";
  collection2b->suiv = NULL;


  collection3->titre = "Three days";
  collection3->suiv = NULL;




  lib->auteur = "florent";
  lib->Livres = NULL;
  lib->next = second;

  second->auteur = "thomas";
  second->Livres = collection2;
  second->next = third;

  third->auteur = "jordan";
  third->Livres = collection3;
  third->next = NULL;

}

void setup_lib1(void)
{
  lib1 = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  cellLivre *collection2 = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection2b = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection2c = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellLivre *collection4 = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellLivre *collection3 = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3b = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3c = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellAuteur *second = (cellAuteur *)(malloc(sizeof(cellAuteur)));
  cellAuteur *third = (cellAuteur *)(malloc(sizeof(cellAuteur)));
  cellAuteur *fourth = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  if(collection2 == NULL || collection3 == NULL ||
     lib ==  NULL || second == NULL || third == NULL || collection2b == NULL
    )
    CU_FAIL("La mémoire demandée pour créer une librairie d'auteur n'a pas été allouée");

  collection2->titre = "Divergente";
  collection2->suiv = collection2b;

  collection2b->titre = "Insurgent";
  collection2b->suiv = collection2c;

  collection2c->titre = "Allegiant";
  collection2c->suiv = NULL;

  collection3->titre = "Hunger Games";
  collection3->suiv = collection3b;

  collection3b->titre = "Hunger Games 2";
  collection3b->suiv = collection3c;

  collection3c->titre = "Hunger Games 3";
  collection3c->suiv = NULL;


  collection4->titre = "Enclave";
  collection4->suiv = NULL;


  lib1->auteur = "Ann Aguirre";
  lib1->Livres = collection4;
  lib1->next = second;

  second->auteur = "Suzanne Colins";
  second->Livres = collection3;
  second->next = third;

  third->auteur = "Véronica Roth";
  third->Livres = collection2;
  third->next = fourth;

  fourth->auteur = "A Bellemans";
  fourth->Livres = NULL;
  fourth->next = NULL;
}

void setup_lib2(void)
{
  lib2 = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  cellLivre *collection2 = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection2b = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellLivre *collection3 = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3b = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3c = (cellLivre *)(malloc(sizeof(cellLivre)));
  cellLivre *collection3d = (cellLivre *)(malloc(sizeof(cellLivre)));

  cellAuteur *second = (cellAuteur *)(malloc(sizeof(cellAuteur)));
  cellAuteur *third = (cellAuteur *)(malloc(sizeof(cellAuteur)));
  cellAuteur *fourth = (cellAuteur *)(malloc(sizeof(cellAuteur)));

  if(collection2 == NULL || collection3 == NULL ||
     lib ==  NULL || second == NULL || third == NULL || collection2b == NULL
    )
      CU_FAIL("La mémoire demandée pour créer une librairie d'auteur n'a pas été allouée");



  collection2->titre = "Harry Potter";
  collection2->suiv = collection2b;

  collection2b->titre = "L'enfer c'est à quel étage ?";
  collection2b->suiv = NULL;


  collection3->titre = "Le Héros Perdu";
  collection3->suiv = collection3b;

  collection3b->titre = "Le fils de Neptune";
  collection3b->suiv = collection3c;

  collection3c->titre = "La Marque d'Athéna";
  collection3c->suiv = collection3d;

  collection3d->titre = "La Maison d'Hadès";
  collection3d->suiv = NULL;



  lib2->auteur = "J.K Rowling";
  lib2->Livres = NULL;
  lib2->next = second;

  second->auteur = "Serge Brussolo";
  second->Livres = collection2;
  second->next = third;

  third->auteur = "Rick Riordan";
  third->Livres = collection3;
  third->next = fourth;

  fourth->auteur = "A Bellemans";
  fourth->Livres = NULL;
  fourth->next = NULL;
}

//Fonction de vérification pour les tests (différentes de celles de l'utilisateur)
//---------------------------------------------- begin
int recherche_livre(cellAuteur * librairie, char * auteur, char * titre)
{
  //recherche du livre ajouté...
  cellAuteur *current = librairie;
  while(current != NULL)
  {
    if(strcmp(current->auteur, auteur) == 0)
    {
      cellLivre *books = current->Livres;
      while(books != NULL)
      {
        if(strcmp(books->titre, titre) == 0)
        {
          return 1;
        }
        books = books->suiv;
      } //end while books
    } // end if
    current = current->next;
  } // end while auteurs
  return 0;
}

cellAuteur * recherche_auteur(cellAuteur * librairie , char * auteur)
{
  cellAuteur *current = librairie;
  while(current != NULL)
  {
    if(strcmp(auteur, current->auteur) == 0) return current;
    current = current->next;
  }
  return NULL;
}

int compte_auteur(cellAuteur * librairie)
{
  cellAuteur *current = librairie;
  int count = 0;
  while(current != NULL)
  {
    count++;
    current = current->next;
  }
  return count;
}

int compte_livre(cellAuteur *librairie, char *strAuteur, char *strLivre)
{
  cellAuteur *author = recherche_auteur(librairie, strAuteur);//modif
  cellLivre *books = author->Livres;

  int countBook = 0;
  while(books != NULL)
  {
    if(strcmp(books->titre, strLivre) == 0)
    {
      countBook++;
    }
    books = books->suiv;
  }
  return countBook;
}

//Fonction qui libérera toute la mémoire occupée par les librairies créées
void deleteALL(cellAuteur* lib)
{
  cellAuteur *current = lib;
  while(current != NULL)
  {
    cellLivre *books = current->Livres;
    while(books != NULL)
    {
      cellLivre *toRemove = books;
      books = books->suiv;
      free(toRemove);
    }
    cellAuteur *toRemove = current;
    current = current->next;
    free(toRemove);
  }
  nbUseOfFree = 0; //réinitialisation
	return;
}
//----------------------------------------------- end


//------------------------Commencement des tests------------------------------//

// @existe:test_existe_auteurNotFound => [La fonction existe ne gère pas la cas où l'auteur passé en argument ne se trouve pas dans la librairie]
void test_existe_auteurNotFound(void)
{
  setup_lib();
  cellAuteur *empty = existe(lib, "jodie");
  CU_ASSERT_PTR_NULL(empty);
  deleteALL(lib);
}

// @existe:test_existe_auteurFound => [La fonction existe ne renvoie pas le bon auteur]
void test_existe_auteurFound(void)
{
  setup_lib();
  cellAuteur *found1 = existe(lib, "thomas");
  cellAuteur *found2 = existe(lib, "florent");
  cellAuteur *found3 = existe(lib, "jordan");

  CU_ASSERT_STRING_EQUAL(found1->auteur, "thomas");
  CU_ASSERT_STRING_EQUAL(found2->auteur, "florent");
  CU_ASSERT_STRING_EQUAL(found3->auteur, "jordan");
  deleteALL(lib);
}

// @existe:test_existe_parmametreIncorrect => [La fonction existe ne gère pas le cas où le paramètre strAuteur vaut NULL]
void test_existe_parmametreIncorrect(void)
{
  setup_lib();
  cellAuteur *incorrect = existe(lib, NULL);
  CU_ASSERT_PTR_NULL(incorrect);
  deleteALL(lib);
}

// @compteOuvrage:test_compteOuvrage => [La fonction compteOuvrage ne compte pas correctement les livres d'un auteur se trouvant dans la librairie]
void test_compteOuvrage(void)
{
  setup_lib1();
  int compte = compteOuvrage(lib1,"Ann Aguirre");
  CU_ASSERT_EQUAL(compte, 1);
  compte = compteOuvrage(lib1,"Suzanne Colins");
  CU_ASSERT_EQUAL(compte, 3);
  compte = compteOuvrage(lib1,"Véronica Roth");
  CU_ASSERT_EQUAL(compte, 3);
  compte = compteOuvrage(lib1,"A Bellemans");
  CU_ASSERT_EQUAL(compte, 0);
  deleteALL(lib1);
}

jmp_buf label_test_compteOuvrage_auteurNotInside;
void sig_handler2(int signo) {
    siglongjmp(label_test_compteOuvrage_auteurNotInside,1);
}
// @compteOuvrage:test_compteOuvrage_auteurNotInside => [La fonction compteOuvrage ne gère pas le cas où l'auteur ne se trouverait pas dans la librairie et segfault]
void test_compteOuvrage_auteurNotInside(void)
{
  setup_lib1();
  if (signal(SIGSEGV, sig_handler2) == SIG_ERR)
  {
    CU_FAIL("Impossible d'enregistrer un signal handler.");
    return;
  }
  if(setjmp(label_test_compteOuvrage_auteurNotInside)==0){
    int compte = compteOuvrage(lib1, "J.K Rowling");
    CU_ASSERT_EQUAL(compte, 0);
  }
  else
    CU_ASSERT_TRUE(0);

  signal(SIGSEGV, SIG_DFL);
  deleteALL(lib1);
}

jmp_buf label_test_auteurSansLivres;

void sig_handler(int signo) {
    siglongjmp(label_test_auteurSansLivres,1);
}

// @add: test_add_auteurSansLivres => [La fonction add provoque un segfault lorsqu'on demande d'ajouter un livre à un auteur dont sa liste d'ouvrages est vide]
void test_add_auteurSansLivres(void)
{
  setup_lib2();
  if (signal(SIGSEGV, sig_handler) == SIG_ERR)
  {
    CU_FAIL("Impossible d'enregistrer un signal handler.");
    return;
  }

  if(setjmp(label_test_auteurSansLivres)==0){
    add(lib2, "A Bellemans", "Physique 5");
    CU_ASSERT_TRUE(1);
  }
  else
    CU_ASSERT_TRUE(0);

  signal(SIGSEGV, SIG_DFL);
  deleteALL(lib2);
}

// @add:test_add_auteurAvecLivres => [La fonction add n'ajoute pas correctement le livre dans la liste d'ouvrage de l'auteur demandé]
void test_add_auteurAvecLivres(void)
{
  setup_lib2();
  add(lib2, "J.K Rowling", "Harry Potter 2");
  if(recherche_livre(lib2,"J.K Rowling","Harry Potter 2") == 1)
  {
    CU_ASSERT_TRUE(1);
  }
  else
  {
    CU_ASSERT_TRUE(0);
  }
  deleteALL(lib2);
}

// @add:test_add_le_meme_livre => [La fonction add ajoute un live déjà présent dans la collection d'un même auteur]
void test_add_le_meme_livre(void)//test ajout d'un livre déjà présent
{
  setup_lib2();
  add(lib2,"Rick Riordan","Le fils de Neptune");
  int compte = compte_livre(lib2, "Rick Riordan","Le fils de Neptune");
  CU_ASSERT_EQUAL(compte,1);
  deleteALL(lib2);
}

// @supprimer:test_supprimer_auteur => [La fonction ne libère pas la mémoire correctement]
void test_supprimer_auteur(void)
{
  setup_lib2();
  nbUseOfFree = 0;
  supprimer(&lib2, "Rick Riordan");

  CU_ASSERT_EQUAL(nbUseOfFree, 5);//puisqu'on a ajouté un livre chez jordan voir
                                  //fonction du dessus (test_add_auteurAvecLivres)

  nbUseOfFree = 0; //réinitialisation du nombre d'utilisation de la fonction free
  deleteALL(lib2);
}

// @supprimer:test_suppression_premier_auteur => [La fonction supprimer ne supprime pas correctement le premier auteur de la liste librairie]
void test_suppression_premier_auteur(void)
{
  setup_lib2();
  supprimer(&lib2, "J.K Rowling");
  if(recherche_auteur(lib2,"J.K Rownling") == NULL  && compte_auteur(lib2) == 3)
  {
    CU_ASSERT_TRUE(1);
  }
  else
  {
    CU_ASSERT_TRUE(0);
  }
  deleteALL(lib2);
}

// @supprimer:test_suppression_autre_auteur => [La fonction supprimer ne supprime pas correctement un auteur quelconque de la liste librairie]
void test_suppression_autre_auteur(void)
{
  setup_lib2();
  supprimer(&lib, "Serge Brussolo");
  if(recherche_auteur(lib2,"Serge Brussolo") == NULL && compte_auteur(lib2) == 3)
  {
    CU_ASSERT_TRUE(1);
  }
  else
  {
    CU_ASSERT_TRUE(0);
  }
  deleteALL(lib2);
}


//Fonction de fin de tests. Nous en l'utilisons pas
int teardown(void)
{
    return 0; //en effet, toutes les librairies sont déjà libérées dans les tests.
}

int setup(void) {return 0;}


int main(int argc, char const *argv[]) {
	//Initialisation des tests
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	CU_pSuite pSuite = NULL;


	pSuite = CU_add_suite("BookStore", setup, teardown);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	//Ajout des tests
	if (NULL == CU_add_test(pSuite, "test_existe_auteurNotFound", test_existe_auteurNotFound) ||
      NULL == CU_add_test(pSuite, "test_existe_auteurFound", test_existe_auteurFound) ||
      NULL == CU_add_test(pSuite, "test_compteOuvrage", test_compteOuvrage) ||
      NULL == CU_add_test(pSuite, "test_compteOuvrage_auteurNotInside", test_compteOuvrage_auteurNotInside) ||
      NULL == CU_add_test(pSuite, "test_add_auteurSansLivres", test_add_auteurSansLivres) ||
      NULL == CU_add_test(pSuite, "test_add_auteurAvecLivres", test_add_auteurAvecLivres) ||
      NULL == CU_add_test(pSuite, "test_add_le_meme_livre", test_add_le_meme_livre) ||
      NULL == CU_add_test(pSuite, "test_supprimer_auteur", test_supprimer_auteur) ||
      NULL == CU_add_test(pSuite, "test_supprimer_premier_auteur", test_suppression_premier_auteur)
	   )
  {
		CU_cleanup_registry();
		return CU_get_error();
	}


	//Lancement des tests
	CU_automated_run_tests();

	return CU_get_error();
}
