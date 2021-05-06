all: exec

libs: c-lib

c-lib: LineParser.c task0c.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task0c.o task0c.c

exec: task0c.c libs
	gcc -g -m32 task0c.o -o task0c
	rm task0c.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task0c
