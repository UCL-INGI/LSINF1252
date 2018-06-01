CC=gcc
EXEC=tests
LDFLAGS=-lcunit -Wl,-wrap=fork -Wl,-wrap=waitpid -Wl,-wrap=open -Wl,-wrap=fopen -Wl,-wrap=dup2 -Wl,-wrap=system
#SRC=$(wildcard code/*.c)
SRC = $(filter-out code/code.c, $(wildcard code/*.c))
OBJ=$(SRC:.c=.o)
CFLAGS=-I/opt/local/include

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -Wall -Werror -D C99 -std=gnu99 -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./tests

.PHONY: run
