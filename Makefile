CC = gcc

all:
	$(CC) -g src/main.c -o bin/main

run:
	bin/main

clean:
	rm -rf bin/*
