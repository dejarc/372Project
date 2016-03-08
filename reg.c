/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "reg.h"

/* SYSTEM WIDE DECLARATION OF REGISTER NAMES */
const char *REG_NAMES[] = {
    //0		1	  	2		3		4	   	5	  	6		7
	"$zero","$at",	"$v0",	"$a0",	"$a1",	"$a2",	"$t0",	"$t1",

    //8	   	9	  	10	 	11		12	   	13	  	14	 	15
    "$t2",	"$s0",	"$s1",	"$s2",	"$k0",	"$sp",	"$fp",	"$ra"
};

/* *****************************************************************************
 * registerfile (reg)
 * *****************************************************************************
 * The heart of the LC2200 is the registerfile, a group of 16 registers that
 * are the only components the cpu can interact with. Each of the registers has
 * a specific index, name, and write and read controls. The parallel arrays
 * of reserved bits and callee-save bits indicate which registers are
 * write-protected and save-protected respectively. The index of the current
 * register activated is determined by a regno multiplexor.
 */
reg_ reg_ctor() {
    int regfile;
    registerfile *reg = malloc(sizeof(registerfile));
    reg->DrREG = false;
    reg->WrREG = false;
    reg->regno = 0;
    //					0	   1	  2		 3		4	   5	  6		 7
	//					$zero  $at	  $v0	 $a0	$a1	   $a2	  $t0	 $t1
	bit reserved[] = {	true,  true,  false, false, false, false, false, false,

    //					8	   9	  10	 11		12	   13	  14	 15
	//					$t2	   $s0    $s1	 $s2	$k0	   $sp	  $fp	 $ra
						false, false, false, false, true,  false, false, false,
	};
	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->reserved[regfile] = reserved[regfile];
    //					0	   1	  2		 3		4	   5	  6		 7
	//					$zero  $at	  $v0	 $a0	$a1	   $a2	  $t0	 $t1
	bit calleesave[] = {false, false, false, false, false, false, false, false,

    //					8	   9	  10	 11		12	   13	  14	 15
	//					$t2	   $s0    $s1	 $s2	$k0	   $sp	  $fp	 $ra
						false, true,  true,  true,  false, false, true,  false,
	};
	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->calleesave[regfile] = calleesave[regfile];
	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->REG[regfile] = 0;
	return reg;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the reg.
 */
void reg_kill(reg_ reg) {
	free(reg);
	reg = NULL;
}

/* DRIVE
 * -----------------------------------------------------------------------------
 * If the drive signal is enabled, the bus is updated to contain the register
 * value indicated by regno.
 */
void reg_Dr(reg_ reg) {
	if (reg->DrREG) bus = reg->REG[reg->regno];
}

/* WRITE
 * -----------------------------------------------------------------------------
 * If the write signal is enabled and the respective reserved array indicates
 * the register at regno is write-enabled, updates that regsiter with the bus.
 */
void reg_Wr(reg_ reg) {
	if (reg->WrREG) if (!reg->reserved[reg->regno]) reg->REG[reg->regno] = bus;
}

/* WRITE RESERVED
 * -----------------------------------------------------------------------------
 * Unimplemented.
 */
void reg_writeReserved(reg_ reg) {
	//unimplemented
}

/* CALLE SAVE
 * -----------------------------------------------------------------------------
 * Unimplemented.
 */
word save(reg_ reg) {
	//unimplemented
	return 0;
}
