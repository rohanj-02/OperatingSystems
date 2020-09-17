section .text
	global long_add
;32 bit mode 
long_add:
	push	ebp
	mov	ebp, esp
	push	ebx		; Save ebx register to stack

	mov	eax, [ebp + 20]
	mov	ebx, [ebp + 16]
	add	eax, ebx		;add 2 numbers
	mov	ebx, [ebp + 12]
	add	eax, ebx		;add 2 numbers
	mov	ebx, [ebp + 8]
	add	eax, ebx		;add 2 numbers

	mov	ebx, [ebp - 4]
	mov	esp, ebp
	pop	ebp
	ret