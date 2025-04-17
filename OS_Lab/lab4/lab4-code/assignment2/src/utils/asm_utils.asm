[bits 32]

global asm_hello_world

asm_hello_world:
    push eax
    xor eax, eax

    mov ah, 0x70 ;白底黑字
    mov al, 'y'
    mov [gs:2 * 0], ax

    mov al, 'p'
    mov [gs:2 * 1], ax

    mov al, 'k'
    mov [gs:2 * 2], ax

    mov al, ' '
    mov [gs:2 * 3], ax

    mov al, '2'
    mov [gs:2 * 4], ax

    mov al, '3'
    mov [gs:2 * 5], ax

    mov al, '3'
    mov [gs:2 * 6], ax

    mov al, '3'
    mov [gs:2 * 7], ax

    mov al, '6'
    mov [gs:2 * 8], ax

    mov al, '2'
    mov [gs:2 * 9], ax

    mov al, '7'
    mov [gs:2 * 10], ax

    mov al, '9'
    mov [gs:2 * 11], ax

    pop eax
    ret
