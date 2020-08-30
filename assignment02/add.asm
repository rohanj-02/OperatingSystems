section .text
    	global _add

_add:
	add 	rsi, rdi
	mov	rax, rsi
	ret