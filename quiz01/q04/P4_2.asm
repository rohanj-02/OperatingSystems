section .text
	global extended_add
;64 bit mode 
extended_add:
	mov	rax, rdi
	add	rax, rsi
	add	rax, rdx
	add	rax, rcx
	ret