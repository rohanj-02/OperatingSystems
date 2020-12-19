; Offsets every memory reference to this address
; This address is where the BIOS loads the boot sector.
[org 0x7c00]

main:
	mov	bp, 0x8000 ; Initialise stack at 0x8000 address
	mov	sp, bp
	mov	bx, HELLO_WORLD
	call	print_string
	call	protected_switch
	jmp $

%include "gdtload.asm"
%include "printing.asm"

[bits 16]
; Loads the gdt table and initialises cr0 register.
; Calls init_protected after finish
protected_switch:
	cli	;Clear interrupts because real mode IVT is useless after switching to protected mode
	lgdt	[gdt_descriptor]
	mov	eax, cr0
	or	eax, 0x1	; Set first bit of CR0 register to 1 to switch to protected mode
	mov	cr0, eax
	jmp	CODE_SEG:init_protected	; Make far jump to clear all instructions preloaded in real mode 

[bits 32]
init_protected:
	mov	ax, DATA_SEG ; Update segment registers to the protected mode definitiion of data segment
	mov	ds, ax
	mov	ss, ax
	mov	fs, ax
	mov	es, ax
	mov	gs, ax

	mov	ebp, 0x90000 ; Update stack pointers to a freely available space
	mov	esp, ebp

	call	after_start

after_start:
	mov	ebx, HELLO_WORLD_PROTECTED
	call	print_string_protected ; Call the print method 
	mov	eax, 100100b
	call	convert_int_to_string
	jmp	$	; Do not exit

HELLO_WORLD:
	db	"Hello World", 0

HELLO_WORLD_PROTECTED:
	db	"Hello Protected World", 0
; Fill the binary till 510 bytes with 0s as placeholder after removing the size of the previous code
; $ represents the current address and $$ represents the start address
times	510-($-$$) db 0

; To make BIOS recognise this as a bootsector
dw	0xaa55