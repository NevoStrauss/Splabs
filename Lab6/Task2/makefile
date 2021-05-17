all: exec

libs: c-lib

c-lib: LineParser.c task2.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task2.o task2.c

exec: task2.c libs
	gcc -g -m32 task2.o -o task2
	rm task2.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task2
