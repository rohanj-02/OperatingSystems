    global add_two
    section .text
add_two: 
    add     rsi, rdi
    mov     rax, 1
    mov     rdi, 1
    mov     rcx, rsi
    mov     rsi, rcx
    mov     rdx, 4
    syscall
    mov     rax, 60         
    xor     rdi, rdi                
    syscall  