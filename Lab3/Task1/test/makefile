all: exec

libs: c-lib

c-lib: test.c
	gcc -g -m32 -c -o test.o test.c

exec: test.c libs
	gcc -g -m32 test.o -o test
	rm test.o  
.PHONY: clean
clean:
	rm -rf *.o test
