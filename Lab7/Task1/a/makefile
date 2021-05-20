all: exec

libs: c-lib

c-lib: hexeditplus1a.c
	gcc -g -m32 -c -o hexeditplus1a.o hexeditplus1a.c

exec: hexeditplus1a.c libs
	gcc -g -m32 hexeditplus1a.o -o hexeditplus1a
	rm hexeditplus1a.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus1a