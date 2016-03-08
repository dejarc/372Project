/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADERS */
#include "LC2200.h"
#include "asm.h"
#include "gui.h"
#include "test.h"

/* The LC2200
 * *****************************************************************************
 * Static global reference for the computer itself.
 */
static LC2200_ LC2200;

/* STEP
 * -----------------------------------------------------------------------------
 * Runs one instruction.
 */
static void step() {
	start(LC2200, 's');
}

/* RUN
 * -----------------------------------------------------------------------------
 * Runs until halt.
 */
static void run() {
	start(LC2200, 'h');
	//
}

/* JUMP OR BEQ
 * -----------------------------------------------------------------------------
 * Runs until a jump or beq is reached.
 */
static void jumporbeq() {
	start(LC2200, 'j');
}

/* BREAKPOINT
 * -----------------------------------------------------------------------------
 * Runs until predefined breakpoint.
 */
static void breakpoint(word breakpoint) {
	LC2200->cycle = breakpoint;
	start(LC2200, 'b');
}

/* OPEN FILE
 * -----------------------------------------------------------------------------
 * Opens the file.
 */
static void open_file(FILE *file) {
	printf("%d", fgetc(file));
    //
    //
}

/* UPLOAD ASM
 * -----------------------------------------------------------------------------
 * Takes a file and converts it to binary, then uploads that into machine memory.
 */
static void uploadASM(LC2200_ LC2200, char* input) { //only works with old vertion
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

/* *****************************************************************************
 * main
 * *****************************************************************************
 * Creates the LC2200, sets up the GUI and connects functions to it, and then
 * launches the GUI connected to the ASM and LC2200.
 */
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

	LC2200_kill(LC2200);

	return 0;
}
