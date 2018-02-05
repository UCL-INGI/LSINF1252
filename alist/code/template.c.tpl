#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#include "lecture.h"

struct array_list {
	char *content;
	size_t size;
	size_t elem_size;
	sem_t mutex;
};

/* Initialize an array-list
 * 
 * @return: Pointer to array_list on success, NULL on error and errno is set.
 */
struct array_list *arraylist_init(size_t n_elem, size_t elem_size)
{
	struct array_list *alist= (struct array_list *)malloc(sizeof(*alist));

	if (!alist)
		return NULL;

	alist->content = malloc(n_elem * elem_size);
	if (!alist->content) {
		free(alist);
		return NULL;
	}
	
	if (sem_init(&alist->mutex, 0, 1)) {
		free(alist->content);
		free(alist);
		return NULL;
	}

	alist->size = n_elem;
	alist->elem_size = elem_size;

	return alist;
}

/* Set the element 'elem' at position 'index' (indexes start at 0) inside the
 * array-list
 *
 * @return: -1 on error, 0 on success.
 */
int set_element(struct array_list *alist, int index, void *elem)
{
	char *ptr;

	if (index >= alist->size)
		return -1;

	if (sem_wait(&alist->mutex))
		return -1;

	ptr = alist->content + (index * alist->elem_size);
	memcpy(ptr, elem, alist->elem_size);

	if (sem_post(&alist->mutex))
		return -1;

	return 0;
}

/* Get the element at position 'index' of the array-list and copy it into 'elem'.
 * It is exepected that 'elem' has a sufficiently large memory-region to
 * hold the data.
 *
 * @return: -1 on error, 0 on success.
 */
int get_element(struct array_list *alist, int index, void *elem)
{
	char *ptr;

	if (index >= alist->size)
		return -1;

	if (sem_wait(&alist->mutex))
		return -1;

	ptr = alist->content + (index * alist->elem_size);
	memcpy(elem, ptr, alist->elem_size);

	if (sem_post(&alist->mutex))
		return -1;

	return 0;
}

/* Get the current size of the array-list */
size_t get_size(struct array_list *alist)
{
	return alist->size;
}

/* Get the size of the elements of the array-list */
size_t get_elem_size(struct array_list *alist)
{
	return alist->elem_size;
}

/* Expand the array-list by one element and put 'elem' at this place.
 *
 * @return: -1 on error, 0 on success
 */
int add_tail(struct array_list *alist, void *elem)
{
@@ArrayList@@
}

/* Change the size of the array-list. If size is smaller than the actual size
 * of the array-list, elements will be lost
 *
 * @return: -1 on error, 0 on success
 */
int change_size(struct array_list *alist, size_t size)
{
	if (sem_wait(&alist->mutex))
		return -1;

	alist->content = realloc(alist->content, size * alist->elem_size);
	if (alist->content == NULL) {
		sem_post(&alist->mutex);
		return -1;
	}

	alist->size = size;

	if (sem_post(&alist->mutex))
		return -1;

	return 0;
}

/* Find the element 'elem' in the array-list and return the index. On case of
 * error, or if the element is not in the array-list, -1 is returned.
 *
 * Elements are equal, if each byte is the same.
 */
int find_element(struct array_list *alist, void *elem)
{
	int i;

	if (sem_wait(&alist->mutex))
		return -1;

	for (i = 0; i < alist->size; i++) {
		char *ptr = alist->content + (i * alist->elem_size);
		if (memcmp((void *)ptr, elem, alist->elem_size))
			continue;
		/* Found the element */
		break;
	}
	
	if (sem_post(&alist->mutex))
		return -1;

	return i < alist->size ? i : -1;
}

/* Fully destroys the array-list. All elements will be lost */
int array_list_destroy(struct array_list *alist)
{
	if (sem_wait(&alist->mutex))
		return -1;

	free(alist->content);
	sem_destroy(&alist->mutex);
	free(alist);

	return 0;
}

void montest(void) {
 @@montest@@

}