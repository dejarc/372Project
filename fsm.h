/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef FSM_H
#define FSM_H

/* BIT HEADER */
#include "bit.h"

/* ROM BIT MACROS */
#define ROM_SIZE 32	//How many microstates total
#define OPCD_0 0	//Opcode first index
#define OPCD_1 3	//Opcode last index
#define Z_VAL 4		//Z-value index
#define MICS_0 5	//Microstate first index
#define MICS_1 9	//Microstate last index
#define PC_DR 10	//pc drive index
#define ALU_DR 11	//alu drive index
#define REG_DR 12	//reg drive index
#define MEM_DR 13	//mem drive index
#define OFF_DR 14	//ir sign-extend drive index
#define PC_LD 15	//pc load index
#define A_LD 16		//alu a load index
#define B_LD 17		//alu b load index
#define MAR_LD 18	//mem mar lead index
#define IR_LD 19	//ir load index
#define Z_LD 20		//bus z-value load index
#define MEM_WR 21	//mem write index
#define REG_WR 22	//reg write index
#define ALUFN_0 23	//alu function first index
#define ALUFN_1 25	//alu function last index
#define IRREG_0 26	//ir reg first index
#define IRREG_1 27	//ir reg last index
#define S_O 28		//next state check opcode index
#define S_Z 29		//next state check z-value index
#define S_S 30		//next state check state index
#define CALLEE_SAVE 31 //unused

/* ROM OPCODE MACROS */
#define FETCH 0		//fetch cycle start
#define ADD 4		//add cycle start
#define NAND 7		//nand cycle start
#define ADDI 10		//addi cycle start
#define LW 13		//lw cycle start
#define SW 17		//sw cycle start
#define BEQ 21		//beq cycle start
#define JALR 27		//jalr cycle start
#define HALT 31		//halt command

/* finitestatemachine (fsm) struct
 * -----------------------------------------------------------------------------
 * Contains a word state, which is where in the ROM the current microstate is,
 * and the ROM itself, which provides for the control unit information on what
 * signals to send out for every microstate.
 */
typedef struct {
	word state;
	word ROM[ROM_SIZE];
} finitestatemachine;

/* POINTER */
typedef finitestatemachine *fsm_;

/* CONSTRUCTOR/DESTRUCTOR */
fsm_ fsm_ctor();
void fsm_kill(fsm_);

#endif
