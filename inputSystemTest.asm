;    //	0	1	2	 3	4	   5	  6	 7
;    //	$zero 	$at	$v0	 $a0	$a1	   $a2	  $t0	 $t1
;	true,  true,  	false, 	false, false, 	  false, false, false,
;
;   //  8	   9	  10	 11	12	   13	  14	 15
;   //	$t2	   $s0    $s1	 $s2	$k0	   $sp	  $fp	 $ra
;	false,     false, false, false, true,  	   false, false, false,
.ORIG 0
BEQ	$a2, $a0, 11
ADDI	$a2 $a0 -4;
ADDI	$s1,$a0, 4
ADD $zero,$zero $a2
	ADD $fp, $a0 $a2;;;;;;;;;
 NAND	$v0, $a0 $a2
ADDI $a1 $zero 2
LW $t0 $a1, 4
SW $s1 $a0 5
ADDI $a1 $a1 1
LW $v0 $a1 0
JALR $a0, $ra
HALT;
HALT
SW $t0, $a0 1
HALT
BEQ $zero $zero 3
HALT
HALT;;;;;;
					HALT		;
HALT;
HALT
HALT
HALT
HALT
HALT
HALT
HALT
HALT
HALT
.END

;	LC2200->mem->MEM[0] = stow("0101 0011 0000 0000 0000 0000 0000 1011"); //beq fail
;	LC2200->mem->MEM[1] = stow("0010 0101 0011 1111 1111 1111 1111 1100"); //reg5 =adds -4 to reg3
;	LC2200->mem->MEM[2] = stow("0010 1010 0011 0000 0000 0000 0000 0100"); //reg10=adds 4  to reg3
;	LC2200->mem->MEM[3] = stow("0000 0000 0000 0000 0000 0000 0000 0101"); //attempts to add reg5 to $zero
; 	LC2200->mem->MEM[4] = stow("0000 1110 0011 0000 0000 0000 0000 0101"); //reg14=reg3+reg5
; 	LC2200->mem->MEM[5] = stow("0001 0010 0011 0000 0000 0000 0000 0011"); //reg2=reg3nandreg3
; 	LC2200->mem->MEM[6] = stow("0010 0100 0000 0000 0000 0000 0000 0010"); //reg4+=2
; 	LC2200->mem->MEM[7] = stow("0011 0110 0100 0000 0000 0000 0000 0100"); //reg6=mem[reg4+4]
; 	LC2200->mem->MEM[8] = stow("0100 1010 0011 0000 0000 0000 0000 0101"); //mem[reg3+5]=reg10
; 	LC2200->mem->MEM[9] = stow("0010 0100 0100 0000 0000 0000 0000 0001"); //reg4++
; 	LC2200->mem->MEM[10]= stow("0011 0010 0100 0000 0000 0000 0000 0000"); //reg2=mem[reg4]
; 	LC2200->mem->MEM[11]= stow("0110 0011 1111 0000 0000 0000 0000 0000"); //jump to 14
; 	LC2200->mem->MEM[12]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (jumped)
; 	LC2200->mem->MEM[13]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (jumped)
; 	LC2200->mem->MEM[14]= stow("0100 0110 0011 0000 0000 0000 0000 0001"); //mem[reg3+1] = reg6
; 	LC2200->mem->MEM[15]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (overwritten)
; 	LC2200->mem->MEM[16]= stow("0101 0000 0000 0000 0000 0000 0000 0011"); //beq->pc18
; 	LC2200->mem->MEM[17]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
; 	LC2200->mem->MEM[18]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
; 	LC2200->mem->MEM[19]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (overwritten)
; 	LC2200->mem->MEM[20]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
; 	LC2200->mem->MEM[50]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //emergency halt
