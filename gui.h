#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "bit.h"

struct PRIVATE_GUI;
typedef struct PRIVATE_GUI *Gui;

Gui gui_ctor();
void gui_connect_memory(Gui gui, word data[], word count);
void gui_connect_registers(Gui gui, word data[], const char *names[], word count);
void gui_connect_run(Gui gui, void (*run)());
void gui_connect_step(Gui gui, void (*step)());
void gui_main();

#endif // GUI_H_INCLUDED