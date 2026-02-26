; barret_mod(base=RDI, exp=RSI, mod=RDX)
; algoritm SI MAI eficientizat pentru base^exp % mod pentru implementare rsa

global barrett_mod


section .text
    barrett_mod:

        cmp rdx, 1
        je .done_base; daca mod e 1 return 0

        push rbx    
        push r12
        mov r12, rdx ; mod = R12, base = RDI, exp = RSI

        ; cuplul rax/rdx pentru a calcula mu = 2^128 / mod
         
        xor rax, rax      
        mov rdx, 1
        shl rdx, 63   ; RDX = 2^63, deci RDX:RAX = 2^63 * 2^64 = 2^127
        div r12      
        shl rax, 1 ; RAX = 2^128 / mod = mu

        mov rbx, rax ; mu = RBX

     .reduce:
        mov rcx, 1
        mov rax, rdi ; RAX = base
        xor rdx, rdx ; RDX = 0
        div r12      ; RAX = base/mod, RDX = base % mod
        mov rdi, rdx ; RDI = base = base % mod
        

        .loop:
            test rsi, rsi
            jz .done

            ; result * base % mod  
            mov rax, rcx        ; RAX = result
            mul rdi             ; RDX:RAX = result * base
            
                ;; to fin barret reduction
 
            jmp .loop
 




    .done:
        mov rax, rcx ; return result

        pop r12
        pop rbx
        ret
    .done_base:
        mov rax, 0 ; return 0
        ret