all: exec

libs: c-lib

c-lib: LineParser.c task2a.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task2a.o task2a.c

exec: task2a.c libs
	gcc -g -m32 task2a.o -o task2a
	rm task2a.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task2a
