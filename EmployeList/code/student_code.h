#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Un employé d'une entreprise
 */

typedef struct employe {
  char *name;  // nom de l'employé 
  unsigned int age; // âge de l'employé
} Employe;
/* 
 * Un noeud de la liste
 */
typedef struct node {
  void *data; // pointeur vers la donnée stockée
  struct node *next; // pointeur vers le noeud suivant
} Node;

/* 
 * La structure représentant la liste
 */
typedef struct _linkedList {
  Node *head; // le premier noeud
  Node *tail; // le dernier noeud
} LinkedList;

void initializeList(LinkedList *list);
Node * addHead(LinkedList *list, void *data);
int deleteAllYounger(LinkedList *list, int age) ;
