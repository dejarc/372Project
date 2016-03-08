/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "pc.h"

/* *****************************************************************************
 * programcounter (pc)
 * *****************************************************************************
 * Holds the value of the next instruction to be processed--if its drive or load
 * signal is enable, can push or replace this value. Used by the LC2200 as a
 * marker indicated where in the code it currently is and where to go next.
 */
pc_ pc_ctor() {
	programcounter *pc = malloc(sizeof(programcounter));
	pc->DrPC = false;
	pc->LdPC = false;
	pc->pc = 0;
	return pc;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the pc.
 */
void pc_kill(pc_ pc) {
	free(pc);
	pc = NULL;
}

/* DRIVE
 * -----------------------------------------------------------------------------
 * If the drive signal is enabled, pushes the current pc onto the bus.
 */
void pc_Dr(pc_ pc) {
	if (pc->DrPC) bus = pc->pc;
}

/* LOAD
 * -----------------------------------------------------------------------------
 * If the load signal is enabled, replaces the current pc with the bus value.
 */
void pc_Ld(pc_ pc) {
	if (pc->LdPC) pc->pc = bus;
}
