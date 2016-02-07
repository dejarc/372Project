/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "reg.h"

reg_ reg_ctor() {
	registerfile reg = { 0 };
	reg_ regp = &reg;
	return regp;
}

void reg_kill(reg_ reg) {
	free(reg);
	reg = NULL;
}

void reg_Dr(reg_ reg) {
	if (reg->DrREG) {
		//get register indicated and bus = reg;
	}
}
void reg_Wr(reg_ reg) {
	if (reg->WrREG) {
		//write to indicated reegister from bus;
	}
}

void reg_writeReserved(reg_);
word save(reg_);
