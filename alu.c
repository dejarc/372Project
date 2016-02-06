/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "alu.h"

alu_ alu_ctor() {
	arithmeticlogicunit alu = { 0 };
	alu_ alup = &alu;
	return alup;
}
void alu_kill(alu_ alu) {
	free(alu);
	alu = NULL;
}

void alu_Dr(alu_ alu) {
	if (alu->DrALU) {
	//call function set by fn[]
		///

		//bus = alu->functionresult
	}
}
void alu_LdA(alu_ alu) {
	if (alu->LdA) alu->A = bus;
}
void alu_LdB(alu_ alu) {
	if (alu->LdB) alu->B = bus;
}

void alu_addb(alu_);
void alu_nand(alu_);
void alu_asub(alu_);
void alu_ainc(alu_);
