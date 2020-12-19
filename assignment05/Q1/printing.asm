[bits 16] ; Code to be compiled for 16 bit

; Printing in real mode
print_string: ; To call this, the string pointer should be in bx register.
	pusha		; Pushes all the registers on the stack

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
	popa		; Pops all the registers off the stack
	ret

[bits 32] ; Code to be compiled for 32 bit

VGA_MEMORY	equ	0xb8000	; Address at which VGA is stored
TEXT_COLOUR	equ	0x0f		; Text settings white on black for VGA

; VGA_MEMORY is stored like : 0xb8000 + 2 * (row * 80 + col)

print_string_protected:		; To call this, the string pointer must be in ebx register, and the row number to be kept in ecx register and column number on eax register
	pusha
	mov	esi, eax	; Save value of eax register to esi register because mul overwrites the contents of eax register
	mov	eax, 80 	; Todo row * 80
	mul	ecx		; eax -> row * 80 
	mov	edx, esi	; Move column number to edx register
	add	eax, edx	; eax -> row * 80 + col
	mov	ecx, eax	; ecx -> row * 80 + col
	mov	eax, 2	; To multiply ecx by 2
	mul	ecx		; eax -> 2 * (row * 80 + col)
	mov	edx, VGA_MEMORY
	add	edx, eax	; edx -> 0xb8000 + 2 * (row * 80 + col)
	mov	eax, 0
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

convert_binary_to_string:	; Requires the number to be printed to be stored in eax, returns the pointer to the binary in eax
	; Store the register values to be used on stack
	push	ebx
	push	ecx
	push	edx

	mov	ebx, 31		; 31 bytes because the string is initialised backwards. So storing starts from the back
					; ebx stores the offset from CONVERTED_BIN to access the binary value 

convert_binary_to_string_loop:
	mov	edx, 0	; Dividend already in eax
	mov	ecx, 2	; Divide ecx by 2 to get the Least significant bit (0 or 1)
	div	ecx	
	add	edx, 48	; Convert the number to its ASCII representation and store in edx

	; Save the character to CONVERTED_BIN
	mov	ecx, CONVERTED_BIN
	add	ecx, ebx		; Generate the address to save by adding offset to CONVERTED_BIN
	mov	[ecx], dl		; Save lower word of the edx register to address pointed by ecx
	
	; If the divisor value is 0 then end loop else subtract 1 from the offset and start again
	cmp	eax, 0
	je	convert_binary_to_string_exit
	sub	ebx, 1
	jmp	convert_binary_to_string_loop

convert_binary_to_string_exit:
	; Get the start of the converted binary string and store it in ebx
	add	ebx, CONVERTED_BIN
	; Return value in eax register so mov ebx -> eax
	mov	eax, ebx
	; Remove initial register values from stack
	pop	edx
	pop	ecx
	pop	ebx
	ret

CONVERTED_BIN:
	times	33 db 0	; Reserve 32 bytes of space for storing the binary string and 1 for null terminator 

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
	mov	ecx, 1
	mov	eax, 0
	call	print_string_protected
	popa
	ret

CONVERTED_INT:
	times	11 db 0	; Reserve 10 bytes of space for storing the integer and 1 for null terminator 
