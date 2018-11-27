CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

all: ifj2018

ifj2018: ifj2018.c parser.c scanner.c str.c symtable.c error.c code_generator.c expression.c
	$(CC) $(CFLAGS) ifj2018.c parser.c scanner.c str.c symtable.c error.c code_generator.c expression.c -o ifj2018

test_scanner: test_scanner.c scanner.c str.c
	$(CC) $(CFLAGS) test_scanner.c str.c -o test_scanner

clean:
	rm *.o parser scanner test_scanner ifj2018
