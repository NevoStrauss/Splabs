all: exec

libs: c-lib

c-lib: hexeditplus3a.c
	gcc -g -m32 -c -o hexeditplus3a.o hexeditplus3a.c

exec: hexeditplus3a.c libs
	gcc -g -m32 hexeditplus3a.o -o hexeditplus3a
	rm hexeditplus3a.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus3a