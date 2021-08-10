# -----------------------------------------------------
# Data Declarations
.data
hdr:    .asciiz "Enter n: "
m1:     .asciiz "Enter X coordinate: "
m2:     .asciiz "Enter Y coordinate: "
ansMsg: .asciiz "Squared sum: "  
mess:   .asciiz "\nArea of entered coordinates: "
space:  .asciiz " "   
nl:     .asciiz "\n"  
er:     .asciiz "Enter N greater than or equal to 2\n"
erx:    .asciiz "Enter coordinates with X coordinate in non-decreasing order\n"
twof:   .double 2.0 
# -----------------------------------------------------
# text/code section
.text
.globl main
.ent   main
main:
    li $v0,4         # Prints "Enter n: "
    la $a0, hdr
    syscall

    li $v0,5        # Takes input n
    syscall
    
    move $t0,$v0    # moves n to t0

    blt $t0,2,k     # if n is less than 2, goes to k
    j l             # if n is isn't less than 2, code jumps to l

k:  li $v0,4        # this code is executed if n is less than 2
    la $a0, er      
    syscall         # prints error message
    j m             # jumps to m, to exit the program

l:  li $v0,4        # Prints "enter x coordinate"
    la $a0, m1
    syscall

    li $v0,5        # takes input x
    syscall

    move $t1,$v0    # moves x to t1

    li $v0,4        # Prints "enter y coordinate"
    la $a0, m2
    syscall

    li $v0,5        # takes input y
    syscall     

    move $t2,$v0    # moves y to t2

    sub $t0,$t0,1   # stores t0 -1 in t0. (n-1)

    

loop:
    li $v0,4        # prints "enter x coordinate"
    la $a0, m1
    syscall

    li $v0,5        # takes input x
    syscall

    move $t3,$v0    # moves input to t3

    blt $t3, $t1, sort_error # loop is continued if previous x <= input x
    j loop_continued

sort_error:
    li $v0,4        # this code is executed if input x < previous x
    la $a0, erx      
    syscall         # prints error message
    j m             # jumps to m, to exit the program

    
loop_continued:     # loop continued 
    li $v0,4        # prints "enter y coordinate"
    la $a0, m2
    syscall

    li $v0,5        # takes input y
    syscall

    move $t4,$v0    # moves input y to t4

    mul $s0,$t4,$t2 # s0 stores y*y'

    abs $s2, $t4    #abs y
    abs $s1, $t2    #abs y'
    
    sub $t5,$t3,$t1 # t3 - t1 is stored in t5(t5=x-x')
    add $t6,$s1,$s2 # s2 + s1 is stored in t6(t6=y+y')
    
    blt $0, $s0, loop_c1    # jumps to loop_c1 if y*y' > 0
    #---This section computes the area if y*y' < 0 ---#
    mul $s4, $s2, $s2       # y*y
    mul $s3, $s1, $s1       # y'*y'
    
    add $s5, $s3, $s4       # y*y+y'*y'
    mul $s6, $t5, $s5       # (x-x')(y^2+y'^2)

    mtc1.d $s6, $f0     # content of s6  in f0 register
    cvt.d.w $f0, $f0    # converts to floating point

    mtc1.d $t6, $f2     # stores y+y' in f2 register
    cvt.d.w $f2, $f2    # converts to floating point

    div.d $f2, $f0, $f2 # the answer is twice the area if y*y' < 0
    j loop_c2           # jumps to part printing the final area



loop_c1:
    #---This section computes the area if y*y' >= 0 ---#
    

    mul $t7,$t5,$t6  # t7 = t5*t6 (t7=(x-x')*(y+y'))
    #add $t8,$t8,$t7 # t8 = t8 + t7 (t8 is the sum of area calculated in previous iterations of loop. sum = previous sum + t7) 

    mtc1.d $t7,$f0    # copy from t7 to f0
    cvt.d.w $f2,$f0   # convert data representation from integer to float

loop_c2:
    l.d $f4,twof        # load 2.0 in f4

    div.d $f6,$f2,$f4   # f6 = f2/f4
    add.d $f12,$f12,$f6 # f12 = f12+f6
    
    move $t1,$t3    # x becomes x' for the next loop
    move $t2,$t4    # y becomes y' for the next loop

    add $t9,$t9,1       # counter(t9) is increased by 1
    blt $t9,$t0,loop    # if counter<n-1, run the loop again
    
    li $v0,4        # this code is executed after loop stops repeating
    la $a0, mess    # prints "Area of entered coordinates: "
    syscall

    li $v0,3        # prints sum calculated/2(f12) 
    syscall

    li $v0,4        # prints "\n"
    la $a0, nl
    syscall

    
# -----------------------------------------------------    
m:
    li $v0, 10  # quits program
    syscall
.end main
