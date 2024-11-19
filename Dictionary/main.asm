section .text
%include "colon.inc"

extern read_word
extern find_word
extern print_string
extern exit
extern string_length
extern print_newline

global _start

section .data
buffer: times 255 db 0
error_message: db "The program could not find such entry", 0

%include "words.inc"

section .text
_start:
	mov rdi, buffer ; буффер для системного вызова
	mov rsi, 255	; размер буфера
	call read_word
	mov rdi, rax	; указатель на считанную строку
	mov rsi, next_entry	;указатель на посл запись
	call find_word
	test rax, rax
	jz .print_error
	mov rdi, rax	; указатель на найденную запись
	add rdi, 8		; смещение до ключа
	push rdi		
	call string_length
	pop rdi
	add rdi, rax	; смещение на длину от ключа до начала значения
	inc rdi			; смещение т.к. null
	call print_string
	call print_newline
	call exit
.print_error:
	mov rdi, error_message
	call print_string
	call print_newline
	call exit
	