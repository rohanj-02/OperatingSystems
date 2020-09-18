section .bss
	printVal	resb	8
section .text
	global checkGreater, _start
; x = 7, y = 2
checkGreater:
	cmp	rdi, rsi
	jle	retZero
	mov	rdx, 49
	mov	rcx, printVal
	mov	[rcx], dl
	jmp	print

_start:
	mov	rdi, 0
	mov	rsi, 1
	call	checkGreater
	jmp 	print
	;Print
print:
	mov	rax, 1
	mov	rdi, 1
	mov	rsi, printVal
	mov	rdx, 1
	syscall
	mov	rax, 60
	mov	rdi, 0
	syscall

retZero:
	mov	rdx, 48
	mov	rcx, printVal
	mov	[rcx], dl
	jmp	print
