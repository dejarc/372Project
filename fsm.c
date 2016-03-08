/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "fsm.h"

/* *****************************************************************************
 * finitestatemachine (fsm)
 * *****************************************************************************
 * The finite state machine, containing an array of all possible microstates the
 * computer can be in. Apart from containing the ROM of microstates, relies on
 * external control by the LC2200 to progress to the next finite state. Used for
 * state transition by flipping the appropriate load, drive, and write switches
 * and updating the state to be next executed. The fsm is organized with the
 * first 10 bits representing the current state's opcode, z-value, and state--
 * these are used for finding the next appropriate state later. The next 13 bits
 * tell the control unit what drive, load, and write signals should be on or
 * off. The next 5 bits tell the control unit what function the alu should
 * execute and which of the three registers the ir should drive.
 *
 * Layout of FSM by line:
 * sig	opcode	z-value	state	drive	load	write	alufunc	irreg 	nextstate
 * com	-		-		-		PARMO	PABMIZ	MR		-		-		-
 * idx	0123	4		56789	01234	567890	12		345		67		8901
 * bin	0000	0		00000	00000	000000	00		000		00		0000
 *
 * For bit flipping and signaling, when updating, the fsm will provide bit
 * switches to explain how each current state should determine the next
 * sequential state by using the last four bits in the microstate line. It is
 * expected that the control unit take care of the following conditions: if the
 * current state is ifetch3, the bit sequence 1000 will search only opcodes
 * using the opcode in the ir; if the current state is beq3, the control unit
 * reads primes the z-value by triggering the alu. The default starting state
 * upon boot-up is at ifetch1.
 */
fsm_ fsm_ctor() {
	int romaddress;
	finitestatemachine *fsm = malloc(sizeof(finitestatemachine));
	fsm->state = 0; //ifetch1
				   /* MICROSTATE BIT DETAIL
				    *
				    *			 MICROSTATE ROM LAYOUT
				    * 	         op   z st    dr    ld     wr fn  rg +
				    * 	         code - ##### PARMO PABMIZ MR ### ## OZ+S
					* 			 0123 4 56789 01234 567890 12 345 67 8901
				    *
				    * op 0123		the opcode address of the command
				    * z  4			the z address of the command
				    * st 56789		the state address of the command
				    * dr 01234		drive bits in order: PC,ALU,REG,MEM,OFF
				    * ld 567890		load bits in order:  PC,ALU-A,ALU-B,MAR,IR,Z
				    * wr 12			write bits in order: MEM,REG
				    * fn 345		function bits to ALU
				    * rg 67			register bits to IR
				    * +  8901		how to go to the next step:
				    * 					O 27: use opcode in address
				    * 					Z 28: use z-val in address
				    * 					+ 29: increment and use state in address
				    * 					S 30: whether to invoke callee-save
				    *
				    */
	word states[] = {/* FETCH */
				/* 	         	 op   z st    dr    ld     wr fn  rg +
				* 	       	     code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*ifetch1*/stow("1111 0 00000 10000 010100 00 000 00 0010"),//0
				/*ifetchc*/stow("1111 0 10011 00000 000000 00 000 00 0000"),//1
				/*ifetch2*/stow("1111 0 00001 00010 000010 00 000 00 0010"),//2
				/*ifetch3*/stow("1111 0 00010 01000 100000 00 011 00 1000"),//3

					 /* ADD */
				/* 	         	 op   z st    dr    ld     wr fn  rg +
				* 	       	     code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*add1   */stow("0000 0 10000 00100 010000 00 000 01 1010"),//4
				/*add2   */stow("0000 0 10001 00100 001000 00 000 10 1010"),//5
				/*add3   */stow("0000 0 10010 01000 000000 01 000 00 0000"),//6

					 /* NAND */
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*nand1  */stow("0001 0 10000 00100 010000 00 000 01 1010"),//7
				/*nand2  */stow("0001 0 10001 00100 001000 00 000 10 1010"),//8
				/*nand3  */stow("0001 0 10010 01000 000000 01 001 00 0000"),//9

					 /* ADDI */
				/* 	        	 op   z st    dr    ld     wr fn  rg +
				* 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*addi1  */stow("0010 0 10000 00100 010000 00 000 01 1010"),//10
				/*addi2  */stow("0010 0 10001 00001 001000 00 000 10 1010"),//11
				/*addi3  */stow("0010 0 10010 01000 000000 01 000 00 0000"),//12

					 /* LW */
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*lw1    */stow("0011 0 10000 00100 010000 00 000 01 1010"),//13
				/*lw2    */stow("0011 0 10001 00001 001000 00 000 00 1010"),//14
				/*lw3    */stow("0011 0 10010 01000 000100 00 000 00 1010"),//15
				/*lw4    */stow("0011 0 10011 00010 000000 01 000 00 0000"),//16

					 /* SW */
				/* 	        	 op   z st    dr    ld     wr fn  rg +
				* 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*sw1    */stow("0100 0 10000 00100 010000 00 000 01 1010"),//17
				/*sw2    */stow("0100 0 10001 00001 001000 00 000 00 1010"),//18
				/*sw3    */stow("0100 0 10010 01000 000100 00 000 00 1010"),//19
				/*sw4    */stow("0100 0 10011 00100 000000 10 000 00 0000"),//20

					 /* BEQ */
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*beq1   */stow("0101 0 10000 00100 010000 00 000 00 1010"),//21
				/*beq2   */stow("0101 0 10001 00100 001000 00 000 01 1010"),//22
				/*beq3   */stow("0101 0 10010 01000 000001 00 010 00 0110"),//23
				/*beq4   */stow("0101 1 10011 10000 010000 00 000 00 1010"),//24
				/*beq5   */stow("0101 0 10100 00001 001000 00 000 00 1010"),//25
				/*beq6   */stow("0101 0 10101 01000 100000 00 000 00 0000"),//26

					 /* JALR */
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*jalr1  */stow("0110 0 10000 10000 000000 01 000 01 1010"),//27
				/*jalr2  */stow("0110 0 10001 00100 010000 00 000 00 1010"),//28
				/*jalr3  */stow("0110 0 10010 00001 001000 00 000 10 1010"),//29
				/*jalr4  */stow("0110 0 10011 01000 100000 00 000 00 0000"),//30
				// note: JALR has been modified to additionally read immediate
				//		 values and add them to the second register's value
				//		 to determine the pc value--similar to addi, lw, sw, beq


					 /* HALT */
				/* 	        	 op   z st    dr    ld     wr fn  rg +
				* 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*halt   */stow("0111 0 10000 00000 000000 00 000 00 0000") //31

				//		APPENDIX
				////							  |						 |						  |			 |			   |
				////            CODE      STATE | PC	ALU		REG	MEM	 OFF | PC  A  B  MAR   IR   Z | MEM  REG | FUNC  REGNO | M  T
				/*		 ORIGNAL JALR REGISTER ONLY CODE: 				  */
				/*jalr1  stow("0110 0 10000 10000 000000 01 000 01 1010"),//27*/
				/*jalr2  stow("0110 0 10001 00100 100000 00 000 00 0000"),//28*/
	};
	for(romaddress = 0; romaddress < ROM_SIZE; romaddress++)
		fsm->ROM[romaddress] = states[romaddress];
	return fsm;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the fsm.
 */
void fsm_kill(fsm_ fsm) {
	free(fsm);
	fsm = NULL;
}
