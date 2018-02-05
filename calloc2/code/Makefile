all:
	gcc -D C99 -std=gnu99 -Wall -Werror -fPIC -shared -o mymalloc.so code/mymalloc.c -ldl
	gcc -D C99 -std=gnu99 -Wall -Werror code/student_code.c code/tests.c -lcunit -rdynamic -o tests

run:
	LD_PRELOAD=./mymalloc.so ./tests

.PHONY: run
