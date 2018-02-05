CC=gcc
EXEC=tests
LDFLAGS=-lpthread -lcunit -Wl,-wrap=sem_post -Wl,-wrap=sem_wait -Wl,-wrap=realloc 
SRC=$(wildcard code/*.c)
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -D C99 -std=c99 -I/opt/local/include

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./tests

.PHONY: run
