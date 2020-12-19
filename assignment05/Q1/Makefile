FILENAME = bootloader.asm
BINARYNAME = bootloader.bin

compile:
	nasm -f bin $(FILENAME) -o $(BINARYNAME)

run: compile
	qemu-system-x86_64 $(BINARYNAME)

clean:
	rm -f $(BINARYNAME)