CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: listas_legais

listas_legais: *.c
	gcc $(CFLAGS) *.c -o listas_legais $(LDFLAGS)

clean:
	rm -f listas_legais
	rm -f *.o

test: listas_legais
	./listas_legais < testes/listas_legais9.in

test_valgrind: listas_legais
	valgrind --leak-check=full -s ./listas_legais < testes/listas_legais10.in

new_test: listas_legais
	./listas_legais < teste.in