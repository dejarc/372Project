/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "fsm.h"

/*
 * The finite state machine, containing an array of all possible microstates the
 * computer can be in. When signaled by clock.c, either progresses to the next
 * finite state by flipping the appropriate load, drive, and write switches or
 * updates the state to be next executed. When updating, fsm.c will use the
 * current state to determine the next sequential state unless one of two
 * conditions are met: if the current state is ifetch3, fsm.c reads the opcode
 * stored in ir.c; if the current state is beq3, fsm.c reads the z-value stored
 * in bus.c. The default starting state upon boot-up is at ifetch1. Only
 * component able to circumvent the regular order of communications by being
 * able to directly read the ir.c’s instruction and and the bus.c’s z-value.
 * Signal function switches all the bits of drive, load, write; next loads the
 * next state, calls ir or z if needed.
 */

fsm_ fsm_ctor() {
	int romaddress;
	finitestatemachine *fsm = malloc(sizeof(finitestatemachine));
	fsm->mode = false;
	fsm->state = 0;

				   /* 	         code - ##### PARMO PABMIZ MR ## ## OZ+SC
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
				    * 	         op   z st    dr    ld     wr fn  rg +
				    * 	         code - ##### PARMO PABMIZ MR ### ## OZ+S */
	word states[] = {	      /* 0123 4 56789 01234 567890 12 345 67 8901 */
				/*ifetch1*/stow("1111 0 00000 10000 010100 00 000 00 0010"),//0
				/*ifetchc*/stow("1111 0 10011 00000 000000 00 000 00 0000"),//1
				/*ifetch2*/stow("1111 0 00001 00010 000010 00 000 00 0010"),//2
				/*ifetch3*/stow("1111 0 00010 01000 100000 00 011 00 1000"),//3
				/*add1   */stow("0000 0 10000 00100 010000 00 000 01 1010"),//4
				/*add2   */stow("0000 0 10001 00100 001000 00 000 10 1010"),//5
				/*add3   */stow("0000 0 10010 01000 000000 01 000 00 0000"),//6
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*nand1  */stow("0001 0 10000 00100 010000 00 000 01 1010"),//7
				/*nand2  */stow("0001 0 10001 00100 001000 00 000 10 1010"),//8
				/*nand3  */stow("0001 0 10010 01000 000000 01 001 00 0000"),//9
				/*addi1  */stow("0010 0 10000 00100 010000 00 000 01 1010"),//10
				/*addi2  */stow("0010 0 10001 00001 001000 00 000 10 1010"),//11
				/*addi3  */stow("0010 0 10010 01000 000000 01 000 00 0000"),//12
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*lw1    */stow("0011 0 10000 00100 010000 00 000 01 1010"),//13
				/*lw2    */stow("0011 0 10001 00001 001000 00 000 00 1010"),//14
				/*lw3    */stow("0011 0 10010 01000 000100 00 000 00 1010"),//15
				/*lw4    */stow("0011 0 10011 00010 000000 01 000 00 0000"),//16
				/*sw1    */stow("0100 0 10000 00100 010000 00 000 01 1010"),//17
				/*sw2    */stow("0100 0 10001 00001 001000 00 000 00 1010"),//18
				/*sw3    */stow("0100 0 10010 01000 000100 00 000 00 1010"),//19
				/*sw4    */stow("0100 0 10011 00100 000000 10 000 00 0000"),//20
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*beq1   */stow("0101 0 10000 00100 010000 00 000 00 1010"),//21
				/*beq2   */stow("0101 0 10001 00100 001000 00 000 01 1010"),//22
				/*beq3   */stow("0101 0 10010 01000 000001 00 010 00 0110"),//23
				/*beq4   */stow("0101 1 10011 10000 010000 00 000 00 1010"),//24
				/*beq5   */stow("0101 0 10000 00001 001000 00 000 00 1010"),//25
				/*beq6   */stow("0101 0 10001 01000 100000 00 000 00 0000"),//26
			    /* 	        	 op   z st    dr    ld     wr fn  rg +
			    * 	         	 code - ##### PARMO PABMIZ MR ### ## OZ+S
				*				 0123 4 56789 01234 567890 12 345 67 8901 */
				/*jalr1  */stow("0110 0 10000 10000 000000 01 000 01 1010"),//27
				/*jalr2  */stow("0110 0 10001 00100 100000 00 000 00 0000"),//28
				/*halt   */stow("0111 0 10000 00000 000000 00 000 00 0000") //29
				////							  |						 |						  |			 |			   |
				////            CODE      STATE | PC	ALU		REG	MEM	 OFF | PC  A  B  MAR   IR   Z | MEM  REG | FUNC  REGNO | M  T
	};
	for(romaddress = 0; romaddress < ROM_SIZE; romaddress++)
		fsm->ROM[romaddress] = states[romaddress];

	return fsm;

}

void fsm_kill(fsm_ fsm) {
	free(fsm);
	fsm = NULL;
}


//void fsm_next(fsm_ fsm);
//void fsm_ir(fsm_ fsm);
//void fsm_z(fsm_ fsm);

/*
 *
 *
 *
 * ************************************************************************** *
 * ************************************************************************** *
 * ************************************************************************** *
 * ************************************************************************** */


//fsm_ fsm = (fsm_)malloc(sizeof(finitestatemachine));
//	fsmp->mode = 1;
//	fsmp->states[2] = fsmp->states[0];
//fsm->states = {/*ifetch1*/stow("1111 000 10000 010100 00 0000 00000 010"),
	//		   /*ifetchc*/stow("1111 001 10000 010100 00 0000 00000 001") };
//};
