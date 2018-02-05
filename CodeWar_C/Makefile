all: build codewar.o

build: codewar.o
	gcc -o codewar codewar.o -Wall

codewar.o:
	gcc -c codewar.c -Wall

clean:
	rm -f codewar.o codewar
