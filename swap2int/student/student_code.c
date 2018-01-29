#include<stdio.h>
#include<stdlib.h>

void swap(int *i, int *j){
	int k = *j;
	*j = *i;
	*i = k;
}


