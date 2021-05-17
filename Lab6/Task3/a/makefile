all: exec

libs: c-lib

c-lib: LineParser.c task3a.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task3a.o task3a.c

exec: task3a.c libs
	gcc -g -m32 task3a.o -o task3a
	rm task3a.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task3a
