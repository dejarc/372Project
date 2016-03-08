/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef REG_H
#define REG_H

/* BIT HEADER */
#include "bit.h"

/* MACROS */
#define REG_LOG 4	//number of functions
#define REG_NUM 16	//number of registers

/* registerfile (reg) struct
 * -----------------------------------------------------------------------------
 * Contains the drive and write bits as well as a register for holding the
 * the currently selected register, two parallel arrays of bit switches for
 * determining writability and callee-save status, which are parallel to the
 * registers themselves, REG.
 */
typedef struct {
	bit DrREG;
	bit WrREG;
	word regno;
	bit reserved[REG_NUM];
	bit calleesave[REG_NUM];
	word REG[REG_NUM];
} registerfile;

/* POINTER */
typedef registerfile *reg_;

/* CONSTRUCTOR/DESTRUCTOR */
reg_ reg_ctor();
void reg_kill(reg_);

/* BUS METHODS */
void reg_Dr(reg_);
void reg_Wr(reg_);

/* SPECIAL METHODS */
//note: both methods unimplemented
void reg_writeReserved(reg_);
word save(reg_);

#endif
