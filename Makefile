CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror -g

scanner: scanner.c
	$(CC) $(CFLAGS) scanner.c -o scanner

