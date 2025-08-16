extern init_v
extern pop_v
extern push_v
extern size_v
extern get_element_v
extern resize_v
extern delete_v

section .text
global init_h
global delete_h
global size_h
global insert_h
global get_max
global pop_max
global heapify
global heapsort

init_h:
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

        call init_v ; just initialize the vector

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

delete_h:
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

        call delete_v ; delete the vector

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


size_h:
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

        call size_v ; return the size of the vector

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


insert_h:
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

        call push_v ; push the element at the last position

        pop rsi
        pop rdi

        ; later fix the heap
        mov r8, qword[rdi + 8]
        sub r8, 1 

whileloop:
        mov rbx, 0
        cmp rbx, r8
        jge endwhile

        ; calculating the parent(i)
        mov r9, r8
        sub r9, 1
        shr r9, 1

        mov r10, qword[rdi + 16]

        mov r11, qword[r10 + r9*8]
        mov r12, qword[r10 + r8*8]
        cmp r11, r12 ; compare arr[i] and arr[parent(i)]
        jge endwhile ; if parent is greater then heap is fixed

        ; swap arr[parent(i)] and arr[i] 
        mov qword[r10 + r9*8], r12
        mov qword[r10 + r8*8], r11

        mov r8, r9 ; i = parent(i)
        jmp whileloop
endwhile:

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

get_max:
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

        mov r8, qword[rdi + 16] 
        mov rax, qword[r8 + 0] ; return the first element of the heap

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

pop_max:
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

        call size_h
        pop rdi
        mov r8, rax
        dec r8 ; r8 contains n-1

        mov r9, qword[rdi + 16]

        ; swap arr[0], arr[n-1]
        mov r10, qword[r9 + r8*8]
        mov rax, qword[r9]
        mov qword[r9 + r8*8], rax 
        mov qword[r9], r10

        ; call heapify(0) which fixes the first element
        mov rsi, r8 ; pass the new size
        mov rdx, 0 ; heapify(0)
        call heapify

        call pop_v

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

heapify:
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
        push rdx
        
        ; takes arguments rdi -> heap, rsi -> size of heap, rdx -> position to heapify; rsi,rdx -> fake arguments
        mov rbx, qword[rdi + 16]

        mov r14, qword[rbx + rdx*8] ; arr(i)
        ; calculating right(i)
        mov r9, rdx
        imul r9, 2
        add r9, 2

        ; calculating left(i)
        mov r11, rdx
        imul r11, 2
        add r11, 1

        cmp r11, rsi
        jge do_nothing ; if left does not exist then right also does not exist, so do nothing 
        cmp r9, rsi
        jge only_left ; if right does'nt exist compare only left

        mov r12, qword[rbx + r9*8]
        mov r13, qword[rbx + r11*8]

        cmp r12, r13 ; compare arr[right(i)] and arr[left(i)]
        jg comp_right ; if left is bigger, we compare with left and vice versa

comp_left:
        cmp r14, r13 ; if arr[i] is bigger then do nothing
        jge do_nothing

        ; swap arr[i] and arr[left(i)] 
        mov qword[rbx + rdx*8], r13
        mov qword[rbx + r11*8], r14

        mov rdx, r11 ; recursively call heapify(left(i)) 
        call heapify
        jmp do_nothing ; this heapify has done its work

comp_right:
        cmp r14, r12 ; if arr[i] is bigger then do nothing
        jge do_nothing

        ; swap arr[i] and arr[right(i)] 
        mov qword[rbx + rdx*8], r12
        mov qword[rbx + r9*8], r14

        mov rdx, r9 ; recursively call heapify(right(i)) 
        call heapify
        jmp do_nothing ; this heapify has done its work

only_left:
        mov r13, qword[rbx + r11*8] ; if arr[i] is bigger then do nothing

        cmp r14, r13
        jge do_nothing

        ; swap arr[i] and arr[left(i)] 
        mov qword[rbx + rdx*8], r13
        mov qword[rbx + r11*8], r14

        mov rdx, r11 ; recursively call heapify(left(i)) 
        call heapify
        jmp do_nothing ; this heapify has done its work

do_nothing:
        pop rdx
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

heapsort:
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
 