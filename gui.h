/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <stdio.h>
#include "bit.h"

struct PRIVATE_GUI;
typedef struct PRIVATE_GUI *Gui;

Gui gui_ctor();
void gui_connect_memory(Gui gui, word data[], word count);
void gui_connect_registers(Gui gui, word data[], const char *names[], word count);
void gui_connect_pc(Gui gui, word *pc);
void gui_connect_run(Gui gui, void (*run)(void));
void gui_connect_step(Gui gui, void (*step)(void));
void gui_connect_open_file(Gui gui, void (*open_file)(FILE *));
void gui_main();

#endif // GUI_H_INCLUDED