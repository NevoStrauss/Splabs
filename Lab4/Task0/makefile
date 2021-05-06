all: exec

libs: asm-lib c-lib

asm-lib: start.s
	nasm -f elf32 start.s -o start.o

c-lib: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: main.c libs   
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
	ld -m elf_i386 start.o main.o util.o -o task0
	rm main.o start.o util.o

.PHONY: clean

clean:
	rm -rf ./*.o task0
