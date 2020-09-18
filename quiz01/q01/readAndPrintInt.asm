section .bss           ;Uninitialized data
	num resb 5
	
section .text
	global _start

_start:
	mov	rax, 3
	mov	rbx, 2
	mov	rcx, num  
	mov	rdx, 5
	int	0x80

	mov	rax, 4
	mov	rbx, 1
	mov	rcx, num
	mov	rdx, 5
	int	0x80
	mov 	rax, 60	;System call for program end 
	mov	rdi, 0	;Error code for program end
	syscall 		;End program with exit code 0