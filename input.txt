;This file adds two 10 numbers;
.ORIG 2000 ;This is the start 
add $a2, $zero, $zero; more stuff here dditional text here 
addi $a1, $zero, 11;
NOT_DONE: 3004 add $a2, $a2, $a0
add $a2, $a2, $a0
sw $a0, 1000($zero)
lw $a0, 1000($zero)
TERMINATE: 3009 add $a2, $a2, $a0
nand $a2, $a1, $a1
addi $a0, $a0
add $v0, $a0, $a1
3011 jalr TERMINATE
3007 j NOT_DONE
3001 beq $a0, $zero, NEXT 
sw $a0, 1000($zero)
lw $a0, 1000($zero)
3002 beq $a0, $zero, TEMP 
add $a2, $a2, $a0
TEMP: 3005 halt  
nand $a2, $a1, $a1
addi $a0, $a0
add $v0, $a0, $a1
NEXT: 3008 halt
.END
done
nand $a2, $a1, $a1 
