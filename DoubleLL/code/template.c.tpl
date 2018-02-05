// source: http://www.thelearningpoint.net/computer-science/data-structures-doubly-linked-list-with-c-program-source-code

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

/*
 * @pre l!=NULL
 * @post a initialisé la liste doublement chaînée l. Une liste vide
 *       contient toujours un premier noeud qui est vide
 *       et dont les pointeurs next et prev pointent vers NULL
 *       retourne 0 en cas de succès, -1 en cas d'erreur
 */

int list_init(list *l) {

  l->start=malloc(sizeof(node));
  if(l->start==NULL) 
    return -1;
  l->start->next=NULL;
  l->start->prev=NULL;
  l->start->data=-1;
  l->tail=l->start;
  return 0;
}

/*
 * @pre l!=NULL
 * @post insert à la fin de la liste un noeud content la donnée data
 *       retourne -1 en cas d'erreur, 0 sinon
 */
int insert(list *l, int data)
{
  node *pointer;
  pointer = (node *)malloc(sizeof(node));
  if(pointer==NULL)
    return -1;
  l->tail->next=pointer;
  pointer->prev=l->tail;
  l->tail=pointer;
  pointer->data = data;
  pointer->next = NULL;
  return 0;
}

// non founi
int count(list *list, int key)
{
  int count=0;
  node *pointer;
  pointer=list->start;
  pointer =  pointer -> next; //First node is dummy node.
  /* Iterate through the entire linked list and search for the key. */
  while(pointer!=NULL)
    {
      if(pointer->data == key) //key is found.
        {
          count++;
        }
      pointer = pointer -> next;//Search in the next node.
    }
  /*Key is not found */
  return count;
}
/*
 * @pre l!=NULL
 * @post a retiré de la liste l tous les noeuds dont la donnée
 *       est key
 *       retourne le nombre de noeuds retirés, -1 en cas d'erreur
 */
int delete(list *l, int key)
{
@@dl@@
}

// verifie la structure de la liste, 0 si correct, -1 en cas d'erreur
int verify(list *l) {
  // liste vide
  if(l->start->next==NULL && l->start->prev==NULL) {
    return 0;
  }
  // liste non-vide
  // parcours en avant
  node * pointer, * previous;
  previous=l->start;
  pointer=l->start->next;
  while(pointer!=NULL) {
    printf("looking at %d\n",pointer->data);
    if(pointer->prev != previous) {
      // error
    printf("error prev\n");
    return -1;
    }
    previous= pointer;
    pointer = pointer -> next;
  }
  // verifie que c'est tail
  if(previous!=l->tail) {
    // error
    printf("error tail\n");
    return -1;
  }
  return 0;
}

//non fourni
void print(node *pointer)
{
  if(pointer==NULL) {
    return;
  }
  printf("%d ",pointer->data);
  print(pointer->next);
}

void montest(void) {
@@montest@@
}
