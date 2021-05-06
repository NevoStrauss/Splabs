all: exec

libs: c-lib

c-lib: LineParser.c task2b.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task2b.o task2b.c

exec: task2b.c libs
	gcc -g -m32 task2b.o -o task2b
	rm task2b.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task2b
