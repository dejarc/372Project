/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef ALU_H
#define ALU_H

/* BIT HEADER */
#include "bit.h"

/* MACROS */
#define NUM_FUNC 8	//number of functions

/* arithmeticlogicunit (alu) struct
 * -----------------------------------------------------------------------------
 * Contains the drive and both load bits (one for register A and B) as well as
 * two special signed registers for holding the operands, A and B, and a bit
 * switch array for determining what mathematical operator to use on A and B.
 * current program counter.
 */
typedef struct {
	bit DrALU;
	bit LdA;
	bit LdB;
	aluw A;
	aluw B;
	bit func[NUM_FUNC];
} arithmeticlogicunit;

/* POINTER */
typedef arithmeticlogicunit *alu_;

/* CONSTRUCTOR/DESTRUCTOR */
alu_ alu_ctor();
void alu_kill(alu_);

/* BUS METHODS */
void alu_Dr(alu_);
void alu_LdA(alu_);
void alu_LdB(alu_);

/* SPECIAL METHODS */
//note: last four functions unimplemented in fsm
void alu_addb(alu_); //000	a + b
void alu_nand(alu_); //001	a nand b
void alu_asub(alu_); //010	a - b
void alu_ainc(alu_); //011	a + 1
void alu_aorb(alu_); //100	a or b
void alu_andb(alu_); //101	a and b
void alu_axrb(alu_); //110	a xor b
void alu_nota(alu_); //111	not a

#endif
