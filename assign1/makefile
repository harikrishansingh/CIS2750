CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude/ -I/usr/include/libxml2/

parser: svgparser.o liblist.o
	$(CC) -shared bin/SVGParser.o bin/LinkedListAPI.o -o bin/libsvgparse.so

svgparser.o: src/SVGParser.c include/SVGParser.h
	$(CC) $(CFLAGS) -c src/SVGParser.c -fpic -o bin/SVGParser.o

liblist.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c src/LinkedListAPI.c -fpic -o bin/LinkedListAPI.o

test: parser
	$(CC) $(CFLAGS) bin/libsvgparse.so src/main.c -o bin/main -lxml2

clean:
	rm bin/*.o bin/*.so bin/main
