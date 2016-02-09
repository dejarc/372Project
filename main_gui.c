#include "gui.h"

word i = -1;
word j = 32;

void func() {
	i = i + 1;
	j <<= 1;
}

int main() {
    Gui gui = gui_ctor(100);
	gui_connect_memory_address(gui, 3, &j);
	gui_connect_memory_address(gui, 1, &i);
	gui_connect_register(gui, 1, &i);
	gui_connect_run(gui, &func);
	gui_main();
	return 0;
}

