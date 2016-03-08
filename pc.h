/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef PC_H
#define PC_H

/* BIT HEADER */
#include "bit.h"

/* programcounter (pc) struct
 * -----------------------------------------------------------------------------
 * Contains the drive and load bits as well as a register for holding the
 * current program counter.
 */
typedef struct {
	bit DrPC;
	bit LdPC;
	word pc;
} programcounter;

/* POINTER */
typedef programcounter *pc_;

/* CONSTRUCTOR/DESTRUCTOR */
pc_ pc_ctor();
void pc_kill(pc_);

/* BUS METHODS */
void pc_Dr(pc_);
void pc_Ld(pc_);

#endif
