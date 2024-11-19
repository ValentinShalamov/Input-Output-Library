
section .text

extern string_equals
extern print_string

global find_word

;rdi - The pointer to the null terminated key string
;rsi - The pointer to the last word in the dictionary

;return rax: the pointer to the found string if exists otherwise 0

find_word:
.loop:
	mov r8, rsi        ; сохраняю адрес текущей записи
	add rsi, 8         ; смещение до ключа
	push rdi		   ; здесь искомая строка
	push r8			   ; здесь адрес текущей записи
	call string_equals
	pop r8
	pop rdi
	cmp rax, 1         ; проверка на равенство строк
	jz .found
	mov rsi, [r8]      ; переход к следующей записи
	test rsi, rsi      ; проверка на конец списка
	jz .not_found
	jmp .loop
.found:
	mov rax, r8		   ; если нашлась строка 
	ret
.not_found:
	xor rax, rax       ; если нет строки
	ret
	