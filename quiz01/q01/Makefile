FILENAME_A := checkGreater
FILENAME_C := p1

default: main

compileAssembly:
	nasm -f elf64 $(FILENAME_A).asm 	

compileC:
	gcc -c $(FILENAME_C).c

_start: compileAssembly compileC
	@echo "Entry point is start in assembly"
	# gcc -o p1 p1.c checkGreater.o --entry=_start -no-pie -nostdlib
	ld $(FILENAME_C).o $(FILENAME_A).o -entry=_start -o startP1
	./startP1
	@echo ""

main: compileAssembly compileC
	@echo "Entry point is main"
	# gcc p1.c checkGreater.o --entry=main -no-pie -o p1
	ld $(FILENAME_C).o $(FILENAME_A).o -entry=main -o mainP1
	./mainP1
	@echo ""

clean:
	@echo "Cleaning up ..."
	rm $(FILENAME_A).o $(FILENAME_C).o startP1 mainP1