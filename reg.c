/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "reg.h"

reg_ reg_ctor() {
    int regfile;
    registerfile *reg = malloc(sizeof(registerfile));
    reg->DrREG = false;
    reg->WrREG = false;
	for(regfile = 0; regfile < REG_LOG; regfile++)
		reg->regno[regfile] = false;

	//					$zero  $at	  $v0	 $a0	$a1	   $a2	  $t0	 $t1
	bit reserved[] = {	true,  true,  false, false, false, false, false, false,

	//					$t2	   $s0    $s1	 $s2	$k0	   $sp	  $fp	 $ra
						false, false, false, false, true,  false, false, false,
	};

	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->reserved[regfile] = reserved[regfile];

	//					$zero  $at	  $v0	 $a0	$a1	   $a2	  $t0	 $t1
	bit calleesave[] = {false, false, false, false, false, false, false, false,

	//					$t2	   $s0    $s1	 $s2	$k0	   $sp	  $fp	 $ra
						false, true,  true,  true,  false, false, true,  false,
	};

	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->calleesave[regfile] = calleesave[regfile];

	for(regfile = 0; regfile < REG_NUM; regfile++)
		reg->registers[regfile] = 0;

	return reg;

}

void reg_kill(reg_ reg) {
	free(reg);
	reg = NULL;
}

void reg_Dr(reg_ reg) {
	if (reg->DrREG) {
		//get register indicated and bus = reg;
	}
}
void reg_Wr(reg_ reg) {
	if (reg->WrREG) {
		//write to indicated reegister from bus;
	}
}

void reg_writeReserved(reg_);
word save(reg_);
