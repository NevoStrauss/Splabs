all: exec

libs: asm-lib c-lib

asm-lib: start.s
	nasm -f elf32 start.s -o start.o

c-lib: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: task2b.c libs   
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2b.c -o task2b.o
	ld -m elf_i386 start.o task2b.o util.o -o task2b
	rm task2b.o start.o util.o

.PHONY: clean

clean:
	rm -rf ./*.o task2b
