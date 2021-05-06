output: encoder.o
	gcc -g -m32 -Wall -o encoder encoder.c

encoder.o: encoder.c
	gcc -g -m32 -Wall -c -o encoder.o encoder.c

.PHONY: clean

clean:
	rm *.o encoder