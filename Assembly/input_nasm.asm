section .data
    fmt dw "%s", 0
    defmsg dw "Hello World!", 0 

; scanf?   
section .bss
    msg resw 1000
    
section .text

; main
global _main

; imported
extern _printf
extern _scanf

_main:
    mov ebp, esp
    ; get from input
    push msg
    call get1
    ; get from variable
    ; mov dword [msg], defmsg
    push fmt
    push msg
    call print1
    add esp, 8
    mov esp, ebp
    ret

get1:
    push ebp
    mov ebp, esp
    push dword [ebp + 8]
    push fmt
    call _scanf
    mov esp, ebp
    pop ebp
    ret

print1:
    push ebp
    mov ebp, esp
    push dword [ebp + 8]
    push dword [ebp + 12]
    call _printf
    add esp, 8
    mov esp, ebp
    pop ebp
    ret