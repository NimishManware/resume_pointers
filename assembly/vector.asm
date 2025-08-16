extern realloc
extern free


section .text
global init_v
global delete_v
global resize_v
global get_element_v
global push_v
global pop_v
global size_v

init_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        mov qword[rdi], 0 ; Initializing everything to 0 or NULL
        mov qword[rdi + 8], 0
        mov qword[rdi + 16], 0

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

delete_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi

        mov rbx, qword[rdi + 16]
        test rbx, rbx ; testing if memory is allocated
        jmp exit
        call free 
        pop rdi

        ; resetting everything to 0 or null
        mov qword[rdi], 0
        mov qword[rdi + 8], 0
        mov qword[rdi + 16], 0
exit:

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

resize_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi
        push rsi

        ; move base address of the memory to rdi and mem., size to rsi to pass arguments to realloc
        mov rdi, qword[rdi + 16]
        imul rsi, 8
        call realloc

        pop rsi
        pop rdi

        ; update newly allocated memory (if any) by realloc and update buffer size 
        mov qword[rdi + 16], rax 
        mov qword[rdi], rsi

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

get_element_v:
        push rbp
        mov rbp, rsp
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi
        push rsi

        mov rdi, qword[rdi + 16]

        imul rsi, 8
        add rsi, rdi
        mov rax,rsi ; *address* of the ith elem is in rax now

        pop rsi
        pop rdi

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

push_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi
        push rsi 

        mov r8, qword[rdi]
        mov r9, qword[rdi + 8]
        cmp r8, r9 ; check if buffer size is greater than the size of the vector
        jg pushing ; if it is then we just enter the new elem

        push rdi
        push rsi

        ; s := 2*s + 1
        imul r8, 2
        add r8, 1
        mov rsi, r8

        call resize_v ; increase the buffer size to 2*s + 1

        pop rsi
        pop rdi

pushing:
        mov r10, qword[rdi + 16]
        mov r9, qword[rdi + 8]

        mov qword[r10 + r9*8], rsi ; enter the new element at the nth position

        add r9, 1 ; increment buffer size
        mov qword[rdi + 8], r9 

        pop rsi
        pop rdi

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

pop_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi

        mov r8, qword[rdi + 8]
        sub r8, 1 ; r8 contains n-1
        mov rdi, qword[rdi + 16] ; rdi contains base addres
        mov rax, qword[rdi + r8*8] ; move the last element in rax which is return register

        pop rdi ; restore rdi

        mov qword[rdi + 8], r8 ; update size to n-1

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret

size_v:
        push rbp
        mov rbp, rsp
        push rax
        push rbx
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        ; ENTER YOUR CODE HERE, DO NOT MODIFY EXTERNAL CODE
        push rdi

        mov rax, qword[rdi + 8]

        pop rdi

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rbx
        mov rsp, rbp
        pop rbp
        ret
