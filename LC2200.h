/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef LC_2200_H
#define LC_2200_H

//#include "bit.h"
#include "fsm.h"
#include "pc.h"
#include "alu.h"
#include "reg.h"
#include "mem.h"
#include "ir.h"

typedef enum {
	Dr, Ld, Wr
} signal;

typedef enum {
	_pc, _aluA, _aluB, _reg, _mem, _ir
} component;

typedef struct {
	bit clock;
	fsm_ fsm;
	pc_ pc;
	alu_ alu;
	reg_ reg;
	mem_ mem;
	ir_ ir;

} littlecomputer2200;

typedef littlecomputer2200 *LC2200_;

LC2200_ LC2200_ctor();
void LC2200_kill(LC2200_);
void start(LC2200_, char, word);
void cycle(LC2200_);

#endif

//typedef union {
//	finitestatemachine ;
//	systembus;
//	programcounter;
//	arithmeticlogicunit;
//	registerfile;
//	randomaccessmemory;
//	instructionregister;
//} component;
//
//
//typedef enum {
//
//} type;
//typedef struct {
//	type type;
//	component component;
//} component;
