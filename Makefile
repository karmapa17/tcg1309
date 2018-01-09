CC = gcc

all:
	$(CC) -g src/main.c -o bin/main

clean:
	rm -rf bin/*
