main:
	; Move 0e or 10 to ah (ax high 8 bytes) to indicate teletype BIOS routine 
	mov	ah, 0x0e
	; Move ASCII code of character to print to al (ax lower 8 bytes)  
	mov	al, 'H'
	; Interrupt to print
	int	0x10
	mov	al, 'e'
	int	0x10
	mov	al, 'l'
	int	0x10
	int	0x10 ; 'l' is still on al, remember?
	mov	al, 'o'
	int	0x10
	mov	bx, the_secret
	add	bx, 0x7c00
	mov	al, [ bx ]
	int	0x10
	jmp	$

the_secret :
	db "X"

; Fill the binary till 510 bytes with 0s as placeholder after removing the size of the previous code
; $ represents the current address and $$ represents the start address
times	510-($-$$) db 0

; To make BIOS recognise this as a bootsector
dw	0xaa55