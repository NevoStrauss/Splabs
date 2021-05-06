all: exec

libs: c-lib

c-lib: LineParser.c looper.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o looper.o looper.c

exec: looper.c libs
	gcc -g -m32 looper.o -o looper
	rm looper.o LineParser.o

.PHONY: clean

clean:
	rm -rf *.o looper
