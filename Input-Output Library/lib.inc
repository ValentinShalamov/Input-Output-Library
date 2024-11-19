section .data
    buffer: times 255 db 0

section .text

string_length:
    xor rax, rax
    .loop:
    cmp byte [rdi+rax], 0
    jz .end
    inc rax
    jmp .loop
    .end:
    ret

print_string:
    push rdi
    call string_length
    pop rsi
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret


print_char:
    push rdi
    mov rdi, rsp
    call print_string
    pop rdi
    ret

print_newline:
    call print_char
    ret


print_uint:
    test rdi, rdi
    jz .print_zero
    xor r8, r8
    xor rdx, rdx
    mov rcx, 10
    mov rax, rdi
.del:
    div rcx
.check:
    test rax, rax
    jnz .loop
    test rdx, rdx
    jz .print
.loop:
    or rdx, byte 0x30
    push rdx
    xor rdx, rdx
    add r8, 1
    jmp .del 
.print:
    pop r10
    mov rdi, r10
    push r8
    call print_char
    pop r8
    sub r8, 1
    test r8, r8
    jnz .print
    jmp .end
.print_zero:
    or rdi, byte 0x30
    call print_char
.end:
    ret

print_int:
    test rdi, rdi
    js .pr_sign
    call print_uint
    ret
.pr_sign:
    neg rdi
    push rdi
    mov rdi, 0x2D
    call print_char
    pop rdi
    call print_uint
    ret


read_char:
    mov rax, 0
    mov rdi, 0
    push rax
    mov rsi, rsp
    mov rdx, 1
    syscall
    cmp rax, -1
    jnz .char
.zero:
    xor rax, rax
    ret
.char:
    pop rax
    ret 

read_word:
.call:
    push rdi
    push rsi
    call read_char
    pop rsi
    pop rdi
    cmp al, 0x20
    je .call
    cmp al, 0x09
    je .call
    cmp al, 0xA
    je .call
    xor rcx, rcx
.loop:
    cmp al, 0x20
    je .end
    cmp al, 0x09
    je .end
    cmp al, 0xA
    je .end
    cmp al, 0
    je .end
    mov byte [buffer + rcx], al
    inc rcx
    cmp rsi, rcx
    jz .overflow
    push rdi
    push rsi
    push rcx
    call read_char
    pop rcx
    pop rsi
    pop rdi
    jmp .loop
.end:
    mov rax, buffer
    mov rdx, rcx
    ret
.overflow:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length

parse_uint:
    xor rcx, rcx
    xor rax, rax
    xor r8, r8
    mov r9, 10
    test rdi, rdi
    jz .end
.loop:
    cmp byte [rdi+rcx], 0x39
    ja .end
    cmp byte [rdi+rcx], 0x30
    jb .end
    mov r8b, byte [rdi+rcx]
    and r8, 0xF
    inc rcx
    mul r9
    add rax, r8
    jmp .loop
.end:
    mov rdx, rcx
    ret


; rdi points to a string
; returns rax: number, rdx : length

parse_int:
    cmp byte[rdi], '-'
    jne .p_uint
    lea rdi, [rdi + 1]
    call parse_uint
    neg rax
    inc rdx
    ret 
.p_uint:
    call parse_uint
    ret

string_equals:
    push rdi
    push rsi
    call string_length
    mov r8, rax
    pop rsi
    mov rdi, rsi
    push rsi
    call string_length
    mov r9, rax
    pop rsi
    pop rdi
    cmp r8, r9
    jne .ret_zero
    xor rcx, rcx
    xor r8, r8
    xor r9, r9
.loop:
    cmp rax, rcx
    jz .ret_one
    mov r8b, byte [rdi+rcx]
    mov r9b, byte [rsi+rcx]
    inc rcx
    cmp r8b, r9b
    je .loop
.ret_zero:
    xor rax, rax
    ret
.ret_one:
    mov rax, 1
    ret

string_copy:
    xor rcx, rcx
.loop:
    cmp rdx, rcx
    jb .ret
    mov r8b, [rdi+rcx]
    mov [rsi+rcx], r8b
    inc rcx
    jmp .loop
.ret:
    push rsi
    call string_length
    pop rsi
    inc rax
    cmp rax, rdx
    jz .good
.zero:    
    xor rax, rax
    ret
.good:
    mov rax, rsi
    ret
