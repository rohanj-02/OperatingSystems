section .bss
	MEM_DIGIT resb 100			; Reserve bytes to store character representation of Integer
	MEM_DIGIT_CURR resb 8		;Reserve bytes to store address of current pointer to MEM_DIGIT
	NEGATIVE_SIGN resb 1
section .text
	global _add_two

_add_two:

	add rsi, rdi		;Add the two parameters
	mov rax, rsi		;Move the sum to rax
	call _decide_print			;Call print function to store strign version of integer and then print to stdout

_decide_print:
	cmp rax, 0
	jle _negate
	call _print

_negate:
	neg rax
	mov r8, rax
	mov rsi, NEGATIVE_SIGN			
	mov rbx, 45					
	mov [rsi], rbx
	mov rax, 1
	mov rdi, 1
	mov rdx, 1
	syscall
	mov rax, r8
	call _print

_prog_exit:				;System call for return with exit code 0
	mov rax, 60			
	mov rdi, 0
	syscall

_print:
	mov rcx, MEM_DIGIT			;Move the address of MEM_DIGIT to rcx
	mov rbx, 10					;10 is the ASCII code for a new line
	mov [rcx], rbx				;Move 10 to Address stored in rcx i.e. MEM_DIGIT
	inc rcx						;Increment rcx to go to next memory space
	mov [MEM_DIGIT_CURR], rcx	;Move rcx to MEM_DIGIT_CURR

_print_loop:
	mov rdx, 0			;While dividing value already present in rdx gets concatenated. Thats why better to set it to 0
	mov rbx, 10			;For division by 10
	div rbx				;Divide rax by rbx
	push rax			;Save rax to stack
	add rdx, 48			;rdx stores remainder, Convert remainder integer to ASCII code 

	mov rcx, [MEM_DIGIT_CURR]		;Save Address stored at MEM_DIGIT_CURR to rcx
	mov [rcx], dl					;Access lower 8 bits of rdx for character value stores
	inc rcx							;Increment rcx to go to next memory space
	mov [MEM_DIGIT_CURR], rcx		;Move rcx value to MEM_DIGIT_CURR
	
	pop rax							;Remove value from stack and add to rax
	cmp rax, 0						;If rax is 0 then print_loop_stdout otherwise repeat division
	jne _print_loop

_print_loop_stdout:
	mov rcx, [MEM_DIGIT_CURR]	

	mov rax, 1
	mov rdi, 1
	mov rsi, rcx
	mov rdx, 1
	syscall

	mov rcx, [MEM_DIGIT_CURR]
	dec rcx
	mov [MEM_DIGIT_CURR], rcx

	cmp rcx, MEM_DIGIT
	jge _print_loop_stdout

	call _prog_exit