/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef FSM_H
#define FSM_H
#include "bit.h"

#define ROM_SIZE 30

typedef struct {
	bit mode;
	word state;
	word ROM[ROM_SIZE];
} finitestatemachine;

typedef finitestatemachine *fsm_;
fsm_ fsm_ctor();
void fsm_kill(fsm_);

void fsm_signal(fsm_);
void fsm_next(fsm_);
void fsm_ir(fsm_);
void fsm_z(fsm_);

#endif
