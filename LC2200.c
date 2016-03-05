/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "LC2200.h"

word bus;

LC2200_ LC2200_ctor() {
	littlecomputer2200 *LC2200 = malloc(sizeof(littlecomputer2200));
	LC2200->clock = false;
	LC2200->fsm = fsm_ctor();
	LC2200->pc = pc_ctor();
	LC2200->alu = alu_ctor();
	LC2200->reg = reg_ctor();
	LC2200->mem = mem_ctor();
	LC2200->ir = ir_ctor();
//			{ 0,
//									 fsm_ctor()
////									 ,
////									 pc_ctor(),
////									 alu_ctor(),
////									 reg_ctor(),
////								     mem_ctor(),
////									 ir_ctor()
//								};
//	LC2200_ lcp = &LC2200;
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

void start(LC2200_ LC2200, char mode, word cycles) {
	//once fully coded, char will be modes of single step, loop step,
	//										  run-until-breakpoint,
	//									      run-until-label,
	//									      run-until-halt
	int tick = 0;
//	for(;tick<cycles;tick++) cycle(LC2200);
}

void cycle(LC2200_ LC2200) {
	int bit;
	int nextbit;
	word *rom = LC2200->fsm->ROM;
	word state = LC2200->fsm->state;
	char next[10] = { ' ' };

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
			case Z_LD:		LC2200->ir->DrOFF	= bitt(rom[state], OFF_DR);	break;
		}

	for (bit = MEM_WR; bit <= REG_WR; bit++)
		switch(bit) {
			case MEM_WR:	LC2200->mem->WrMEM	= bitt(rom[state], MEM_WR);	break;
			case REG_WR:	LC2200->reg->WrREG	= bitt(rom[state], REG_WR);	break;
		}
	for (bit = 0; bit < NUM_FUNC; bit++) LC2200->alu->func[bit] = false;
	LC2200->alu->func[bits(rom[state], ALU_FN, ALU_FN+2)] = true;

	LC2200->reg->regno = ir_reg(LC2200->ir, bits(rom[state], IR_REG, IR_REG+1));

	if (bitt(rom[state], S_O)) for (bit = OP_CODE, nextbit = 0; bit < OP_CODE+4; bit++, nextbit++) next[nextbit] = bitt(rom[state], bit);
	if (bitt(rom[state], S_Z)) next[5] = bitt(rom[state], Z_VALUE);
	if (bitt(rom[state], S_S)) for (bit = MICRO_STATE, nextbit = 0; bit < MICRO_STATE+5; bit++, nextbit++) next[nextbit] = bitt(rom[state], bit);

	if (bitt(rom[state], CALLEE_SAVE)) //initiatecalleesave
		4 + 54;


	// check for next microstate to advance to
	//	LC2200->fsm->state++;

	//line to read previous and update next state
	//line or whatever to set next finite state machine state
//	setup(LC2200);
	microstate(LC2200);
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
					case Wr:						break;
				}; break;
//				case _bus:	switch(signal) {
//					case Dr:
//					case Ld: reg_Ld(LC2200->reg);	break;
//					case Wr: 						break;
//				}; break;
			}
}

void debug(LC2200_ LC2200) {
	int w;
	printf("--------\n");
	printf("ROM: mode: %lu\n", LC2200->fsm->mode);
	printf("ROM: state: %s: %lu\n", wtos(LC2200->fsm->state), LC2200->fsm->state);
	for (w = 0; w < ROM_SIZE; w++) printf("ROM: rom %lu: %s: %lu\n", w, wtos(LC2200->fsm->ROM[w]), LC2200->fsm->ROM[w]);
	printf("--------\n");
	printf("PC: Dr: %lu\n", LC2200->pc->DrPC);
	printf("PC: Ld: %lu\n", LC2200->pc->LdPC);
	printf("PC: pc: %s: %lu\n", wtos(LC2200->pc->pc), LC2200->pc->pc);
	printf("--------\n");
    printf("ALU: Dr: %lu\n", LC2200->alu->DrALU);
    printf("ALU: LdA: %lu\n", LC2200->alu->LdA);
    printf("ALU: LdB: %lu\n", LC2200->alu->LdB);
    printf("ALU: A: %s: %lu\n", wtos(LC2200->alu->A), LC2200->alu->A);
    printf("ALU: A: %s: %lu\n", wtos(LC2200->alu->B), LC2200->alu->B);
	for(w = 0; w < NUM_FUNC; w++) printf("ALU: func %d: %lu\n", w, LC2200->alu->func[w]);
	printf("--------\n");
    printf("REG: Dr: %lu\n", LC2200->reg->DrREG);
    printf("REG: Wr: %lu\n", LC2200->reg->WrREG);
    printf("REG: regno: %lu\n", LC2200->reg->regno);
//    for(w = 0; w < REG_LOG; w++) printf("REG: regno %d: %lu\n", w, LC2200->reg->regno[w]);
    for(w = 0; w < REG_NUM; w++) printf("REG: reserved %d: %lu\n", w, LC2200->reg->reserved[w]);
    for(w = 0; w < REG_NUM; w++) printf("REG: calleesave %d: %lu\n", w, LC2200->reg->calleesave[w]);
    for(w = 0; w < REG_NUM; w++) printf("REG: reg %d: %s: %lu\n", w, wtos(LC2200->reg->registers[w]), LC2200->reg->registers[w]);
	printf("--------\n");
    printf("MEM: Dr: %lu\n", LC2200->mem->DrMEM);
    printf("MEM: Ld: %lu\n", LC2200->mem->LdMAR);
    printf("MEM: Wr: %lu\n", LC2200->mem->WrMEM);
    printf("MEM: MAR: %s: %lu\n", wtos(LC2200->mem->MAR), LC2200->mem->MAR);
    for(w = 0; w < 100; w++) printf("MEM: MEM %d: %s: %lu\n", w, wtos(LC2200->mem->memory[w]), LC2200->mem->memory[w]);
    printf("MEM: MEM MAX: %s: %lu\n", wtos(LC2200->mem->memory[MAX_MEM-1]), LC2200->mem->memory[MAX_MEM-1]);
    printf("IR: Dr: %lu\n", LC2200->ir->DrOFF);
    printf("IR: Ld: %lu\n", LC2200->ir->LdIR);
    printf("IR: instruction: %s: %lu\n", wtos(LC2200->ir->instruction), LC2200->ir->instruction);
	printf("--------\n");
	printf("%s\n", wtos(LC2200->fsm->ROM[0]));
	printf("%s\n", wtos(bits(LC2200->fsm->ROM[0], 16, 30)));
	for(w = 0; w < WORD_LEN; w++)
	printf("%d", bitt(LC2200->fsm->ROM[0], w));

	printf("\n");
	//start(LC2200, 'x', 1000);
	w = 0;

}
