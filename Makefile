CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

all: test_scanner 

test_scanner: test_scanner.o
	$(CC) $(CFLAGS) test_scanner.o str.o -o test_scanner

test_scanner.o: test_scanner.c str.c str.h scanner.c scanner.h 
	$(CC) $(CFLAGS) -c test_scanner.c -o test_scanner.o