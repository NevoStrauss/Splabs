all: exec

libs: asm-lib c-lib

asm-lib: add.s
	# flags meanings:
	# -g = Add debug information.
	# -f elf = Specifies the output file format -> output format (ELF 32 bit).
	# -o add.o = write assembled file to add.o
	nasm -g -f elf -o add.o add.s

c-lib: numbers.c
	# flags meanings:
	# -g = Add debug information.
	# -m32 = Specifies the output file format to be 32bit.
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o numbers.o numbers.c

exec: main.c libs
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32  main.o add.o numbers.o -o main
	rm main.o add.o numbers.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf ./*.o main
