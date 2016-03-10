/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <stdio.h> // for FILE type
#include "bit.h"   // for word type

/* define Gui to be a pointer to a hidden struct that is defined in the .c file */
struct PRIVATE_GUI;
typedef struct PRIVATE_GUI *Gui;

/* initializes and returns a new Gui */
Gui gui_ctor();

/************************************************************************************
 * methods to connect data */
void gui_connect_memory(Gui gui, word data[], word count);
void gui_connect_registers(Gui gui, word data[], const char *names[], word count);
void gui_connect_pc(Gui gui, word *pc);
/************************************************************************************/

/************************************************************************************
 * methods to connect function callbacks */

/* connect function called when the run button is pressed */
void gui_connect_run(Gui gui, void (*run)(void));

/* connect function called when the step button is pressed */
void gui_connect_step(Gui gui, void (*step)(void));

/* connect function called after the given file is chosen to be opened */
void gui_connect_open_file(Gui gui, void (*open_file)(FILE *));

/* connect function called to print the given word as assembly into the given char buffer */
void gui_connect_asm_print(Gui gui, void (*asm_print)(char[], word));
/************************************************************************************/

/* shows the gui and runs until it is closed */
void gui_main();

#endif // GUI_H_INCLUDED