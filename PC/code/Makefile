CC=gcc
EXEC=tests
LDFLAGS=-lcunit -lpthread -Wl,-wrap=sem_wait -Wl,-wrap=sem_post 

SRC=$(wildcard code/*.c)
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -D C99 -std=gnu99 -I/opt/local/include

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./tests

.PHONY: run
