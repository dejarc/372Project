/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef REG_H
#define REG_H
#include "bit.h"

typedef struct {
	bit DrREG;
	bit WrREG;
	bit regno[4];
	word Din;
	word Dout;
	const word zero;
	word registers[16];
	const bit reserved[16];
	const bit calleesave[16];
} registerfile;

typedef registerfile *reg_;
reg_ reg_ctor();
void reg_kill(reg_);

void reg_Dr(reg_);
void reg_Wr(reg_);

void reg_writeReserved(reg_);
word save(reg_);

#endif
