# Written by Rohan Jain 2019095

section .text
    	global _add

_add:
	add 	rsi, rdi
	mov	rax, rsi
	ret