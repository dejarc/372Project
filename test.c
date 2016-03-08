/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include <stdio.h>
#include "LC2200.h"
#include "asm.h"
#include "gui.h"
//#include "main.h"


int testLC2200main(int argc, char *argv[]) {
	printf("ready player one\n");
	LC2200_ LC2200 = LC2200_ctor();
	printf("computer built pronto\n");
//	uploadASM(LC2200, "input.txt");
//	testSystem(LC2200);
	LC2200->microdebug = true;
	printf("machine cooloff\n");
	return 0;
}

int testBitmain(int argc, char *argv[]) {
    word num = 1;
    char start = 0;
    char end = 31;
    printf("enter a number: ");
    scanf("%lu", (unsigned long*)num);
    printf("enter a start bit: ");
//    scanf("%ms", start);
    printf("enter an end bit: ");
//    scanf("%ms", end);
    printf("the value of the specified bits are: %lu", (unsigned long)bits(num,start, end));
    return 0;
}

int testGUImain(int argc, char *argv[]) {
    Gui gui = gui_ctor();
    word mem[100] = {0};
    mem[2] = 99;
    word r[16] = {0};
    r[4] = -1;
    gui_connect_memory(gui, mem, 100);
    gui_connect_registers(gui, r, REG_NAMES, 16);
//    gui_connect_step(gui, &func);
//    gui_connect_pc(gui, &pc);
//    gui_connect_open_file(gui, &open_file);
    gui_main();
    return 0;
}

void testSystem(LC2200_ LC2200) {

	debug(LC2200);
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
 	LC2200->mem->MEM[16]= stow("0101 0000 0000 0000 0000 0000 0000 0011"); //beq->pc19
 	LC2200->mem->MEM[17]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[18]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[19]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt (overwritten)
 	LC2200->mem->MEM[20]= stow("0110 0000 0000 0000 0000 0000 0001 1000"); //jump pc24
 	LC2200->mem->MEM[21]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[22]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[23]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[24]= stow("0010 1000 1000 0000 0000 0000 0100 0000"); //reg8=reg8+64
	LC2200->mem->MEM[25]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt
 	LC2200->mem->MEM[50]= stow("0111 0000 0000 0000 0000 0000 0000 0000"); //emergency halt

// 	LC2200->safetydebug = true;
 	LC2200->statedebug = true;
// 	LC2200->microdebug = true;
//	start(LC2200, 'h');
}


void testBits(LC2200_ LC2200) {
	word w = stow("1001");
	printf("I am %lu\n", w);
}

void testBasicStates(LC2200_ LC2200) {
	fsm_ fsm = LC2200->fsm;
	bit on = 0;
	bit array[20][20] = { { 0 } };
	word contents = on * on;
	const bit testbit = false;
	word testword = on + array[0][0] + contents;
	testword++;
	int size = sizeof(bit);
	int wize = sizeof(word);
	printf("The bool size is %d and word size is %d and bit is %d", size, wize, testbit);

	printf("My bit is deleted, my word is %lu, and my array is %lu, %lu, %lu\n",
		     fsm->state, fsm->ROM[0], fsm->ROM[1], fsm->ROM[2]);

}
