; modpow(base=RDI, exp=RSI, mod=RDX)
; algoritm eficientizat pentru base^exp % mod pentru implementare rsa

global modpow

section .text

modpow: 

    push rbx
    push r12

    ; mut mod din RDX (arg 3) in R12 (prelucram RDX pentru mul/div)
    mov r12, rdx

    
    mov rax, rdi        ; RAX = base
    xor rdx, rdx        ; RDX = 0  
    div r12             ; RAX = base/mod, RDX = base % mod
    mov rdi, rdx        ; RDI = base % mod

    ; result = 1
    mov rcx, 1          ; RCX = result

.loop1: 
    test rsi, rsi ; daca RAX e 0
    jz .done

    ; result * base % mod  
    mov rax, rcx        ; RAX = result
    mul rdi             ; RDX:RAX = result * base
    div r12             ; RDX = (result * base) % mod

    ; dacă bitul e 1, result = RDX (noul result)
    ; dacă bitul e 0, result e RCX (valoarea veche)
    test rsi, 1

    cmovnz rcx, rdx     ; if (exp & 1) result = newresult

    shr rsi, 1

    mov rax, rdi
    mul rdi
    div r12 ;   
    mov rdi, rdx

    jmp .loop1
.skip_multiply:
     
    shr rsi, 1

    ; base = base * base % mod
    mov rax, rdi        ; RAX = base
    mul rdi             ; RDX:RAX = base * base
    div r12             ; RDX = (base * base) % mod
    mov rdi, rdx        ; base = remainder

    jmp .loop1

.done:
    mov rax, rcx        ; return result

    pop r12
    pop rbx
    ret