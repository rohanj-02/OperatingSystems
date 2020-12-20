; db-> 1 byte; dw -> 1 word 2 bytes; dd -> 2 words 4 bytes
gdt_init:

gdt_null_descriptor:
	; The compulsory null descriptor to denote start of GDT(8 bytes)
	dd	0x0
	dd	0x0

; Describing Intel's basic flat model
;Flag Meaning:
;Descriptor type: 1 for code or data and 0 for traps
;Privilege: 0 is highest priviilege
;Present: 1 for is Segment present in memory
;Code: 1 for code
;Conforming: 0 for lower privilege cannot call code here
;Readable: 1 if segment is readible and 0 if execute only
;Accessed: Used for debugging
;Granularity: 1 multiplies limit by 16*16*16 = 4K 
;32-bit default: 1 if 32 bit code in the segment
;64-bit default: 0 because we are on 32 bit machine
;AVL: Used for debugging
;
;Memory Layout in Segment Descriptor: (8 bytes data structure)
;Base(31-24)
;Granularity bit
;32-bit default bit
;64-bit default bit
;AVL bit
;Limit(19-16)
;Present bit
;Privilege level 2 bits
;Descriptor type bit
;Segment type 4 bits: Code, conforming, readable, accessed
;Base (23-16)
;Base (15-0)
;Limit(15-0)

; To describe the code segment
gdt_code_decriptor:
;Base = 0, Limit = fffffh, Flags: 1 for present, 00 for privilege, 1 for descriptor type -> 1001b
;Flags for type: 1 for code, 0 for conforming, 1 for readable, 0 for accessed -> 1010b
;More flags: 1 for granularity, 1 for 32-bit default, 0 for 64-bit default, 0 for AVL -> 1100
	dw	0xffff	;Limit (0-15)
	dw	0h		;Base (0-15)
	db	0h		;Base (16-23)
	db	10011010b	;Flags, Flags for type
	db	11001111b	;More flags and Limit (16-19)
	db	0h		;Base (24-31)

gdt_data_decriptor:
; Flags for type changed, rest is same
; Flags for type: 0 for code, 0 for expand down, 1 for writable, 0 for accessed ->  0010
	dw	0xffff	;Limit (0-15)
	dw	0h		;Base (0-15)
	db	0h		;Base (16-23)
	db	10010010b	;Flags, Flags for type
	db	11001111b	;More flags and Limit (16-19)
	db	0h		;Base (24-31

gdt_finish: ; To calculate size of the gdt

gdt_descriptor: ; 6 byte data structure with size of gdt in the first 2 bytes and the start address of the GDT in the last 4 bytes
	dw	gdt_finish - gdt_init - 1 ; Size of the gdt table
	dd	gdt_init

; MACROS for easy access
CODE_SEG	equ gdt_code_decriptor - gdt_init
DATA_SEG 	equ gdt_data_decriptor - gdt_init