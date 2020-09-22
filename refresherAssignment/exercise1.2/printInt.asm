section .data
	negativeSign	db	'-' 		;to print negative sign in front of negative numbers

section .bss
	convertedInt	resb	64		;to store string representation of integer
	pointer    		resb	8		;to store the pointer to the end of convertedInt

section .text
	global _add

_add: 
	add 	rsi, rdi		;The values from C come in rdi, rsi
	cmp	rsi, 0		;If value of rsi negative then negate to get two's complement
	jl	_negative
	mov	rax, rsi
	call	_convert_int
	call	_prog_end

_convert_int: 
	call	_add_end_line
	call _int_loop

_negative:
	neg	rsi				;Negate rsi 
	push	rsi				;Push rsi(sum) on stack as syscall uses rsi 
	call	_print_negative_sign	;Print the negative sign
	pop	rax				;Store sum in rax
	call 	_convert_int
	call	_prog_end

_add_end_line: 
	mov 	rbx, convertedInt
	mov 	rcx, 10			;ASCII Code for new line
	mov	[rbx], rcx			;Save end line at base of convertedInt
	inc	rbx				;Increment pointer to character array because a character stored now
	mov	[pointer], rbx		;Save pointer value to address of convertedInt
	ret

_print_negative_sign:
	mov	rax, 1			;System call for stdout
	mov	rsi, negativeSign		;rsi stores the address of character array to print
	mov	rdx, 1			;negative sign has size 1
	mov	rdi, 1
	syscall				;system call to print to stdout
	ret

_int_loop:
	mov	rdx, 0		;In division RDX concatenated to RAX for dividend.. Dividend already in RAX
	mov 	rcx, 10		;Divisor 
	div	rcx			;RAX / RCX quotient in rax and remainder in rdx
	push 	rax
	add	rdx, 48		;Converting remainder to character
	; Not adding rax to stack just to see how this works . Does add , mov ,inc use rax or not ?
	call	_save_character
	pop	rax
	cmp	rax, 0
	jne	_int_loop		;If rax is not zero continue loop to store int	
	call _print_loop
	ret

_print_loop:
; //Changes over here
	mov 	rcx, [pointer]
	mov	rax, 1			;System cal for stdout
	mov	rdi, 1
	mov 	rsi, rcx			;Address of character is rsi
	mov	rdx, 1			;One byte space for character
	syscall	
	
	mov 	rcx, [pointer]
	dec 	rcx				;Decrease pointer to go to next character
	mov	[pointer], rcx		;Update pointer with decremented value
	cmp	rcx, convertedInt		;If updated pointer is greater than start of string then repeat
	jge 	_print_loop
	call 	_prog_end

_save_character:
	mov 	rcx, [pointer]		;Transfer contents of pointer to rcx
	mov	[rcx], dl			;Take last 8 bits of rdx (dl) and move to the address stored at rcx(Character is 8 bits)
	inc 	rcx				;Increment pointer to next position
	mov 	[pointer], rcx		;Move updated pointer (rcx) to pointer
	ret


_prog_end:
	mov 	rax, 60	;System call for program end 
	mov	rdi, 0	;Error code for program end
	syscall 		;End program with exit code 0






