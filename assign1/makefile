CC = gcc
CFLAGS = -Wall -std=c11 -g
INCLUDES = -Iinclude/ -I/usr/include/libxml2/

parser: svgparser.o liblist.o
	$(CC) -shared bin/SVGParser.o bin/LinkedListAPI.o -o bin/libsvgparse.so

svgparser.o: src/SVGParser.c include/SVGParser.h
	$(CC) $(CFLAGS) $(INCLUDES) -fpic -c src/SVGParser.c -o bin/SVGParser.o

liblist.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) $(INCLUDES) -fpic -c src/LinkedListAPI.c -o bin/LinkedListAPI.o

test: parser
	mv bin/libsvgparse.so ./
	$(CC) $(CFLAGS) $(INCLUDES) libsvgparse.so src/main.c -o bin/test -lxml2
	mv libsvgparse.so bin/

clean:
	rm bin/*.o bin/*.so bin/main
