all: exec

libs: c-lib

c-lib: LineParser.c task0d.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task0d.o task0d.c

exec: task0d.c libs
	gcc -g -m32 task0d.o -o task0d
	rm task0d.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task0d
