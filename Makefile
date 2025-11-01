# Compiler & Flags
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: bin/inorder bin/preorder bin/postorder bin/calculate

bin/inorder: src/inorder.c
	$(CC) $(CFLAGS) src/inorder.c -o bin/inorder

bin/preorder: src/preorder.c
	$(CC) $(CFLAGS) src/preorder.c -o bin/preorder

bin/postorder: src/postorder.c
	$(CC) $(CFLAGS) src/postorder.c -o bin/postorder

bin/calculate: src/calculate.c
	$(CC) $(CFLAGS) src/calculate.c -o bin/calculate

clean:
	rm -f bin/inorder bin/preorder bin/postorder bin/calculate