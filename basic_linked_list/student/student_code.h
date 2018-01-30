#ifndef _STUDENT_H
#define  _STUDENT_H

/**
* Structure node
*
* @next: pointer to the next node in the list, NULL if last node_t
* @value: value of the node
*/
typedef struct node{
  struct node* next;
  int value;
} node_t;

/**
* Structure list
*
* @first: first node of the list, NULL if empty
* @size: number of nodes in the list.
*/
typedef struct list{
  struct node* first;
  int size;
} list_t  ;

/*
* Create a new node with the value @value
*
* @value: value of the node
* @return: return the pointer to the new node, NULL either.
*/
node_t* init_node(int value);

/*
* Create a new empty list
*
* @return: return the pointer to the new list, NULL either.
*/
list_t* init_list();

/*
* Free the node @n
*/
void free_node(node_t* n);

/*
* Free the list @l
*/
void free_list(list_t* l);

/*
* get function
*
* @l: list of integers
* @index: index of the node in the list to get
*
* @return: the node at index @index or NULL if there's not such a node.
*/
node_t* get(list_t* l, int index);

/*
* Add an element to the list @l
*
* @l: list of integers
* @value: value to add to the list
*
* @return: 0 if success, -1 either
*/
int add_node(list_t* list, int value);

/**
* Remove an element from the list @l
*
* @l: list of integers
* @index: index of the element to Remove
*
* @return: 0 if success, -1 either
*/
int remove_node(list_t* l, int index);



#endif
