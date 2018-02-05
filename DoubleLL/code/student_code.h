#include<stdio.h>
#include<stdlib.h>

// Noeud de la liste chaînée
typedef struct Node {
        int data;
        struct Node *next;
        struct Node *prev;
} node;

// La liste doublement chaînée. Cette liste comprend un premier
// noeud vide qui ne contient pas de donnée utile. Les données
// utiles sont stockées à partir du second noeud.

typedef struct List {
  node *start;  // pointe toujours vers le premier élément
  node *tail;   // pointe toujours vers le dernier élément
} list;

int delete(list *l, int key);
int list_init(list *l);
void montest(void);
