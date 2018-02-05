CC=gcc
EXEC=tests
LDFLAGS=-lcunit -Wl,-wrap=open -Wl,-wrap=close -Wl,-wrap=read -Wl,-wrap=write

SRC=$(wildcard code/*.c)
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -D C99 -std=gnu99 
all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./tests

.PHONY: run
