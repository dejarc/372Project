#include "gui.h"


int main() {
	Gui gui = gui_ctor(100);
	word i = 99999;
	word j = 90;
	gui_set_mem(gui, 3, &j);
	gui_set_mem(gui, 1, &i);
	gui_main();
	return 0;
}