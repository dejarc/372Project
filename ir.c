/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "ir.h"

/* *****************************************************************************
 * instructionregister (ir)
 * *****************************************************************************
 * Contains the register for holding the current instruction and drive and load
 * bits. The LC2200 uses the ir for determining what opcode to set the fsm to,
 * pulling the registers need for calculations, and for sign-extended the
 */
ir_ ir_ctor() {
	instructionregister *ir = malloc(sizeof(instructionregister));
	ir->DrOFF = false;
	ir->LdIR = false;
	ir->instruction = 0;
	return ir;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the ir.
 */
void ir_kill(ir_ ir) {
	free(ir);
	ir = NULL;
}

/* DRIVE
 * -----------------------------------------------------------------------------
 * If the drive signal is enabled, sign-extends the last 20 bits of the
 * instruction and pushes them onto the bus.
 */
void ir_Dr(ir_ ir) {
	if (ir->DrOFF) bus = sigx(bits(ir->instruction, 12, 31), bitt(ir->instruction, 12));
}

/* LOAD
 * -----------------------------------------------------------------------------
 * If the load signal is enabled, updates the register with bus value.
 */
void ir_Ld(ir_ ir) {
	if (ir->LdIR) ir->instruction = bus;
}

/* OPCODE RETRIEVE
 * -----------------------------------------------------------------------------
 * Returns as a word the opcode (first 4 bits) of the current instruction.
 */
word ir_opc(ir_ ir) {
	return bits(ir->instruction, 0, 3);
}

/* REGISTER RETRIEVE
 * -----------------------------------------------------------------------------
 * Given the positional register desired, returns as a word that register's
 * number. Each possible register storage space in the instruction is four bits
 * long, or 2^4, the exact amount of registers there are in the registerfile
 * (reg). Setup as follows:
 *
 * reg	OPCODE	REGX/1	REGY/2	IMMEDIATE/UNUSED	REGZ/3/IMMEDIATE/UNUSED
 * bit	0123	4567	8901	2345678901234567	8901
 */
word ir_reg(ir_ ir, word reg) {
	if (reg == 0) return bits(ir->instruction, 4, 7);
	if (reg == 1) return bits(ir->instruction, 8, 11);
	if (reg == 2) return bits(ir->instruction, 28, 31);
	return 0;
}
