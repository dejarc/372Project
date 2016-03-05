/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "pc.h"

pc_ pc_ctor() {
	programcounter *pc = malloc(sizeof(programcounter));
	pc->DrPC = false;
	pc->LdPC = false;
	pc->pc = 0;
	return pc;
}

void pc_kill(pc_ pc) {
	free(pc);
	pc = NULL;
}

void pc_Dr(pc_ pc) {
	if (pc->DrPC) bus = pc->pc;
}

void pc_Ld(pc_ pc) {
	if (pc->LdPC) pc->pc = bus;
}
