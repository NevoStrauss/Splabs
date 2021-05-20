all: exec

libs:c-lib

c-lib: task4.c hexeditplus4.c
	gcc -g -m32 -fno-pie -c -o task4.o task4.c
	gcc -g -m32 -c -o hexeditplus4.o hexeditplus4.c

exec: libs
	gcc -g -m32 -fno-pie -c -o task4.o task4.c
	gcc -g -m32 -fno-pie  task4.o -o task4
	gcc -g -m32 hexeditplus4.o -o hexeditplus4
	rm task4.o hexeditplus4.o

.PHONY: clean

clean:
	rm -rf ./*.o task4 hexeditplus4