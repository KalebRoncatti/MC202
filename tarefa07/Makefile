CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: mensageiro

mensageiro: mensageiro.c
	gcc $(CFLAGS) *.c -o mensageiro $(LDFLAGS)

clean:
	rm -f mensageiro

test: mensageiro
	./mensageiro < testes/mensageiro2.in

valgrind_test: mensageiro
	valgrind --leak-check=full -s ./mensageiro < testes/mensageiro3.in

