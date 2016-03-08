; Calculate the sum of all odd numbers from 1 to 10


.ORIG 2000
;find the sum of odd numbers from 1 to 10
		3000	add	$a2, $zero, $zer0
		3001	addi	$a1, $zero, 11
		3002	addi	$a0, $zero, 1
NOT_DONE:	3004	add	$a2, $a2, $a0
		3005	addi	$a0, $a0, 1
		3006	beq	$a0, $a1, DONE
		3007	j	NOT_DONE   j is an unconditional jump
DONE:		3008	halt	
.END
