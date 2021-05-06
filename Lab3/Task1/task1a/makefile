all: exec

libs: c-lib

c-lib: task1a.c
	gcc -g -m32 -c -o task1a.o task1a.c

exec: task1a.c libs
	gcc -g -m32 task1a.o -o task1a
	rm task1a.o  
.PHONY: clean
clean:
	rm -rf *.o task1a
