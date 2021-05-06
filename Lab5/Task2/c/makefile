all: exec

libs: c-lib

c-lib: LineParser.c task2c.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task2c.o task2c.c

exec: task2c.c libs
	gcc -g -m32 task2c.o -o task2c
	rm task2c.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o task2c
