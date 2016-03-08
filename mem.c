/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "mem.h"

/* *****************************************************************************
 * randomaccessmemory (mem)
 * *****************************************************************************
 * The memory is the largest component of the LC2200, acting as the storage
 * behind the entire system. Although the book's LC2200 contained 2^32 memory
 * addresses, this would constitute > 17gb worth of data--far too large for
 * a regular computer to run efficiently. Thus, the actual size of memory is
 * determined in the mem header file and runs from a minimum of 2^20 memory
 * addresses (a little over a million), which shouldn't take up more than a
 * little over 4mb of actual ram size, to a reliable maximum of 2^28 memory
 * addresses (about 256 million), which would take up over 1gb of ram size.
 *
 * The mem has drive, load, and write components. All three utilized the value
 * stored in the memory address register (MAR) to choose which memory cell to
 * interact with the bus.
 */
mem_ mem_ctor() {
	int address;
	randomaccessmemory *mem = malloc(sizeof(randomaccessmemory) +
									 MAX_MEM * sizeof(word));
	mem->DrMEM = false;
	mem->LdMAR = false;
	mem->WrMEM = false;
	mem->MAR = 0;
	for(address = 0; address < MAX_MEM; address++)
		mem->MEM[address] = 0; //initialize to 0

	return mem;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the mem.
 */
void mem_kill(mem_ mem) {
	free(mem);
	mem = NULL;
}

/* DRIVE
 * -----------------------------------------------------------------------------
 * If the drive signal is enabled, pushes MEM[MAR] onto the bus.
 */
void mem_Dr(mem_ mem) {
	if (mem->DrMEM) bus = mem->MEM[mem->MAR];
}

/* LOAD
 * -----------------------------------------------------------------------------
 * If the load signal is enabled, sets MAR to bus contents.
 */
void mem_Ld(mem_ mem) {
	if (mem->LdMAR) mem->MAR = bus;
}

/* WRITE
 * -----------------------------------------------------------------------------
 * If the write signal is enabled, writes to MEM[MAR] from the bus.
 */
void mem_Wr(mem_ mem) {
	if (mem->WrMEM) mem->MEM[mem->MAR] = bus;
}
