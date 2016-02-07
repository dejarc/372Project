/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef PC_H
#define PC_H
#include "bit.h"

typedef struct {
	bit DrPC;
	bit LdPC;
	word pc;
} programcounter;

typedef programcounter *pc_;
pc_ pc_ctor();
void pc_kill(pc_);

void pc_Dr(pc_);
void pc_Ld(pc_);

#endif
