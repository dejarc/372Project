B: add $t0, $t1, $t2; $t2; $t0 ?$t1 + $t2
A: lw  $t1, b
C: sw  $t0, a
lw  $t1, b
lw  $t2, c
offset: add $t0, $t1, $t2
sw  $t0, a
TESW: lw  $t1, offset($ra); $t1 ?M[$ra+ offset]
SI: lw $t0, tesw ;
lw $t1, 0($t0); $t1 ?num1
lw $t2, 4($t0); $t2 ?num2
lw $t0, 24($ra);
addi$t0, $t0, 3;
sw $t0, 24($ra)
lw $t0, si($ra);
addi$t0, $t0, 3;
sw $t0, si($ra);
addisi, si, 4;
beq$t0, $t1, L1; L1 is offset
j $a0, $t0;
L1: beq$t0,r0,Done
; loop body
j L1;
Done: BEQ $s0, $s1, offset; if ($s0 == $s1) PC=PC+offset
SUB $s0, $s1, $s2; $s0 <-$s1 -$s2
ADDI $s0, $s1, Imm; $s0 <-$s1 + Immediate value
AND $s0, $s1, $s2; $s0 <-$s1 AND $s2
;Show you can realize the effect of the following insw ruction:
IMM: BEQ $s0, $s1, offset; if ($s0 > $s1) PC=PC+offset
add $v0, $a0, $a1; $v0?$a0+$a1
nand$v0, $a0, $a1; $v0 ?~($a0 && $a1)
addi$v0, $a0, 25; $v0?$a0+25
lw$v0, 0x42($fp); $v0 ?MEM[$fp+0x42]
sw$a0, 0x42($fp); MEM[$fp+0x42]?$a0
beq$a0, $a1, done; if($a0==$a1) PC ?PC+1+OFFSET
jalr$at, $ra; $ra?PC+1; PC ?$at
jalr $k0 $zero
