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
static void open_file(FILE *inputfile) {
	FILE *asmfile = fopen("asm_convert", "w");

	FILE *parsefile;
//	printf("%d", fgetc(file));
    char **binary;
    char lastline[WORD_LEN+1];
    char line[WORD_LEN+1];
    char data[WORD_LEN+1];
    int row = 0;
    int col = 0;
//    char *line = NULL;
//    size_t len = 0;
//    ssize_t read;
    int r;
    int c;
    int plusone = 0;
    int rows = 0;
    if (inputfile) {
        while ((c = getc(inputfile)) != EOF) {
        	if (c < 128)
        		fprintf(asmfile, "%c", c);
//        	printf("%d\n",c);
        }
//            putchar(c);
        fclose(inputfile);
        fclose(asmfile);
    }
    printf("------------------------------\n");
    parsefile = fopen("asm_convert", "r");
//    if (parsefile) {
//        while ((c = getc(parsefile)) != EOF) {
////        	if (c < 128)
////        		fprintf(asmfile, "%c", c);
//        	printf("%d\n",c);
////            putchar(c);
//        }
//        fclose(inputfile);
//        fclose(asmfile);
//    }
//    if (true) return;
    binary = getAllInstructions(parsefile, &row, &col);
//    if (true) return;

    printf("%d %d\n", row, col);

//    while ((read = getline(&line, &len, )) != -1) {
//        printf("Retrieved line of length %zu :\n", read);
//        printf("%s", line);
//    }
    for (c = WORD_LEN; c < WORD_LEN * 2; c++) {
    	line[c-WORD_LEN] = binary[0][c];
    }
    line[WORD_LEN] = 0;
    printf("LINE:%s\n", line);
	LC2200->pc->pc = stow(line);
	printf("PC:%lu %s\n", LC2200->pc->pc, wtos(LC2200->pc->pc));
	for (c = 0; c < WORD_LEN ; c++) {
	    	lastline[c] = '1';
	    }
//	lastline = "11111111111111111111111111111111";
	lastline[WORD_LEN] = 0;
//    if (true) return;
    for (r = 1; r < row; r++) {
        for (c = 0; c < WORD_LEN; c++) {
        	line[c] = binary[r][c];
        }

        line[WORD_LEN] = 0;
//        line
//        if (stow(line) == stow(lastline)) plusone = 1;
//        else plusone = 0;
////    	LC2200->pc->pc = stow(line);
//        for (c = 0; c < WORD_LEN ; c++) {
//        	    	lastline[c] = bitt(wtos(stow(line) + plusone), c)+'0';
//        	    }
//
//        for (c = 0; c < WORD_LEN ; c++) {
//        	    	line[c] = bitt(lastline, c)+'0';
//        	    }
////        lastline = wtos(stow(line) + plusone);
//
        printf("LINE:%s\n", line);

        for (c = WORD_LEN; c < WORD_LEN * 2; c++) {
        	data[c-WORD_LEN] = binary[r][c];
        }
        data[WORD_LEN] = 0;
        printf("DATA:%s\n", data);
    	LC2200->mem->MEM[stow(line)+plusone] = stow(data);
    	printf("MEM:%lu %s\n", LC2200->mem->MEM[stow(line)+plusone], wtos(LC2200->mem->MEM[stow(line)+plusone]));
//    while(binary[rows] != NULL) {
    	printf("012345678901234567890123456789012345678901234567890123456789012345\n");
        printf("\nassembly instruction at row %d is %s\n", r, binary[r]);
        plusone++;
//        rows++;
    }
    LC2200->mem->MEM[stow(line)+plusone+1] =
    		stow("0111 0000 0000 0000 0000 0000 0000 0000"); //halt

    fclose(parsefile);
}

/* *****************************************************************************
 * main
 * *****************************************************************************
 * Creates the LC2200, sets up the GUI and connects functions to it, and then
 * launches the GUI connected to the ASM and LC2200.
 */
int main(int argc, char *argv[]) {
	LC2200 = LC2200_ctor();

//	testSystem(LC2200);

	Gui gui = gui_ctor();
	gui_connect_pc(gui, &LC2200->pc->pc);
	//gui_connect_memory(gui, LC2200->mem->MEM, MAX_MEM);
    gui_connect_asm_print(gui, &asm_print);
	gui_connect_memory(gui, LC2200->mem->MEM, (int)(MAX_MEM/10));
	gui_connect_registers(gui, LC2200->reg->REG, REG_NAMES, REG_NUM);
	gui_connect_step(gui, &step);
	gui_connect_run(gui, &run);
	gui_connect_open_file(gui, &open_file);
	gui_main();

	LC2200_kill(LC2200);

	return 0;
}
