output: hexaPrint.o
	gcc -g -m32 -Wall -o hexaPrint hexaPrint.o

hexaPrint.o: hexaPrint.c
	gcc -g -m32 -Wall -c -o hexaPrint.o hexaPrint.c

.PHONY: clean

clean:
	rm *.o hexaPrint