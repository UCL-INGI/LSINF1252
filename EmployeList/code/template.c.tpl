#include<stdio.h>
#include<string.h>

#include "student_code.h"

/*
 * @pre list!=NULL
 * @post initialise la liste chaînée
 */
void initializeList(LinkedList *list) {
  list->head=NULL;
  list->tail=NULL;
}

/*
 * @pre list!=NULL, data!=NULL, data pointe vers Employe
 * @post Crée un noeud contenant un pointeur vers data
 *       et l'ajoute en début de liste
 *       retourne un pointeur vers le noeud créé, NULL en cas d'erreur
 */
Node * addHead(LinkedList *list, void *data) {
  Node *node= (Node *) malloc(sizeof(Node));
  if(node==NULL)
    return NULL;
  node->data=data;
  if(list->head==NULL) {
    list->tail=node;
    node->next=NULL;
  } else {
    node->next=list->head;
  }
  list->head=node;
  return node;
}

/*
 * @pre list!=NULL, node!=NULL
 * @post retourne le nombre d'occurences du noeud node
 */
int count(LinkedList *list, Node *n) {
  int count=0;
  Node *node=list->head;
  while(node!=NULL) {
    if(node==n)
      count++;
    node=node->next;
  }
  return count;

}
/*
 * @pre list!=NULL
 * @post retourne 0 si elle est correcte, -1 en cas d'erreur
 */
int verifyList(LinkedList *list) {
  if(list->head==NULL && list->tail==NULL) {
    return 0;
  }
  Node *node=list->head;
  while(node!=NULL) {
    if(node->next==NULL && node==list->tail) {
      return 0;
    }
    node=node->next;
  }
  return -1;
}

/*
 * @pre list!=NULL, age>0
 * @post retire de la liste tous les employés strictement plus jeunes
 *       que age. Retourne le nombre d'employés retirés de la
 *       liste, -1 en cas d'erreur.
 */
int deleteAllYounger(LinkedList *list, int age) {
@@deleteAllYounger@@
}

void montest(void) {

@@montest@@

}
