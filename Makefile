CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

all: 

parser:  parser.c scanner.c str.c symtable.c
	$(CC) $(CFLAGS) parser.c scanner.c str.c symtable.c -o parser

scanner: scanner.c str.c
	$(CC) $(CFLAGS) scanner.c str.c -o scanner

test_scanner: test_scanner.c scanner.c str.c
	$(CC) $(CFLAGS) test_scanner.c str.c -o test_scanner

clean:
	rm *.o parser scanner test_scanner