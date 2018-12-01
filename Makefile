CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

all: ifj2018

ifj2018: ifj2018.c parser.c scanner.c str.c stack.c symtable.c error.c code_generator.c expression.c
	$(CC) $(CFLAGS) ifj2018.c parser.c scanner.c str.c stack.c symtable.c error.c code_generator.c expression.c -o ifj2018

test_scanner: tests/test_scanner.c scanner.c str.c
	$(CC) $(CFLAGS) tests/test_scanner.c str.c scanner.c error.c -o tests/test_scanner

test: test_scanner
	@echo "\nRUNNING SCANNER TESTS:\n"
	@./tests/test_scanner
	@echo "\nRUNNING SEMANTIC AND SYNTACTIC TESTS:\n"
	@bash tests/test_compiler.sh
	@rm tests/test_scanner
clean:
	rm *.o parser scanner test_scanner ifj2018
