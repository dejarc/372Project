/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef MEM_H
#define MEM_H
#include "bit.h"

#define MAX_MEM 1048576
//#define MAX_MEM 268435456

typedef struct {
	bit DrMEM;
	bit LdMAR;
	bit WrMEM;
	word MAR;
	word memory[]; //2^20 / MAX_MEM
} randomaccessmemory;

typedef randomaccessmemory *mem_;
mem_ mem_ctor();
void mem_kill(mem_);

void mem_Dr(mem_);
void mem_Ld(mem_);
void mem_Wr(mem_);

#endif
