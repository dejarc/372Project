/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef FSM_H
#define FSM_H
#include "bit.h"

#define ROM_SIZE 32
#define OPCD_0 0
#define OPCD_1 3
#define Z_VAL 4
#define MICS_0 5
#define MICS_1 9
#define PC_DR 10
#define ALU_DR 11
#define REG_DR 12
#define MEM_DR 13
#define OFF_DR 14
#define PC_LD 15
#define A_LD 16
#define B_LD 17
#define MAR_LD 18
#define IR_LD 19
#define Z_LD 20
#define MEM_WR 21
#define REG_WR 22
#define ALU_FN 23
#define IR_REG 26
#define S_O 28
#define S_Z 29
#define S_S 30
#define CALLEE_SAVE 31

#define FETCH 0
#define ADD 4
#define NAND 7
#define ADDI 10
#define LW 13
#define SW 17
#define BEQ 21
#define JALR 27
#define HALT 31

typedef struct {
	bit mode;
	word state;
	word ROM[ROM_SIZE];
} finitestatemachine;

typedef finitestatemachine *fsm_;
fsm_ fsm_ctor();
void fsm_kill(fsm_);

#endif
