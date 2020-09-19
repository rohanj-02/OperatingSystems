section .text
	global long_add
;32 bit mode 
long_add:
	push	ebp
	mov	ebp, esp

	mov	eax, [ebp + 20]
	add	eax, [ebp + 16]		;add 2 numbers 
	add	eax, [ebp + 12]		;add 2 numbers
	add	eax, [ebp + 8]		;add 2 numbers

	mov	esp, ebp
	pop	ebp
	ret