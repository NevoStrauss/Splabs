all: exec

libs: c-lib

c-lib: LineParser.c task0a.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task0a.o task0a.c

exec: task0a.c libs
	gcc -g -m32 task0a.o -o task0a
	rm task0a.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task0a
