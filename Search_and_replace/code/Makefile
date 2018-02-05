CC=gcc
EXEC=tests
LDFLAGS=-lcunit -Wl,-wrap=open -Wl,-wrap=close -Wl,-wrap=read -Wl,-wrap=write -Wl,-wrap=malloc -Wl,-wrap=free    

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
