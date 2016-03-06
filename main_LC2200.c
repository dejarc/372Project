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
	//testSystem(LC2200);
	LC2200->debug = true;
	printf("machine cooloff\n");
	return 0;
}

void testSystem(LC2200_ LC2200) {
//	debug(LC2200);
	LC2200->pc->pc = 0;

	LC2200->reg->REG[3] = stow("0000 0000 0000 0000 0000 0000 0000 1110"); //reg3 starts at 14
	LC2200->mem->MEM[0] = stow("0101 0011 0000 0000 0000 0000 0000 1011"); //beq fail
	LC2200->mem->MEM[1] = stow("0010 0101 0011 1111 1111 1111 1111 1100"); //reg5 =adds -4 to reg3
	LC2200->mem->MEM[2] = stow("0010 1010 0011 0000 0000 0000 0000 0100"); //reg10=adds 4  to reg3
 	LC2200->mem->MEM[3] = stow("0000 0000 0000 0000 0000 0000 0000 0101"); //attempts to add reg5 to $zero
 	LC2200->mem->MEM[4] = stow("0000 1110 0011 0000 0000 0000 0000 0101"); //reg14=reg3+reg5
 	LC2200->mem->MEM[5] = stow("0001 0010 0011 0000 0000 0000 0000 0011"); //reg2=reg3nandreg3
 	LC2200->mem->MEM[6] = stow("0010 0100 0000 0000 0000 0000 0000 0010"); //reg4+=2
 	LC2200->mem->MEM[7] = stow("0011 0110 0100 0000 0000 0000 0000 0100"); //reg6=mem[reg4+4]
 	LC2200->mem->MEM[8] = stow("0100 1010 0011 0000 0000 0000 0000 0101"); //mem[reg3+5]=reg10
 	LC2200->mem->MEM[9] = stow("0010 0100 0100 0000 0000 0000 0000 0001"); //reg4++
 	LC2200->mem->MEM[10]= stow("0011 0010 0100 0000 0000 0000 0000 0000"); //reg2=mem[reg4]
 	LC2200->mem->MEM[11]= stow("0110 0011 1111 0000 0000 0000 0000 0000"); //jump to 14
 	LC2200->mem->MEM[12]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (jumped)
 	LC2200->mem->MEM[13]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (jumped)
 	LC2200->mem->MEM[14]= stow("0100 0110 0011 0000 0000 0000 0000 0001"); //mem[reg3+1] = reg6
 	LC2200->mem->MEM[15]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (overwritten)
 	LC2200->mem->MEM[16]= stow("0101 0000 0000 0000 0000 0000 0000 0011"); //beq->pc18
 	LC2200->mem->MEM[17]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[18]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[19]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (overwritten)
 	LC2200->mem->MEM[20]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[50]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //emergency halt

 	LC2200->debug = true;
 	LC2200->safety = true;
	start(LC2200, 'h');
	LC2200_kill(LC2200);
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
