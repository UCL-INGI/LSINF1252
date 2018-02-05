/* Tests unitaires pour btree-access
Copyright (C) 2016 Bertrand Van Ouytsel Charles-Henry

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
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include "student_code.h"

// @has_key:keyOnFirst => [Votre fonction n'arrive pas trouver la clé recherchée dans le nœud racine.]
void keyOnFirst(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
	CU_ASSERT_TRUE(has_key(A, 20));
	
}
// @has_key:keyNotHere => [Votre fonction ne renvoie pas 0 quand la clé recherchée n'est pas présente.]
void keyNotHere(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
    CU_ASSERT_FALSE(has_key(A, 3));
}
// @has_key:keyOnLeft => [Votre fonction ne renvoie 1 lorsque la clé recherchée se trouve dans un noeud à gauche de la racine.]
void keyOnLeft(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,42,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
    CU_ASSERT_TRUE(has_key(A, 15));
}
// @has_key:keyOnRight => [Votre fonction ne renvoie pas 1 lorsque la clé recherchée se trouve dans un noeud à droite de la racine.]
void keyOnRight(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,19,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
    CU_ASSERT_TRUE(has_key(A, 25));
}
// @compare:treesEquals => [Votre fonction ne renvoie pas 1 alors que les deux arbres sont égaux.]
void treesEquals(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
	Node N={35,2,NULL,NULL};
	Node M={25,133,NULL,NULL};
	Node L={15,24,NULL,NULL};
	Node K={5,4,NULL,NULL};
	Node J={30,74,&M,&N};
	Node I={10,36,&K,&L};
	Node H={20,23,&I,&J};
	
	int result=compare(A,H);
	CU_ASSERT_EQUAL(result,1);
}
// @compare:treesWithDifferentsValues => [Votre fonction ne renvoie pas 0 alors que les deux arbres possèdent des nœuds avec des valeurs différentes.]
void treesWithDifferentsValues(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,1,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
	Node N={35,2,NULL,NULL};
	Node M={25,133,NULL,NULL};
	Node L={15,24,NULL,NULL};
	Node K={5,4,NULL,NULL};
	Node J={30,74,&M,&N};
	Node I={10,36,&K,&L};
	Node H={20,23,&I,&J};
	
	int result=compare(A,H);
	CU_ASSERT_EQUAL(result,0);
}
// @compare:treesWithDifferentsKeys => [Votre fonction ne renvoie pas 0 alors que les deux arbres possèdent des noeuds avec des clés différentes.]
void treesWithDifferentsKeys(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={7,36,&D,&E};
	Node A={20,23,&B,&C};
	
	Node N={35,2,NULL,NULL};
	Node M={25,133,NULL,NULL};
	Node L={15,24,NULL,NULL};
	Node K={5,4,NULL,NULL};
	Node J={30,74,&M,&N};
	Node I={10,36,&K,&L};
	Node H={20,23,&I,&J};
	
	int result=compare(A,H);
	CU_ASSERT_EQUAL(result,0);
}
// @compare:treesWithDifferentLeftPart => [Votre fonction ne renvoie pas 0 alors que les deux arbres ont une structure différente à gauche.]
void treesWithDifferentLeftPart(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
	Node N={35,2,NULL,NULL};
	Node M={25,133,NULL,NULL};
	Node K={5,4,NULL,NULL};
	Node J={30,74,&M,&N};
	Node I={10,36,&K,NULL};
	Node H={20,23,&I,&J};
	
	int result=compare(A,H);
	CU_ASSERT_EQUAL(result,0);
}
// @compare:treesWithDifferentsRightPart => [Votre fonction ne renvoie pas 0 alors que les deux arbres ont une structure différente à droite.]
void treesWithDifferentsRightPart(void)
{
	Node G={35,2,NULL,NULL};
	Node F={25,133,NULL,NULL};
	Node E={15,24,NULL,NULL};
	Node D={5,4,NULL,NULL};
	Node C={30,74,&F,&G};
	Node B={10,36,&D,&E};
	Node A={20,23,&B,&C};
	
	Node N={35,2,NULL,NULL};
	Node L={15,24,NULL,NULL};
	Node K={5,4,NULL,NULL};
	Node J={30,74,NULL,&N};
	Node I={10,36,&K,&L};
	Node H={20,23,&I,&J};
	
	int result=compare(A,H);
	CU_ASSERT_EQUAL(result,0);
}
int main (int argc, char** argv)
{
	
 if(CUE_SUCCESS != CU_initialize_registry())
   {return CU_get_error();}
  CU_pSuite pSuiteHasKey = CU_add_suite("Suite de Test pour la fonction int has_key(Node root,int key)",NULL,NULL);
  CU_pSuite pSuiteCompare = CU_add_suite("Suite de Test pour la fonction int compare(Node root_a, Node root_b)",NULL,NULL);
 
  if(NULL== pSuiteHasKey||NULL==pSuiteCompare)
   	{
	 CU_cleanup_registry();
	return CU_get_error();
	}
  if(NULL==CU_add_test(pSuiteHasKey, "keyOnFirst", keyOnFirst)||
     NULL==CU_add_test(pSuiteHasKey, "keyNotHere", keyNotHere)||
     NULL==CU_add_test(pSuiteHasKey, "keyOnRight", keyOnRight)||
     NULL==CU_add_test(pSuiteHasKey, "keyOnLeft", keyOnLeft)||
     NULL==CU_add_test(pSuiteCompare,"treesEquals", treesEquals)||
     NULL==CU_add_test(pSuiteCompare,"treesWithDifferentsValues",treesWithDifferentsValues)||
     NULL==CU_add_test(pSuiteCompare,"treesWithDifferentsKeys",treesWithDifferentsKeys)||
     NULL==CU_add_test(pSuiteCompare,"treesWithDifferentLeftPart",treesWithDifferentLeftPart)||
     NULL==CU_add_test(pSuiteCompare,"treesWithDifferentsRightPart",treesWithDifferentsRightPart))
	{CU_cleanup_registry();
	return CU_get_error();}

    CU_automated_run_tests();

}
