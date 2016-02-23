/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include <stdio.h>
#include "LC2200.h"


int main(int argc, char *argv[]) {
	LC2200_ LC2200 = LC2200_ctor();
	printf("computer built pronto");
	//testBits(LC2200);
	//test(LC2200);
	return 0;
}

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
