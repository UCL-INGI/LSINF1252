CC=gcc
EXEC=tests
LDFLAGS=-lcunit -lm -lpthread
SRC=$(wildcard *.c) libwrap/wrap_mutex.c libwrap/wrap_malloc.c libwrap/wrap_file.c
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Werror -DC99 -std=gnu99 -Ilibwrap
WRAP=-Wl,-wrap=pthread_mutex_lock -Wl,-wrap=pthread_mutex_unlock -Wl,-wrap=pthread_mutex_trylock -Wl,-wrap=pthread_mutex_init -Wl,-wrap=pthread_mutex_destroy -Wl,-wrap=malloc -Wl,-wrap=free -Wl,-wrap=calloc -Wl,-wrap=open -Wl,-wrap=creat -Wl,-wrap=close -Wl,-wrap=read -Wl,-wrap=write -Wl,-wrap=stat -Wl,-wrap=fstat -Wl,-wrap=lseek -Wl,-wrap=exit

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(EXEC): $(OBJ)
	$(CC) $(WRAP) -o $@ $(OBJ) $(LDFLAGS)

.PHONY: tests
