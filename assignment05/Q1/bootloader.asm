; Offsets every memory reference to this address
; This address is where the BIOS loads the boot sector.
[org 0x7c00]

main:
	; Interrupt to clear the screen
	; AL = lines to scroll (0 = clear),
	; BH = Background Color and Foreground color. 7h means white on black
	; CH = Upper row number, CL = Left column number, DH = Lower row number, DL = Right column number 
	mov ax, 0600h	
	mov cx, 0
	mov dl, 60h
	mov dh, 40h
	mov bh, 7h
	int 10h

	mov	bp, 8000h ; Initialise stack at 0x8000 address
	mov	sp, bp
	; Code to print hello world in 16-bit real mode
	; mov	bx, HELLO_WORLD
	; call	print_string
	call	protected_switch	; Switch to protected mode
	jmp $

%include "gdtload.asm"
%include "printing.asm"

[bits 16]
; Loads the gdt table and initialises cr0 register.
; Calls init_protected after finish
protected_switch:
	cli	; Clear interrupts because real mode IVT is useless after switching to protected mode
	lgdt	[gdt_descriptor]		; Load global decriptor table defined in gdtload.asm
	mov	eax, cr0	; Cannot directly change the value of cr0 register
	or	eax, 1	; Set first bit of CR0 register to 1 to switch to protected mode
	mov	cr0, eax
	jmp	CODE_SEG:init_protected	; Make far jump to clear all instructions preloaded in real mode 

[bits 32]	; Switch to 32 bit compilation
init_protected:
; Update segment registers to the protected mode definitiion of data segment
	mov	ax, DATA_SEG
	mov	ds, ax
	mov	ss, ax
	mov	fs, ax
	mov	es, ax
	mov	gs, ax

	mov	ebp, 90000h ; Update stack pointers to a freely available space
	mov	esp, ebp

	call	after_start

after_start:
	; Print hello world
	mov	ebx, HELLO_WORLD_PROTECTED
	mov	ecx, 0	; Print hello world to 1st row
	mov	eax, 0	; Print hello world to 1st column
	call	print_string_protected ; Call the print method 
	
	;Print "Cr0 contents:" 
	mov	ebx, CR0_MSG
	mov	ecx, 1	; Print to 2nd row
	mov	eax, 0	; Print to 1st column
	call	print_string_protected ; Call the print method 
	
	; Print the contents of Cr0
	mov	eax, cr0
	call	convert_binary_to_string	; Address to converted string stored in eax register
	mov	ebx, eax	; print_string_protected wants address to string in ebx so mov eax -> ebx
	mov	ecx, 1	; Print to row #1
	mov	eax, 14	; Print to column 14 because "Cr0 contents: " take 14 spaces
	call	print_string_protected

	jmp	$	; Do not exit, wait forever; Means jump to current address

; clear_screen:
; 	mov	ebx, CLEAR_SCREEN ; ASCII character for space followed by a 0
; 	mov	ecx, 1
; 	mov	eax, 0
; 	jmp	clear_screen_loop

; clear_screen_loop:
; 	; Nested loop to clear all rows and columns
; 	call	clear_screen_inner_loop
; 	add	ecx, 1
; 	cmp	ecx, 25
; 	jne	clear_screen_loop
; 	ret

; clear_screen_inner_loop:
; 	mov	ebx, CLEAR_SCREEN ; ASCII character for space followed by a 0
; 	call	print_string_protected
; 	add	eax, 1
; 	cmp	eax, 80
; 	jne	clear_screen_inner_loop
; 	ret

; HELLO_WORLD:
; 	db	"Hello World", 0

; CLEAR_SCREEN:
; 	db	" ", 0

HELLO_WORLD_PROTECTED:
	db	"Hello World", 0

CR0_MSG:
	db 	"Cr0 contents: ", 0

; Fill the binary till 510 bytes with 0s as placeholder after removing the size of the previous code
; $ represents the current address and $$ represents the start address
times	510-($-$$) db 0

; To make BIOS recognise this as a bootsector
dw	0xaa55