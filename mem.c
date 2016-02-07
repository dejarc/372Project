/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "mem.h"

//const word MAXMEM = 1048576;

mem_ mem_ctor() {
	randomaccessmemory mem = { 0 };
	mem_ memp = &mem;
	return memp;
}

void mem_kill(mem_ mem) {
	free(mem);
	mem = NULL;
}

void mem_Dr(mem_ mem) {
	if (mem->DrMEM) bus = mem->memory[mem->MAR];
}

void mem_Ld(mem_ mem) {
	if (mem->LdMAR) mem->MAR = bus;
}

void mem_Wr(mem_ mem) {
	if (mem->WrMEM) mem->memory[mem->MAR] = bus;
}