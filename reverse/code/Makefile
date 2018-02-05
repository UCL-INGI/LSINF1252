CC=gcc
EXEC=tests
LDFLAGS=-lcunit -L/opt/local/lib -Wl,-wrap=open -Wl,-wrap=mmap -Wl,-wrap=munmap -Wl,-wrap=close -Wl,-wrap=read -Wl,-wrap=pread -Wl,-wrap=fopen
SRC=$(wildcard code/*.c)
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
