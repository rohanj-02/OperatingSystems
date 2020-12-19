[bits 16] ; Code to be compiled for 16 bit

print_string: ; To call this, the string pointer should be in bx register.
	pusha

print_string_loop:
	; Move ASCII code of character to print to al (ax lower 8 bytes)  
	mov	al, [bx]
	; if character to print == 0, i.e., exit function
	cmp	al, 0
	je	print_string_exit
	call	print_char
	; Increment string pointer
	add	bx, 1
	jmp	print_string_loop

; To call this function, simply set al to the character to print
print_char: 
	; Move 0e or 10 to ah (ax high 8 bytes) to indicate teletype BIOS routine 
	mov	ah, 0x0e
	; Interrupt to print
	int	0x10
	ret

print_string_exit:
	popa
	ret

[bits 32] ; Code to be compiled for 32 bit

VGA_MEMORY	equ	0xb8000	; Address at which VGA is stored
TEXT_COLOUR	equ	0x0f		; Text settings white on black for VGA

print_string_protected:		; To call this, the string pointer must be in ebx register
	pusha
	mov	edx, VGA_MEMORY
	mov	ah, TEXT_COLOUR	; Upper byte of ax should be the font specifications and lower 8 bytes the ASCII code to be printed

print_string_protected_loop:
	; Move ASCII code of character to print to al (ax lower 8 bytes)  
	mov	al, [ebx]
	; if character to print == 0, i.e., exit function
	cmp	al, 0
	je	print_string_protected_exit
	mov	[edx], ax
	; Increment string pointer by 1 and VGA pointer by 2 because each character is represented as 2 bytes
	add	ebx, 1
	add	edx, 2
	jmp 	print_string_protected_loop

print_string_protected_exit:
	popa
	ret

convert_int_to_string:
	pusha
	mov	ebx, 9

convert_int_to_string_loop:
; We store the offset to the pointer to CONVERTED_INT in ebx
	mov	edx, 0	; Dividend already in eax
	mov	ecx, 10
	div	ecx
	add	edx, 48

	; Save the character to CONVERTED_INT
	mov	ecx, CONVERTED_INT
	add	ecx, ebx
	mov	[ecx], dl
	; If the integer value is 0 then end loop
	cmp	eax, 0
	je	convert_int_to_string_exit
	sub	ebx, 1
	jmp	convert_int_to_string_loop

convert_int_to_string_exit:
	add	ebx, CONVERTED_INT
	call	print_string_protected
	popa
	ret

CONVERTED_INT:
	times	11 db 0	; Reserve 10 bytes of space for storing the integer and 1 for null terminator 
