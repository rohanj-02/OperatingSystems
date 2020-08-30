FILENAME_C := prog-add
FILENAME_A := add

all: default

FILENAME_C.o : 
	@echo "Converting C file to obj file"
	gcc -c $(FILENAME_C).c

FILENAME_A.o :
	@echo "Converting assembly file to obj file"
	nasm -f elf64 $(FILENAME_A).asm

default: FILENAME_A.o FILENAME_C.o
	gcc -o $(FILENAME_C) $(FILENAME_A).o $(FILENAME_C).o -no-pie
	@echo "Process Completed!"
	@echo "To run the program, execute: ./prog-add"

clean:
	@echo "Cleaning up"
	rm $(FILENAME_A).o $(FILENAME_C).o $(FILENAME_C)
