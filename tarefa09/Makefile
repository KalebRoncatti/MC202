CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: diadeuber

diadeuber: *.c
	gcc $(CFLAGS) *.c -o diadeuber $(LDFLAGS)

clean:
	rm -f diadeuber
	rm -f *.o

test: diadeuber
	./diadeuber < testes/diadeuber1.in


test_valgrind: diadeuber
	valgrind --leak-check=full -s ./diadeuber < testes/diadeuber10.in
