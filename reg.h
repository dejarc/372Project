/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef REG_H
#define REG_H
#include "bit.h"

#define REG_LOG 4
#define REG_NUM 16

typedef struct {
	bit DrREG;
	bit WrREG;
//	word Din;
//	word Dout;
//	const word zero;
//	bit regno[REG_LOG];
	word regno;
	bit reserved[REG_NUM];
	bit calleesave[REG_NUM];
	word registers[REG_NUM];
} registerfile;

typedef registerfile *reg_;
reg_ reg_ctor();
void reg_kill(reg_);

void reg_Dr(reg_);
void reg_Wr(reg_);

void reg_writeReserved(reg_);
word save(reg_);

#endif
