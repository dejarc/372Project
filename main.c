/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */


#include "LC2200.h"
#include "asm.h"
#include "gui.h"

static LC2200_ LC2200;

static void step() {
	start(LC2200, 's');
}

static void run() {
	start(LC2200, 'h');
}

static void open_file(FILE *file) {
	printf("%d", fgetc(file));
    //
    //
}

void uploadASM(LC2200_ LC2200, char* input) { //only works with old vertion
	FILE *input_file;
    char **binary;
    int rows = 0;
	input_file = fopen(input, "r");
    binary = getAllInstructions(input_file);
    while(binary[rows] != NULL) {
        printf("\nassembly instruction at row %d is %s", rows, binary[rows]);
        rows++;
    }
    fclose(input_file);
}

int main(int argc, char *argv[]) {
	LC2200 = LC2200_ctor();

//	testSystem(LC2200);

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

	LC2200_kill(LC2200);

	return 0;
}
