/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef MEM_H
#define MEM_H

/* BIT HEADER */
#include "bit.h"

/* MACROS */
#define MAX_MEM 1048576		//max memory for quick load
//#define MAX_MEM 268435456	//max memory for 32-bit system w/ 2gb RAM

/* randomaccessmemory (mem) struct
 * -----------------------------------------------------------------------------
 * Contains the drive, load, and write bits, the memory address register (MAR)
 * which is used for address lookup, and the memory itself (MEM), which is the
 * actual memory contents.
 */
typedef struct {
	bit DrMEM;
	bit LdMAR;
	bit WrMEM;
	word MAR;
	word MEM[]; //unspecified array length so compiler uses heap and not stack
} randomaccessmemory;

/* POINTER */
typedef randomaccessmemory *mem_;

/* CONSTRUCTOR/DESTRUCTOR */
mem_ mem_ctor();
void mem_kill(mem_);

/* BUS METHODS */
void mem_Dr(mem_);
void mem_Ld(mem_);
void mem_Wr(mem_);

#endif
