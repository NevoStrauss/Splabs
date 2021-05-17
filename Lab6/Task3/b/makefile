all: exec

libs: c-lib

c-lib: LineParser.c task3b.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task3b.o task3b.c

exec: task3b.c libs
	gcc -g -m32 task3b.o -o task3b
	rm task3b.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task3b
