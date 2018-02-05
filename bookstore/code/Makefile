all:
	gcc -D C99 -std=gnu99 -Wall -Werror -fPIC -shared -o myfree.so code/myfree.c -pthread -ldl
	gcc -D C99 -std=gnu99 -Wall -Werror code/student_code.c code/tests.c -lcunit -rdynamic -pthread -o tests

run:
	LD_PRELOAD=./myfree.so ./tests

.PHONY: run
