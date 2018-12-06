CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra

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

zip:
	zip "xsasak01.zip" "code_generator.c" "code_generator.h" "error.c" "error.h" "expression.c" "expression.h" "ifj2018.c" "Makefile" "parser.c" "parser.h" "scanner.c" "scanner.h" "stack.c" "stack.h" "str.c" "str.h" "symtable.c" "symtable.h" "dokumentace.pdf" "rozdeleni"

clean:
	rm *.o parser scanner test_scanner ifj2018
