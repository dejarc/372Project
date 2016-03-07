/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef LC_2200_H
#define LC_2200_H

/* HEADERS */
#include "bit.h"
#include "fsm.h"
#include "pc.h"
#include "alu.h"
#include "reg.h"
#include "mem.h"
#include "ir.h"

/* MACROS */
#define SAFETY_LIMIT 1000
#define MEMORY_DISPLAY_DEBUG 25
#define ROM_RESERVED_CALLEESAVE_DISPLAY_DEBUG false

/* SIGNAL ENUM
 * -----------------------------------------------------------------------------
 * Used in the three macro-stage cycle to emulate drive, load, and write.
 */
typedef enum {
	Dr, Ld, Wr
} signalx;

/* COMPONENT ENUM
 * -----------------------------------------------------------------------------
 * The current component that the control unit is sending signals to.
 */
typedef enum {
	_pc, _aluA, _aluB, _reg, _mem, _ir
} componentx;

/* littlecomputer (LC2200) struct
 * -----------------------------------------------------------------------------
 * The current component that the control unit is sending signals to.
 */
typedef struct {
	bit safetydebug;
	bit statedebug;
	bit microdebug;
	bit clock;
	bit z;
	word cycle;
	fsm_ fsm;
	pc_ pc;
	alu_ alu;
	reg_ reg;
	mem_ mem;
	ir_ ir;
} littlecomputer2200;

/* POINTER */
typedef littlecomputer2200 *LC2200_;

/* CONSTRUCTOR/DESTRUCTOR */
LC2200_ LC2200_ctor();
void LC2200_kill(LC2200_);

/* METHODS */
void start(LC2200_, char);
void setupcycle(LC2200_);
void microstate(LC2200_);
void calleesave(LC2200_);
void debug(LC2200_);

#endif
