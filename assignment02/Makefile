default:
	nasm -felf64 add.asm
	gcc prog-add.c add.o -no-pie
	
test: default
	./a.out 1 3
	./a.out -1 3
	./a.out 1 -3
	./a.out 3 -3
	./a.out -1 -1
	./a.out 0 3
	./a.out 0 -3
	./a.out 9223372036854775808 1
	