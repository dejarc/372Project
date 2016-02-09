#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "bit.h"

struct PRIVATE_GUI;
typedef struct PRIVATE_GUI *Gui;

Gui gui_ctor(word memory_addresses);
void gui_connect_memory_address(Gui gui, word address, word *data);
void gui_connect_register(Gui gui, int num, word *data);
void gui_connect_run(Gui gui, void (*run)());
void gui_connect_step(Gui gui, void (*step)());
void gui_main();

#endif // GUI_H_INCLUDED