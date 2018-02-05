all: 
	gcc -D C99 -std=gnu99 -Wall -Werror -fPIC -shared -o mydestroy.so code/mydestroy.c -ldl 
	gcc -pthread -D C99 -std=gnu99 -Wall -Werror code/student_code.c code/tests.c -lcunit -rdynamic -o tests
	
run:
	LD_PRELOAD=./mydestroy.so ./tests

.PHONY: run
