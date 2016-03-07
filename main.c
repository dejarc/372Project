/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "LC2200.h"
#include "asm.h"
#include "gui.h"


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
	//start(LC2200, 'h');
}

static LC2200_ LC2200;

static void step() {
	start(LC2200, 's');
}

static void run() {
	start(LC2200, 'h');
}

static void open_file(FILE *file) {
    //
	printf("%d", fgetc(file));
    //
    //
}

int main(int argc, char *argv[]) {
	LC2200 = LC2200_ctor();

	testSystem(LC2200);

	Gui gui = gui_ctor();
	gui_connect_pc(gui, &LC2200->pc->pc);
	//gui_connect_memory(gui, LC2200->mem->MEM, MAX_MEM);
    gui_connect_asm_print(gui, &asm_print);
	gui_connect_memory(gui, LC2200->mem->MEM, 100);
	gui_connect_registers(gui, LC2200->reg->REG, REG_NAMES, REG_NUM);
	gui_connect_step(gui, &step);
	gui_connect_run(gui, &run);
	gui_connect_open_file(gui, &open_file);
	gui_main();

	return 0;
}