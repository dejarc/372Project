/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "ir.h"

ir_ ir_ctor() {
	instructionregister *ir = malloc(sizeof(instructionregister));
	ir->DrOFF = false;
	ir->LdIR = false;
	ir->instruction = 0;
	return ir;
}

void ir_kill(ir_ ir) {
	free(ir);
	ir = NULL;
}

void ir_Dr(ir_ ir) {
	if (ir->DrOFF) bus = ir->instruction;//sigx(ir->instruction, 19);
}

void ir_Ld(ir_ ir) {
	if (ir->LdIR) ir->instruction = bus;
}
