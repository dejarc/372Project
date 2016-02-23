/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef MEM_H
#define MEM_H
#include "bit.h"

typedef struct {
	bit DrMEM;
	bit LdMAR;
	bit WrMEM;
	word MAR;
//	word Din;
//	word Dout;
	word memory[]; //2^20
} randomaccessmemory;

typedef randomaccessmemory *mem_;
mem_ mem_ctor();
void mem_kill(mem_);

void mem_Dr(mem_);
void mem_Ld(mem_);
void mem_Wr(mem_);

#endif
