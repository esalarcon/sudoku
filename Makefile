CC = gcc
CFLAGS = -I. -Wall
DEPS = pila.h
OBJ = pila.c sudoku.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sudoku: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o sudoku
