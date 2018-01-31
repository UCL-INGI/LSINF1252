#Tests unitaires pour bitwise-ops

#Copyright (C) 2016  Maxime Wattiaux
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc
OPT = -Werror -Wall

PATH1=$(HOME)/local/include
PATH2=$(HOME)/local/lib

all : .PHONY bitwiseops

bitwiseops : student_code.o tests.o
	$(CC) $(OPT) -L$(PATH2) student_code.o tests.o -lcunit -o bitwiseops
	./bitwiseops

student_code.o : student_code.c student_code.h
	$(CC) $(OPT) -c student_code.c

tests.o : tests.o student_code.h
	$(CC) $(OPT) -I$(PATH1) -lcunit -c tests.c
	
.PHONY : clean 

clean :
	rm -f *.o bitwiseops
