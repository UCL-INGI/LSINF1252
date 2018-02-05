#  Copyright (c) 2015, 2016 Bonaventure Olivier & Massart Aurélie & Hautecoeur Cécile
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CC=gcc
EXEC=tests
LDFLAGS=-L$(HOME)/local/lib -lpthread -lcunit -Wl,-wrap=malloc -Wl,-wrap=free
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
