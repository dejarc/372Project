#include "gui.h"

word i = -1;
word j = 32;

void func() {
	i = i + 1;
	j <<= 1;
}

const char *REG_NAMES[] = {"$zero", "$at", "$v0", "$a0", 
                                  "$a1", "$a2", "$t0", "$t1", 
                                  "$t2", "$s0", "$s1", "$s2",
                     		      "$k0", "$sp", "$fp", "$ra"};

int main() {
    Gui gui = gui_ctor(100);
    word g[100] = {0};
    g[2] = 100;
    word r[16] = {0};
    gui_connect_memory(gui, g, 100);
    gui_connect_registers(gui, r, REG_NAMES, 16);
    gui_connect_run(gui, &func);
    gui_main();
    return 0;
}

