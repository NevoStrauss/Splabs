all: exec

libs: c-lib

c-lib: hexeditplus1b.c
	gcc -g -m32 -c -o hexeditplus1b.o hexeditplus1b.c

exec: hexeditplus1b.c libs
	gcc -g -m32 hexeditplus1b.o -o hexeditplus1b
	rm hexeditplus1b.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus1b