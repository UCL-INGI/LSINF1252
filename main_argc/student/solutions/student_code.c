#include "student_code.h"
#define main main2
#include <stdio.h>
int main(int argc, char **argv)
{
	if (argc == 1){
		printf("\n");
		return 0;
	}
	printf("%s", *(argv+1));
	for (int i = 3; i<argc; i +=2)
		printf(" %s", *(argv+i));
	printf("\n");
	return 0;
}
