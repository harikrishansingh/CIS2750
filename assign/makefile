CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude/ -I/usr/include/libxml2/

test: liblist svgparser
	$(CC) $(CFLAGS) bin/LinkedListAPI.so bin/SVGParser.so src/main.c -o bin/main -lxml2 

liblist: liblist.o
	$(CC) -shared bin/LinkedListAPI.o -o bin/LinkedListAPI.so

liblist.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c src/LinkedListAPI.c -fpic -o bin/LinkedListAPI.o

svgparser: svgparser.o
	$(CC) -shared bin/SVGParser.o -o bin/SVGParser.so

svgparser.o: src/SVGParser.c include/SVGParser.h
	$(CC) $(CFLAGS) -c src/SVGParser.c -fpic -o bin/SVGParser.o

clean:
	rm bin/*.o bin/*.so
