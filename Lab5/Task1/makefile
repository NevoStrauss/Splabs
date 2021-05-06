all: exec

libs: c-lib

c-lib: LineParser.c task1.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task1.o task1.c

exec: task1.c libs
	gcc -g -m32 task1.o -o task1
	rm task1.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task1
