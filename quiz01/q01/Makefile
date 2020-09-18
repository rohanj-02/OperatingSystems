FILENAME_A := checkGreater

default: main

compileAssembly:
	nasm -f elf64 checkGreater.asm 	

# onlyAssembly:
# 	nasm -f elf64 $(FILENAME_A).asm
# 	gcc -o $(FILENAME_A) $(FILENAME_A).o -no-pie -nostdlib
# 	./$(FILENAME_A)

_start: compileAssembly
	@echo "Entry point is start in assembly"
	gcc -o p1 p1.c checkGreater.o --entry=_start -no-pie -nostdlib
	./p1
	@echo ""

main: compileAssembly
	@echo "Entry point is main"
	gcc -o p1 p1.c checkGreater.o --entry=main -no-pie -nostdlib
	./p1
	@echo ""

runAssembly: compileAssembly
	@echo "Running assembly code only "
	gcc -o checkGreater checkGreater.o -no-pie -nostdlib
	./checkGreater
	@echo ""