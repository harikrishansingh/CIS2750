CC = gcc
CFLAGS = -Wall -std=c11 -g

liblist: liblist.o
	$(CC) -shared bin/LinkedListAPI.o -o bin/liblist.so

liblist.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c src/LinkedListAPI.c -fpic -Iinclude/ -o bin/LinkedListAPI.o

clean:
	rm bin/*.o bin/*.so
