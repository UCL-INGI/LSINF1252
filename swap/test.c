#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "swap.h"

int main(void)
{
        struct fract_t f1;
        struct fract_t f2;
        int f1num, f1denum, f2num, f2denum,error;

        srandom(time(NULL));

	error = 0;

        f1num = random();
        f1denum = random();
        f2num = random();
        f2denum = random();

        f1.num = f1num;
        f1.denum = f1denum;
        f2.num = f2num;
        f2.denum = f2denum;

        printf("Avant l'appel à swap:\n");
        printf("a.num %d a.denum %d, b.num %d b.denum %d\n", f1.num, f1.denum, f2.num, f2.denum);
	
        swap(&f1, &f2);

        printf("Après l'appel à swap:\n");
        printf("a.num %d a.denum %d, b.num %d b.denum %d\n", f1.num, f1.denum, f2.num, f2.denum);

        if (f1.num != f2num) {
                fprintf(stderr, "a.num est %d, mais devrait être %d\n", f1.num, f2num);
		error = 1;
	}

        if (f1.denum != f2denum) {
                fprintf(stderr, "a.denum est %d, mais devrait être %d\n", f1.denum, f2denum);
		error = 1;
	}

        if (f2.num != f1num) {
                fprintf(stderr, "b.num est %d, mais devrait être %d\n", f2.num, f1num);
		error = 1;
	}

        if (f2.denum != f1denum) {
                fprintf(stderr, "b.denum est %d, mais devrait être %d\n", f2.denum, f1denum);
		error = 1;
	}

        return error;
}
