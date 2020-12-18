loop:
    jmp loop 

; Fill the binary till 510 bytes as placeholder after removing the size of the previous code
times 510-($-$$) db 0

; To recognise this as a bootable partition
dw 0xaa55