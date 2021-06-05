all: exec

libs: c-lib

c-lib: myELF.c 
	gcc -g -m32 -c -o myELF.o myELF.c

exec: libs
	gcc -g -m32 myELF.o -o myELF
	rm myELF.o
	
.PHONY: clean

clean:
	rm -rf *.o myELF 