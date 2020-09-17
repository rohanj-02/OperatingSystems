FILENAME_A := checkGreater

default:
	nasm -f elf64 $(FILENAME_A).asm
	gcc -o $(FILENAME_A) $(FILENAME_A).o -no-pie -nostdlib
	./$(FILENAME_A)
