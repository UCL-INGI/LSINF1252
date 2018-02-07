OBJ = calc.o operations.o

@@make_calc_simple@@

calc.o: calc.c calc.h operations.h
	$(CC) -c $^

operations.o: operations.c operations.h
	$(CC) -c $^

.PHONY: clean mrproper

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f calc

# No implicit rules
.SUFFIXES:
