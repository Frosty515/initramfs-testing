[bits 64]

global fast_memset
fast_memset: ; rdi=address, rsi=value, rdx=num
    mov rax, rdi ; prepare return value

    test rdx, rdx
    jz .end
    
    ; round rdx up to nearest multiple of 8
    add rdx, 7
    shr rdx, 3
    shl rdx, 3

    xor rcx, rcx ; init counter
.l:
    mov [rdi+rcx], rsi ; doing the setting
    add rcx, 8 ; increment counter
    cmp rcx, rdx ; check if we are over the limit
    jb .l ; restart loop

.end:
    ret

global fast_memcpy
fast_memcpy: ; rdi=dest, rsi=src, rdx=num
    mov rax, rdi ; prepare return value

    test rdx, rdx
    jz .end
    
    ; round rdx up to nearest multiple of 8
    add rdx, 7
    shr rdx, 3
    shl rdx, 3

    xor rcx, rcx ; init counter
.l:
    mov r8, QWORD [rsi+rcx] ; due to weird limitations we can`t move between 2 memory locations
    mov QWORD [rdi+rcx], r8 ; doing the setting
    add rcx, 8 ; increment counter
    cmp rcx, rdx ; check if we are over the limit
    jb .l ; restart loop

.end:
    ret
