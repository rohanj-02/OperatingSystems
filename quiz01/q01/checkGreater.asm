section .bss
	printVal	resb	8			;For 0 and 1 to print

section .text
	global checkGreater, _start

checkGreater:
	cmp	rdi, rsi
	jle	retZero		;Jump tp return Zero if rdi <= rsi
	mov	rdx, 49
	mov	rcx, printVal
	mov	[rcx], dl		;Transfer 49 (ASCII for 1) to printVal
	jmp	print

_start:
	mov	rdi, 29		;Parameter 1 : x
	mov	rsi, 102		;Parameter 2 : y
	jmp	checkGreater
	
print:
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, printVal
	mov	rdx, 1
	syscall		;System call for writing to stdout
	mov	rax, 60
	mov	rdi, 0
	syscall		;System call for exit

retZero:
	mov	rdx, 48
	mov	rcx, printVal
	mov	[rcx], dl		;Transfer 48(ASCII value for 0) to printVal
	jmp	print
