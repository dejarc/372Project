/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "LC2200.h"

word bus;

LC2200_ LC2200_ctor() {
	littlecomputer2200 LC2200 = { 0, fsm_ctor(),
									 pc_ctor(),
									 alu_ctor(),
									 reg_ctor(),
								     mem_ctor(),
									 ir_ctor()
								};
	LC2200_ lcp = &LC2200;
	return lcp;
}

void start(LC2200_ LC2200, char mode, word cycles) {
	//once fully coded, char will be modes of single step, loop step,
	//										  run-until-breakpoint,
	//									      run-until-label,
	//									      run-until-halt
	int tick = 0;
	for(;tick<cycles;tick++) cycle(LC2200);
}

void cycle(LC2200_ LC2200) {
	signal signal;
	component component;
	//line to read previous and update next state
	//line or whatever to set next finite state machine state
	for(signal = Dr; signal <= Wr; signal++)
		for(component = _pc; component <= _ir; component++)
			switch(component) {
				case _pc:	switch(signal) {
					case Dr: pc_Dr(LC2200->pc);		break;
					case Ld: pc_Ld(LC2200->pc);		break;
					case Wr: 						break;
				}; break;
				case _aluA:	switch(signal) {
					case Dr: alu_Dr(LC2200->alu);	break;
					case Ld: alu_LdA(LC2200->alu);	break;
					case Wr: 						break;
				}; break;
				case _aluB:	switch(signal) {
					case Dr: 						break;
					case Ld: alu_LdB(LC2200->alu);	break;
					case Wr: 						break;
				}; break;
				case _reg:	switch(signal) {
					case Dr: reg_Dr(LC2200->reg);	break;
					case Ld: 						break;
					case Wr: reg_Wr(LC2200->reg);	break;
				}; break;
				case _mem:	switch(signal) {
					case Dr: mem_Dr(LC2200->mem);	break;
					case Ld: mem_Ld(LC2200->mem);	break;
					case Wr: mem_Wr(LC2200->mem);	break;
				}; break;
				case _ir:	switch(signal) {
					case Dr: ir_Dr(LC2200->ir);		break;
					case Ld: ir_Ld(LC2200->ir);		break;
					case Wr:						break;
				}; break;
//				case _bus:	switch(signal) {
//					case Dr:
//					case Ld: reg_Ld(LC2200->reg);	break;
//					case Wr: 						break;
//				}; break;
			}
}

