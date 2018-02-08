OBJ = calc.o operations.o

calc: $(OBJ)
	cc -o calc $(OBJ)

@@make_calc@@

.PHONY: clean mrproper

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -f calc

# No implicit rules
.SUFFIXES:
