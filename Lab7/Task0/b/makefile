all: exec

libs: c-lib

c-lib: hexeditplus0.c
	gcc -g -m32 -c -o hexeditplus0.o hexeditplus0.c

exec: hexeditplus0.c libs
	gcc -g -m32 hexeditplus0.o -o hexeditplus0
	rm hexeditplus0.o  
	
.PHONY: clean

clean:
	rm -rf *.o hexeditplus0