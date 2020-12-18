FILENAME = bootloader.asm
BINARYNAME = bootloader.bin

compile:
	nasm -f bin $(FILENAME) -o $(BINARYNAME)

run:
	qemu-system-x86_64 $(BINARYNAME)