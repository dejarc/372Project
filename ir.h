/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef IR_H
#define IR_H

/* BIT HEADER */
#include "bit.h"

/* instructionregister (ir) struct
 * -----------------------------------------------------------------------------
 * Contains the drive and load bits as well as a register for holding the
 * current instruction.
 */
typedef struct {
	bit DrOFF;
	bit LdIR;
	word instruction;
} instructionregister;

/* POINTER */
typedef instructionregister *ir_;

/* CONSTRUCTOR/DESTRUCTOR */
ir_ ir_ctor();
void ir_kill(ir_);

/* BUS METHODS */
void ir_Dr(ir_);
void ir_Ld(ir_);

/* SPECIAL METHODS */
word ir_opc(ir_);
word ir_reg(ir_, word);

#endif
