CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

ALL: str.c scanner.c
	$(CC) $(CFLAGS) str.c scanner.c -o scanner


