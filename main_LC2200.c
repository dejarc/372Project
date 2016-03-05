/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include <stdio.h>
#include "LC2200.h"
//#pragma diag_suppress 28

//const int MAX_MEM = 1000000;

void testSystem(LC2200_);

int main(int argc, char *argv[]) {
	printf("ready player one\n");
	LC2200_ LC2200 = LC2200_ctor();
	printf("computer built pronto\n");
	//testBits(LC2200);
	//test(LC2200);	
	testSystem(LC2200);
	printf("machine cooloff\n");
	return 0;
}

void testSystem(LC2200_ LC2200) {
//	debug(LC2200);
	LC2200->pc->pc = 1;
	start(LC2200, 'h');
}
//	int w;
////	printf("%d\n", LC2200->fsm->rom[w]);
//
////	for(w = 0; w < MAX_MEM; w++) printf("MEM: %lu: %lu\n", w, LC2200->mem->memory[w]);
////	w = 29;
////	printf("ROM: %lu", LC2200->fsm->ROM[10]);
//	printf("ROM: mode: %lu\n", LC2200->fsm->mode);
//	printf("ROM: state: %s: %lu\n", wtos(LC2200->fsm->state), LC2200->fsm->state);
//	for (w = 0; w < ROM_SIZE; w++) printf("ROM: rom %lu: %s: %lu\n", w, wtos(LC2200->fsm->rom[w]), LC2200->fsm->rom[w]);
//	printf("PC: Dr: %lu\n", LC2200->pc->DrPC);
//	printf("PC: Ld: %lu\n", LC2200->pc->LdPC);
//	printf("PC: pc: %s: %lu\n", wtos(LC2200->pc->pc), LC2200->pc->pc);
//    printf("ALU: Dr: %lu\n", LC2200->alu->DrALU);
//    printf("ALU: LdA: %lu\n", LC2200->alu->LdA);
//    printf("ALU: LdB: %lu\n", LC2200->alu->LdB);
//    printf("ALU: A: %s: %lu\n", wtos(LC2200->alu->A), LC2200->alu->A);
//    printf("ALU: A: %s: %lu\n", wtos(LC2200->alu->B), LC2200->alu->B);
//	for(w = 0; w < NUM_FUNC; w++) printf("ALU: func %d: %lu\n", w, LC2200->alu->func[w]);
//
////		char s[] = wtos(LC2200->fsm->ROM[w]);
////		printf("ROM %d: %s", w, s);
////	LC2200->mem->memory[0] = stow();
//	printf("--------\n");
////	for(w = 0; w < WORD_LEN; w++) {
////		bit on = bitt(LC2200->fsm->ROM[0], w);
////		printf("%d", on);
////	}
//	printf("\n--------\n");
//
//
//	//start(LC2200, 'x', 1000);
//
//
//}

//void testBits(LC2200_ LC2200) {
//	word w = stow("1001");
//	printf("I am %lu\n", w);
//}
//
//void test(LC2200_ LC2200) {
//	fsm_ fsm = LC2200->fsm;
//	bit on = 0;
//	bit array[20][20] = { { 0 } };
//	word contents = on * on;
//	const bit testbit = false;
//	word testword = on + array[0][0] + contents;
//	testword++;
//	int size = sizeof(bit);
//	int wize = sizeof(word);
//	printf("The bool size is %d and word size is %d and bit is %d", size, wize, testbit);
//
//	printf("My bit is %lu, my word is %lu, and my array is %lu, %lu, %lu\n",
//		    fsm->mode, fsm->state, fsm->ROM[0], fsm->ROM[1], fsm->ROM[2]);
//
//}
