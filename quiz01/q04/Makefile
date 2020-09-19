default:
	nasm -f elf32 P4_1.asm
	gcc -m32 -o p4_1 P4_1.c P4_1.o
	./p4_1

default64:
	nasm -f elf64 P4_2.asm
	gcc -o p4_2 P4_2.c P4_2.o
	./p4_2

clean: 
	@echo " Cleaning up ... " 
	rm p4_1 p4_2 P4_1.o P4_2.o