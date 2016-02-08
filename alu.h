/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef ALU_H
#define ALU_H
#include "bit.h"
#define NUM_FUNCT 4
typedef struct arithmeticlogicunit *alu_;
struct arithmeticlogicunit {
	bit DrALU;
	bit LdA;
	bit LdB;
	bit func[NUM_FUNCT];
	word A;
	word B;
    word functionResult;
};


alu_ alu_ctor();
void alu_kill(alu_);

void alu_Dr(alu_);
void alu_LdA(alu_);
void alu_LdB(alu_);

void alu_addb(alu_);
void alu_nand(alu_);
void alu_asub(alu_);
void alu_ainc(alu_);

#endif
