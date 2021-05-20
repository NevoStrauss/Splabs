all: exec

libs: c-lib

c-lib: hexeditplus1c.c
	gcc -g -m32 -c -o hexeditplus1c.o hexeditplus1c.c

exec: hexeditplus1c.c libs
	gcc -g -m32 hexeditplus1c.o -o hexeditplus1c
	rm hexeditplus1c.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus1c