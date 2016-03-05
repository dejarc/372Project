/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "mem.h"

//const word MAXMEM = 1048576;

mem_ mem_ctor() {
	int address;
	randomaccessmemory *mem = malloc(sizeof(randomaccessmemory) +
									 MAX_MEM * sizeof(word));
	mem->DrMEM = false;
	mem->LdMAR = false;
	mem->WrMEM = false;
	mem->MAR = 0;
	for(address = 0; address < MAX_MEM; address++)
		mem->MEM[address] = 0;

	//mem_ memp = &mem;
	return mem;//memp;
}

void mem_kill(mem_ mem) {
	free(mem);
	mem = NULL;
}

void mem_Dr(mem_ mem) {
	if (mem->DrMEM) bus = mem->MEM[mem->MAR];
}

void mem_Ld(mem_ mem) {
	if (mem->LdMAR) mem->MAR = bus;
}

void mem_Wr(mem_ mem) {
	if (mem->WrMEM) mem->MEM[mem->MAR] = bus;
}
