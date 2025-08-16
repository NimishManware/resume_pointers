.data
   next : .asciiz "\n"
   bug : .asciiz "buggy\n"
.text
merge:
###################################################################
# s3 -> n1 , s4 -> n2  
###################################################################
   addi $sp , $sp , -4 # for storing ra
   sw $ra , 0($sp) # store this in stack 

   sub $s3, $a3, $a1    
    addi $s3, $s3, 1    # n1 is  = r-l + 1
    sub $s4, $a2, $a3   #n2 is = mid - r
    
    sll $t0 , $s3 , 2  # n1 * 4
    sll $t1 , $s4 , 2 # n2 * 4

  # Allocate the first array L
    li $v0 , 9 
    move $a0, $t0
    syscall 
    move $s5 , $v0  # s5 contain L array


  # Allocate the second array R
    li $v0 , 9
    move $a0 , $t1
    syscall
    move $s6 , $v0 # s6 contain R array

   ### FIRST LOOP ###
    li $t0 , 0  # t0 = 0
    forum1:  # first for loop
    bge $t0 , $s3 , jana
    sll $t1 , $t0 , 2  # multiply the offset by 4 i*4
    add $t2 , $t1 , $s5  # addrees of L[i] is stored in t2

    add $t3 , $a1 , $t0 # l + i
    sll $t3 , $t3 , 2   # 4*(l+i)
    add $t3 , $s7 , $t3 # t3 address of the a[l+i]

    lw $t4 , 0($t3) # this is a[l+i]

    sw $t4 , 0($t2) # L[i] = a[l+i]
    addi $t0 , $t0 , 1  # i++

    j forum1

   ### SECOND LOOP ###
    jana:
    li $t0 , 0 # t0 = 0
    forum2:   # second for loop
    bge $t0 , $s4 , further
    sll $t1 , $t0 , 2  # multiply the offset by 4 j*4
    add $t2 , $t1 , $s6  # addrees of R[i] is stored in t2

    add $t3 , $a3 , $t0 # j + mid
    addi $t3 , $t3 , 1 # t3 has mid + 1 + j
    sll $t3 , $t3 , 2 # 4*(mid+1+j)

    add $t3 , $s7 , $t3 # a[j+mid+1]

     lw $t4 , 0($t3) # this is a[j+1+mid]

    sw $t4 , 0($t2) # L[i] = a[j+1+mid]
    addi $t0 , $t0 , 1 # i++
    j forum2
    
    ### OTHER SECTION ###
    further:
    li $s0 , 0 #i
    li $s1 , 0 #j
    move $s2 , $a1 # k
    while1: # while 1
    bge $s0 , $s3 , further2  # if i < n1
    bge $s1 , $s4 , further2  # if j < n2
    
    move $t0 , $s0
    sll $t0 , $t0 , 2   # 4*i
    add $t1 , $t0 , $s5 # L[i]
    lw $t2 , 0($t1) # L[i] is iin t2 

    move $t3 , $s1
    sll $t3 , $t3 , 2  # 4*j
    add $t4 , $t3 , $s6 # R[j]
    lw $t5 , 0($t4) # R[j] is in t5

    bgt $t2 , $t5 , elsium  # (if left[i] > right[i] go to elsium else continue)

    move $t0 , $s2  # k
    sll $t0 , $t0 , 2  # 4*k
    add $t1 , $t0 , $s7    # a[k]
    sw $t2 , 0($t1)  #  a[k] = L[i] 
    addi $s0 , $s0 , 1 #i++
    addi $s2 , $s2 , 1 #k++
    j while1
    

    elsium:       # else part that occurs if left[i] > right[i]
     move $t0 , $s2  # k
    sll $t0 , $t0 , 2  # 4*k
    add $t1 , $t0 , $s7    # a[k]
    sw $t5 , 0($t1)   # a[k] = R[j]
    addi $s1 , $s1 , 1 #j++
    addi $s2 , $s2 , 1 #k++
    j while1

   
    further2:
    while2:
    bge $s0 , $s3 , while3 # while i < n1
    move $t0 , $s0
    sll $t0 , $t0 , 2   # 4*i
    add $t1 , $t0 , $s5 # L[i]
    lw $t2 , 0($t1) # L[i] is iin t2

     move $t0 , $s2  # k
    sll $t0 , $t0 , 2  # 4*k
    add $t1 , $t0 , $s7    # a[k]
    sw $t2 , 0($t1)  #  a[k] = L[i] 

    addi $s0 , $s0 , 1 #i++
    addi $s2 , $s2 , 1 #k++
    j while2


    while3:  # while i < n2
    bge $s1 , $s4 , story_ends

     move $t3 , $s1
    sll $t3 , $t3 , 2  # 4*j
    add $t4 , $t3 , $s6 # R[j]
    lw $t5 , 0($t4) # R[j] is in t5

     move $t0 , $s2  # k
    sll $t0 , $t0 , 2  # 4*k
    add $t1 , $t0 , $s7    # a[k]
    sw $t5 , 0($t1)   # a[k] = R[j]
    addi $s1 , $s1 , 1  # j++
    addi $s2 , $s2 , 1  # k++

    j while3

    story_ends:   # return by restoring the stack pointer
       
       lw $ra , 0($sp)
       addi $sp , $sp , 4
       jr $ra

 mergesort:
   addi $sp , $sp , -4  # store the return address 
  sw $ra , 0($sp)
   
   blt $a1 , $a2 , l   # l < r
     lw $ra , 0($sp) # load addr and return
     addi $sp , $sp , 4
     
    # sll $t5 , $a1 , 2
    # add $t5 , $t5 , $s7
    # li $v0 , 1
    # lw $a0 , 0($t5)
    # syscall
     
     jr $ra  # if l >= r then return 
   l:
    add $t0 , $a1 , $a2
    srl $t0 , $t0 , 1  # t0 is in mid
    
    addi $sp ,$sp , -12  # sub the stack
    sw $a1 , 8($sp) #low
    sw $a2 , 4($sp) #high
    sw $t0 , 0($sp) #mid
    
    move $a2 , $t0 # low = l , high = mid
    jal mergesort  # call mergesort(arr , l , mid)
    
    lw $t0 , 0($sp) #mid
    addi $t1 , $t0 , 1 #mid + 1
    move $a1 , $t1 # low = mid+1
    lw $a2 , 4($sp) # high = r
    jal mergesort # call mergesort(arr , mid+1 , r);
    
     lw $a1 , 8($sp)  # restore the stack  8
    lw $a2 , 4($sp)  
    lw $a3 , 0($sp) # this is our mid
    jal merge
    
    lw $ra , 12($sp) 
    addi $sp , $sp , 16
    jr $ra
    
 binarySearch:
  addi $sp, $sp, -4           
  sw $ra, 0($sp)              
  blt $a2, $a1, fail   # If r < l then we failed
  add $t1, $a2, $a1    # Right + Left
  srl $t0, $t1, 1      # (Right + Left) / 2 (mid) store in t0
  sll $t1, $t0, 2      # mid*4
  add $t1, $t1, $a0   # Add offset into array
  lw $t1, 0($t1)   # Load array element into memory
  beq $t1, $a3, found    # The element was found
  bgt $t1, $a3, greater  # The element is greater than the pointer
  addi $a1, $t0, 1   # Set left to mid+1
  jal binarySearch  # Search with new variable
  j leave

found:
  move $v1, $t0  # Move mid answer into memory
  j leave  # End

greater:
  addi $a2, $t0, -1 # Set right to mid-1
  jal binarySearch  # Search with new right
  j leave

fail:
  li $v1, -1  # Load failed value
  j leave  # Load end

leave:
  lw $ra, 0($sp) # Load the return
  addi $sp, $sp, 4  # Raise stack poiner
  jr $ra # Return


main:
  li $v0, 5           
   syscall             
   move $t9, $v0  #t9 stores the size of input array
   move $t0, $v0         

   sll $t0, $t0, 2      
  
  ## allocate the array ##
   li $v0, 9          
   move $a0, $t0        
   syscall            
   
   move $s6, $v0        
   move $s7 , $v0  #s7 contains the address of the array
  
  ########################################
  # storing the user inputs in the array # 
  ########################################
   addi $t1 , $zero , 0
   loop_start:
      bge $t1, $t9, loop_end 
      li $v0, 5           
      syscall              
      sw $v0, 0($s6)      
      addi $t1, $t1, 1  
      addi $s6, $s6, 4
      j loop_start

   loop_end:
      #move $v1 , $s7  # v1 stors the address
      move $a2 , $t9  #high
      addi $a2 , $a2 , -1  #now correct high
      addi $a1 , $zero , 0 #correct low

      ### ARRAY HAS BEEN SORTED ###
     jal mergesort
    
    # remember that s7 and t9 are special
    # Now is query time
     li $v0 , 5 
     syscall
     move $t8, $v0    #t8 stores the size of answer array
     move $t0 , $v0 
     
     sll $t0 , $t0 , 2
     move $a0 , $t0
     
     li $v0 , 9
     syscall
     
     move $s5 , $v0
     move $s6 , $v0  #s6 contains the address of ans array
     

     # loop for inputting the numbers q times and then apply binary search to them then store them in others array
    addi $t1 , $zero , 0
    #### QUERY TIME #####
    loopez:
       bge $t1, $t8, loopez_end
   li $v0 , 5 # taking input as number
   syscall
   

  ## loading the arguments
   move $a0 , $s7 
   li $a1 , 0
   move $a2 , $t9
   move $a3 , $v0
   
   addi $sp , $sp , -8
   sw $t1 , 0($sp)
   sw $t8 , 4($sp)
   
   ## get the number and do the binary search
   jal binarySearch
   
   
   lw $t1 , 0($sp)
   lw $t8 , 4($sp)
   addi $sp , $sp , 8
   ## in v1 the value of binary search would be returned , load in ans array
   sw $v1 , 0($s5)
     
     addi $s5 , $s5 , 4
     addi $t1 , $t1 , 1
     
     j loopez
     
     ## WHEN THE ANS ARRAY IS POPULATED THEN IT IS PRINTED
     loopez_end:
     addi $t1 , $zero , 0
     lumber:
      bge $t1, $t8, lumber_end
      
      li $v0 , 1
      
      lw $a0 , 0($s6)
      
      syscall

      li $v0 , 4

      la $a0 , next

      syscall
      
      addi $s6 , $s6 , 4
      addi $t1 , $t1 , 1
      
      j lumber
      
      lumber_end:
      li $v0 , 10
      syscall
      
      
     
       
   
      
      
     
