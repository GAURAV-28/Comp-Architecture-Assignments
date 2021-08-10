.data                    
char:       .space 2
prompt:     .asciiz "\nPlease enter Postfix Expression, then press ENTER:  \n"
null:       .asciiz ""
space:      .ascii " "
newline:    .asciiz "\n"
er:         .asciiz "\nIncorrect Postfix Expression!\n"
err:        .asciiz "\nUnexpected char in the Postfix string!\n"
res:        .asciiz "Result: "
.text                   

main:
move $s0, $sp           # save stack inital position in s0
la $a0, prompt          # load prompt message into $a0
li $v0, 4               # print string
syscall                 
j read                  # call read function to get user input string         

read:                   # read multiple chars from keyboard buffer until ENTER key,

loop:                   # start of read loop
jal getc                # jump to getc subroutine
lb $t0, char            # load the char from char buffer into t0, stripping null
lb $t1, newline         # load newline char into t1
beq $t0, $t1, done      # end of string?  jump to done
j check

getc:                   # read char from keyboard buffer and return ascii value
li $v0, 8               # call code for read string
la $a0, char            # load address of char for read
li $a1, 2               # length of string is 1byte char and 1byte for null
syscall                 # store the char byte from input buffer into char

jr $ra                  # jump-register to calling function
               
check:                  # check loop
blt $t0, 48, w          # check if char is not a digit (ascii<'0')
bgt $t0, 57, inp_error  # check if char is not a digit (ascii>'9')
addi $t0, $t0, -48      # converts t0's ascii value to dec value

subu $sp, $sp, 4
sw $t0, ($sp)
j loop                  # jump to start of read loop

w:
beq $t0, 42, x1          # check for *
beq $t0, 43, x2          # check for +
beq $t0, 45, x3          # check for -
j inp_error

x1:
addu $s1, $sp, 8        # s1 = sp + 8 (to check if stack has 2 elements)
sltu $s6, $s0, $s1      # if s0 < s1, then error (s0->initial stack position)
bne $s6, $0, error
lw $s2, ($sp)           # pop 2 times
lw $s3, 4($sp)
addu $sp, $sp, 8
mul $s4, $s2, $s3       # s4 = s2*s3
subu $sp, $sp, 4        # push s4
sw $s4, ($sp)
j loop                  # jump to start of read loop

x2:
addu $s1, $sp, 8        # s1 = sp + 8 (to check if stack has 2 elements)
sltu $s6, $s0, $s1      # if s0 < s1, then error (s0->initial stack position)
bne $s6, $0, error
lw $s2, ($sp)           # pop 2 times
lw $s3, 4($sp)
addu $sp, $sp, 8
add $s4, $s2, $s3       # s4 = s2 + s3
subu $sp, $sp, 4        # push s4
sw $s4, ($sp)
j loop                  # jump to start of read loop

x3:
addu $s1, $sp, 8        # s1 = sp + 8 (to check if stack has 2 elements)
sltu $s6, $s0, $s1      # if s0 < s1, then error (s0->initial stack position)
bne $s6, $0, error
lw $s2, ($sp)           # pop 2 times
lw $s3, 4($sp)
addu $sp, $sp, 8
sub $s4, $s3, $s2       # s4 = s3 - s2
subu $sp, $sp, 4        # push s4
sw $s4, ($sp)
j loop                  # jump to start of read loop

inp_error:              # print input error message
li $v0, 4
la $a0, err
syscall

j end

error:                  # print postfix error message
li $v0, 4
la $a0, er
syscall
j end

done:
lw $s7, ($sp)        # pop to s7
addu $sp, $sp, 4

bne $s0, $sp, error  # if s0 != sp, then error (stack isn't empty)

la $a0, res          # load res message into $a0
li $v0, 4            # print string
syscall

li $v0, 1           # print result
move $a0, $s7           
syscall 

li $v0, 4           # print newline
la $a0, newline
syscall

end:
li $v0, 10      #ends program
syscall