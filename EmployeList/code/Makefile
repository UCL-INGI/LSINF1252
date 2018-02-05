CC=gcc
EXEC=tests
LDFLAGS=-lcunit -Wl,-wrap=malloc -Wl,-wrap=free

SRC=$(wildcard code/*.c)
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -D C99 -std=c99 
all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./tests

.PHONY: run
