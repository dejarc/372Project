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
	if (ir->DrOFF) bus = sigx(bits(ir->instruction, 12, 31), bitt(ir->instruction, 12));
//			ir->instruction;//sigx(ir->instruction, 19);
}

void ir_Ld(ir_ ir) {
	if (ir->LdIR) ir->instruction = bus;
}

word ir_opc(ir_ ir) {
	return bits(ir->instruction, 0, 3);
}

word ir_reg(ir_ ir, word reg) {
	if (reg == 0) return bits(ir->instruction, 4, 7);
	if (reg == 1) return bits(ir->instruction, 8, 11);
	if (reg == 2) return bits(ir->instruction, 28, 31);
	return 0;
}
