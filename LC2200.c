/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "LC2200.h"

word bus;

LC2200_ LC2200_ctor() {
	littlecomputer2200 *LC2200 = malloc(sizeof(littlecomputer2200));
	LC2200->safetydebug = false;
	LC2200->statedebug = false;
	LC2200->microdebug = false;
	LC2200->clock = false;
	LC2200->z = false;
	LC2200->cycle = 0;
	LC2200->fsm = fsm_ctor();
	LC2200->pc = pc_ctor();
	LC2200->alu = alu_ctor();
	LC2200->reg = reg_ctor();
	LC2200->mem = mem_ctor();
	LC2200->ir = ir_ctor();
	bus = 0;
	return LC2200;
}

void LC2200_kill(LC2200_ LC2200) {
	fsm_kill(LC2200->fsm);
	pc_kill(LC2200->pc);
	alu_kill(LC2200->alu);
	reg_kill(LC2200->reg);
	mem_kill(LC2200->mem);
	ir_kill(LC2200->ir);
	free(LC2200);
	LC2200 = NULL;
}

void start(LC2200_ LC2200, char mode) {
	int tick = 0;
	//once fully coded, char will be modes of single step, loop step,
	//										  run-until-breakpoint,
	//									      run-until-label,
	//									      run-until-halt
	if (mode == 'h') LC2200->cycle = MAX_MEM;
	else if (mode == 's') LC2200->cycle = LC2200->pc->pc + 1;
	LC2200->clock = true;
	while (LC2200->clock) {
		setupcycle(LC2200);
		microstate(LC2200);
		if (LC2200->pc->pc == LC2200->cycle) LC2200->clock = false;
		if (LC2200->safetydebug) {
			printf("TICK:%d\n", ++tick);
			if (tick >= SAFETY_LIMIT) LC2200->clock = false;
		}
		if (LC2200->microdebug) debug(LC2200);
		else if (LC2200->statedebug && LC2200->fsm->state == 0) debug(LC2200);
	}
}

void setupcycle(LC2200_ LC2200) {
	bit debug = LC2200->microdebug;
	int bit;
	word temp;
	word *rom = LC2200->fsm->ROM;
	word state = LC2200->fsm->state;
	const int oplen = (OPCD_1 - OPCD_0)+1;
	const int nslen = (MICS_1 - MICS_0)+1;
	char opcode[oplen+1];
	char zvalue = ' ';
	char nstate[nslen+1];
	for (bit = 0; bit < oplen; bit++) opcode[bit] = ' ';
	for (bit = 0; bit < nslen; bit++) nstate[bit] = ' ';
	opcode[oplen] = 0;
	nstate[nslen] = 0;
	if (debug) printf("state:%s\n", wtos(state));

	for (bit = PC_DR; bit <= OFF_DR; bit++)
		switch(bit) {
			case PC_DR:		LC2200->pc->DrPC	= bitt(rom[state], PC_DR);	break;
			case ALU_DR:	LC2200->alu->DrALU	= bitt(rom[state], ALU_DR);	break;
			case REG_DR:	LC2200->reg->DrREG	= bitt(rom[state], REG_DR);	break;
			case MEM_DR:	LC2200->mem->DrMEM	= bitt(rom[state], MEM_DR);	break;
			case OFF_DR:	LC2200->ir->DrOFF	= bitt(rom[state], OFF_DR);	break;
		}

	for (bit = PC_LD; bit <= Z_LD; bit++)
		switch(bit) {
			case PC_LD:		LC2200->pc->LdPC	= bitt(rom[state], PC_LD);	break;
			case A_LD:		LC2200->alu->LdA	= bitt(rom[state], A_LD);	break;
			case B_LD:		LC2200->alu->LdB	= bitt(rom[state], B_LD);	break;
			case MAR_LD:	LC2200->mem->LdMAR	= bitt(rom[state], MAR_LD);	break;
			case IR_LD:		LC2200->ir->LdIR	= bitt(rom[state], IR_LD);	break;
			case Z_LD:		if (bitt(rom[state], Z_LD)) { alu_asub(LC2200->alu);
								LC2200->z = bus == 0; }						break;
		}

	for (bit = MEM_WR; bit <= REG_WR; bit++)
		switch(bit) {
			case MEM_WR:	LC2200->mem->WrMEM	= bitt(rom[state], MEM_WR);	break;
			case REG_WR:	LC2200->reg->WrREG	= bitt(rom[state], REG_WR);	break;
		}

	for (bit = 0; bit < NUM_FUNC; bit++) LC2200->alu->func[bit] = false;
	LC2200->alu->func[bits(rom[state], ALU_FN, ALU_FN+2)] = true;
	LC2200->reg->regno = ir_reg(LC2200->ir, bits(rom[state], IR_REG, IR_REG+1));


	if (bitt(rom[state], S_O))
		for (bit = 0; bit < oplen; bit++)
			opcode[bit] = bitt(LC2200->ir->instruction, bit)+'0';

	if (bitt(rom[state], S_Z))
		zvalue = bitt(rom[state+1], Z_VAL);

	if (bitt(rom[state], S_S)) {
		for (bit = 0; bit < nslen; bit++)
			nstate[bit] = bitt(rom[state], MICS_0+bit)+'0';
		temp = stow(nstate) + 1;
		for (bit = 0; bit < nslen; bit++)
			nstate[bit] = bitt(temp, WORD_LEN - MICS_0 + bit)+'0';
	}

	if (bitt(rom[state], CALLEE_SAVE)) calleesave(LC2200);

	if (bits(rom[state], OPCD_0, OPCD_1) == bits(rom[ROM_SIZE-1], OPCD_0, OPCD_1))
		LC2200->clock = false;

	if (debug) {
		printf("bitsirreg:%lu\n", bits(rom[state], IR_REG, IR_REG+1));
		printf("nextopcode:%s\n", opcode);
		printf("zval:%d\n", bitt(rom[state+1], Z_VAL));
		printf("next:%s\n", nstate);
		if (LC2200->clock == false) printf("HALT ON LINE %lu\n", LC2200->pc->pc);
	}

	temp = 0;
	state = 0;
	if (opcode[0] != ' ')
		for (state = temp; state < ROM_SIZE; state++) {
			if (debug) printf("state: %d: %s opcd: %s\n", state, wtos(bits(rom[state], OPCD_0, OPCD_1)), wtos(stow(opcode)));
			if (debug) printf("state: %d: %lu opcd: %lu\n", state, bits(rom[state], OPCD_0, OPCD_1), stow(opcode));
			if (bits(rom[state], OPCD_0, OPCD_1) ==	stow(opcode)) {
				temp = state;
				break;
			}
		}

	if (zvalue != ' ')
		for (state = temp; state < ROM_SIZE; state++) {
			if (debug) printf("state: %d: %s zval: %s\n", state, wtos(bitt(rom[state], Z_VAL)), wtos(zvalue));
			if (debug) printf("state: %d: %lu zval: %lu\n", state, bitt(rom[state], Z_VAL), zvalue);
			if (bitt(rom[state], Z_VAL) == LC2200->z) {
				temp = state;
				break;
			}
		}

	if (nstate[0] != ' ')
		for (state = temp; state < ROM_SIZE; state++) {
			if (debug) printf("state: %d: %s next: %s\n", state, wtos(bits(rom[state], MICS_0, MICS_1)), wtos(stow(nstate)));
			if (debug) printf("state: %d: %lu next: %lu\n", state, bits(rom[state], MICS_0, MICS_1), stow(nstate));
			if (bits(rom[state], MICS_0, MICS_1) ==	stow(nstate)) {
				temp = state;
				break;
			}
		}

	LC2200->fsm->state = temp;

}



void microstate(LC2200_ LC2200) {
	signal signal;
	component component;
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
					case Wr: LC2200->z = bus == 0;	break;
				}; break;
				default: break;
			}
}

void calleesave(LC2200_ LC2200) {

}

void debug(LC2200_ LC2200) {
	bit showrom = false;
	bit showres = false;
	bit showcal = false;
	word showmem = 25;
	int w;
	printf("--------\n");
	printf("bus: %s: %lu\n", wtos(bus), bus);
	printf("LC2200: clock: %lu\n", LC2200->clock);
	printf("LC2200: z: %lu\n", LC2200->z);
	printf("LC2200: cycle: %s: %lu\n", wtos(LC2200->cycle), LC2200->cycle);
	printf("--------\n");
	printf("FSM: mode: %lu\n", LC2200->fsm->mode);
	printf("FSM: state: %s: %lu\n", wtos(LC2200->fsm->state), LC2200->fsm->state);
	if (showrom) for (w = 0; w < ROM_SIZE; w++) printf("FSM: ROM %lu: %s: %lu\n", w, wtos(LC2200->fsm->ROM[w]), LC2200->fsm->ROM[w]);
	printf("--------\n");
	printf("PC: Dr: %lu\n", LC2200->pc->DrPC);
	printf("PC: Ld: %lu\n", LC2200->pc->LdPC);
	printf("PC: pc: %s: %lu\n", wtos(LC2200->pc->pc), LC2200->pc->pc);
	printf("--------\n");
    printf("ALU: Dr: %lu\n", LC2200->alu->DrALU);
    printf("ALU: LdA: %lu\n", LC2200->alu->LdA);
    printf("ALU: LdB: %lu\n", LC2200->alu->LdB);
    printf("ALU: A: %s: %ld\n", wtos(LC2200->alu->A), LC2200->alu->A);
    printf("ALU: B: %s: %ld\n", wtos(LC2200->alu->B), LC2200->alu->B);
	for(w = 0; w < NUM_FUNC; w++) printf("ALU: func %d: %lu\n", w, LC2200->alu->func[w]);
	printf("--------\n");
    printf("REG: Dr: %lu\n", LC2200->reg->DrREG);
    printf("REG: Wr: %lu\n", LC2200->reg->WrREG);
    printf("REG: regno: %lu\n", LC2200->reg->regno);
    if (showres) for(w = 0; w < REG_NUM; w++) printf("REG: reserved %d: %lu\n", w, LC2200->reg->reserved[w]);
    if (showcal) for(w = 0; w < REG_NUM; w++) printf("REG: calleesave %d: %lu\n", w, LC2200->reg->calleesave[w]);
    for(w = 0; w < REG_NUM; w++) printf("REG: reg %d: %s: %lu\n", w, wtos(LC2200->reg->REG[w]), LC2200->reg->REG[w]);
	printf("--------\n");
    printf("MEM: Dr: %lu\n", LC2200->mem->DrMEM);
    printf("MEM: Ld: %lu\n", LC2200->mem->LdMAR);
    printf("MEM: Wr: %lu\n", LC2200->mem->WrMEM);
    printf("MEM: MAR: %s: %lu\n", wtos(LC2200->mem->MAR), LC2200->mem->MAR);
    printf("MEM: MEM 0: %s: %lu\n", wtos(LC2200->mem->MEM[0]), LC2200->mem->MEM[0]);
    for(w = 1; w < showmem; w++) printf("MEM: MEM %d: %s: %lu\n", w, wtos(LC2200->mem->MEM[w]), LC2200->mem->MEM[w]);
    printf("MEM: MEM MAX: %s: %lu\n", wtos(LC2200->mem->MEM[MAX_MEM-1]), LC2200->mem->MEM[MAX_MEM-1]);
    printf("IR: Dr: %lu\n", LC2200->ir->DrOFF);
    printf("IR: Ld: %lu\n", LC2200->ir->LdIR);
    printf("IR: instruction: %s: %lu\n", wtos(LC2200->ir->instruction), LC2200->ir->instruction);
	printf("--------\n");
	printf("--------\n");
	printf("--------\n");
	printf("\n");

}
