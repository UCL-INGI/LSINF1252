#include<stdio.h>
#include<stdlib.h>

int factorial(int n)
{
	if (n<=1)
		return 1;
	int sol = n;
	for (int j = n-1; j>0; j--)
		sol = sol*j;
	return sol;
}


