#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "bit.h"

struct PRIVATE_GUI;
typedef struct PRIVATE_GUI *Gui;

Gui gui_ctor(word memory_addresses);
void gui_set_mem(Gui gui, word address, word *data);
void gui_main();

#endif // GUI_H_INCLUDED