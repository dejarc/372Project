/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef IR_H
#define IR_H
#include "bit.h"

typedef struct {
	bit DrOFF;
	bit LdIR;
	word instruction;
} instructionregister;

typedef instructionregister *ir_;
ir_ ir_ctor();
void ir_kill(ir_);

void ir_Dr(ir_);
void ir_Ld(ir_);

word ir_reg(ir_, word);

#endif
