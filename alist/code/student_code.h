#include<stdio.h>
#include<stdlib.h>

void montest(void);


struct array_list *arraylist_init(size_t n_elem, size_t elem_size);
int set_element(struct array_list *alist, int index, void *elem);
int get_element(struct array_list *alist, int index, void *elem);
size_t get_size(struct array_list *alist);
size_t get_elem_size(struct array_list *alist);
int add_tail(struct array_list *alist, void *elem);
int change_size(struct array_list *alist, size_t size);
int find_element(struct array_list *alist, void *elem);
int array_list_destroy(struct array_list *alist);


struct array_list {
  char *content;
  size_t size;
  size_t elem_size;
  sem_t mutex;
};







