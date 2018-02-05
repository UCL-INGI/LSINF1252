all:
	gcc -D C99 -std=gnu99 -Wall -Werror -fPIC -shared -o myfunctions.so code/myfunctions.c -pthread -ldl
	gcc -D C99 -std=gnu99 -Wall -Werror code/student_code.c code/tests.c -lcunit -rdynamic -pthread -o tests

run:
	LD_PRELOAD=./myfunctions.so ./tests

.PHONY: run
