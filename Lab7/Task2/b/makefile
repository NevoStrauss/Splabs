all: exec

libs: c-lib

c-lib: hexeditplus2b.c
	gcc -g -m32 -c -o hexeditplus2b.o hexeditplus2b.c

exec: hexeditplus2b.c libs
	gcc -g -m32 hexeditplus2b.o -o hexeditplus2b
	rm hexeditplus2b.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus2b