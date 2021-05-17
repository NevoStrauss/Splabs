all: exec

libs: c-lib

c-lib: LineParser.c task0b.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task0b.o task0b.c

exec: task0b.c libs
	gcc -g -m32 task0b.o -o task0b
	rm task0b.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task0b
