#include<stdio.h>
#include<stdlib.h>

#include "student_code.h"

#include <string.h>

struct node *pair_filter(struct node *head)
{
	if (head==NULL)
		return NULL;
	const size_t size = sizeof(struct node);
	struct node *nHead = malloc(size);
	if (!nHead)
		return NULL;
	memcpy(nHead, head, size);
	if (!head->next||!head->next->next){
		nHead->next = NULL;
		return nHead;
	}
	struct node *next = nHead;
	for (struct node *runner = head; ; runner = runner->next->next){
		next->next = malloc(size);
		if (!next->next)
			return NULL;
		memcpy(next->next, runner, size);
		if (!runner->next || !runner->next->next)
			break;
	}
	next->next->next = NULL;
	return nHead;
}
