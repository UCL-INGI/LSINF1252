# CC variable for C compiler
# You can replace gcc by clang if you prefer
#CC = gcc
# CFLAGS variable for gcc/clang flags
# See gcc/clang manual to understand all flags
#CFLAGS += -std=c99 # Define which version of the C standard to use
#CFLAGS += -Wall # Enable the 'all' set of warnings
#CFLAGS += -Werror # Treat all warnings as error
#CFLAGS += -Wshadow # Warn when shadowing variables
#CFLAGS += -Wextra # Enable additional warnings
#CFLAGS += -O2 -D_FORTIFY_SOURCE=2 # Add canary code, i.e. detect buffer overflows
#CFLAGS += -fstack-protector-all # Add canary code to detect stack smashing

OBJ = calc.o operations.o

calc: $(OBJ)

calc.o: calc.c calc.h operations.h

operations.o: operations.c operations.h

.PHONY: clean mrproper

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f calc
