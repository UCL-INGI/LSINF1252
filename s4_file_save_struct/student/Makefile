# MakeFile for the tasks using Chestier
CC=gcc
CPP=cppcheck
EXEC=tests
CPPFLAGS=--error-exitcode=1
LDFLAGS=-lcunit -lm -lpthread -ldl -rdynamic
SRC=$(wildcard *.c) CTester/wrap_mutex.c CTester/wrap_malloc.c CTester/wrap_file.c CTester/wrap_sleep.c CTester/CTester.c CTester/trap.c
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -DC99 -std=gnu99 -ICTester
WRAP=-Wl,-wrap=pthread_mutex_lock -Wl,-wrap=pthread_mutex_unlock -Wl,-wrap=pthread_mutex_trylock -Wl,-wrap=pthread_mutex_init -Wl,-wrap=pthread_mutex_destroy -Wl,-wrap=malloc -Wl,-wrap=free -Wl,-wrap=realloc -Wl,-wrap=calloc -Wl,-wrap=open -Wl,-wrap=creat -Wl,-wrap=close -Wl,-wrap=read -Wl,-wrap=write -Wl,-wrap=stat -Wl,-wrap=fstat -Wl,-wrap=lseek -Wl,-wrap=exit -Wl,-wrap=sleep

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) $(WRAP) -o $@ $(OBJ) $(LDFLAGS)

create-po:
	mkdir -p po/fr/
	xgettext --keyword=_ --language=C --add-comments --sort-output --from-code=UTF-8 -o po/tests.pot $(SRC)
	msginit --input=po/tests.pot --locale=fr_BE.utf8 --output=po/fr/tests.po

update-po:
	xgettext --keyword=_ --language=C --add-comments --sort-output --from-code=UTF-8 -o po/tests.pot $(SRC)
	msgmerge --update po/fr/tests.po po/tests.pot

compile-mo:
	msgfmt --no-hash --output-file=po/fr/tests.mo po/fr/tests.po
	mkdir -p fr/LC_MESSAGES/
	cp po/fr/tests.mo fr/LC_MESSAGES/tests.mo
    
check: $(SRC)
	$(CPP) $(CPPFLAGS) $<

clean:
	rm -f $(EXEC) $(OBJ)

.PHONY: tests

