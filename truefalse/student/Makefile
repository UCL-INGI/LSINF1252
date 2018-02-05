EXEC=tests
CFLAGS=-Wall -Werror -D C99 -std=gnu99 -g
CC=gcc
LDFLAGS=-ljansson
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC) 

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ) 
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

po/fr/$(EXEC).mo: po/fr/$(EXEC).po 	
	msgfmt --output-file=$@ $< 

po/fr/$(EXEC).po: po/$(EXEC).pot 
	msgmerge --output-file=$@ $@ $< 

po/$(EXEC).pot: $(EXEC).c 
	xgettext -k_ -j --from-code=utf-8  -lC -c -s -o po/$(EXEC).pot $(EXEC).c 

run:
	./tests

clean: 
	@rm -f $(EXEC) po/fr/*.mo *~

.PHONY: run
