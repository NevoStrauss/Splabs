all: exec

libs: c-lib

c-lib: hexeditplus1d.c
	gcc -g -m32 -c -o hexeditplus1d.o hexeditplus1d.c

exec: hexeditplus1d.c libs
	gcc -g -m32 hexeditplus1d.o -o hexeditplus1d
	rm hexeditplus1d.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus1d