/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "pc.h"

pc_ pc_ctor() {
	programcounter pc = { 0 };
	pc_ pcp = &pc;
	return pcp;
}

void pc_Dr(pc_ pc) {
	if (pc->DrPC) bus = pc->pc;
}

void pc_Ld(pc_ pc) {
	if (pc->LdPC) pc->pc = bus;
}
