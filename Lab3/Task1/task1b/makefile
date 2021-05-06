all: exec

libs: c-lib

c-lib: task1b.c
	gcc -g -m32 -c -o task1b.o task1b.c

exec: task1b.c libs
	gcc -g -m32 task1b.o -o task1b
	rm task1b.o  
.PHONY: clean
clean:
	rm -rf *.o task1b