all: exec

libs: c-lib

c-lib: task1c.c
	gcc -g -m32 -c -o task1c.o task1c.c

exec: task1c.c libs
	gcc -g -m32 task1c.o -o task1c
	rm task1c.o  
.PHONY: clean
clean:
	rm -rf *.o task1c
