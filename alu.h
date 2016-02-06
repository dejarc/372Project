/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef ALU_H
#define ALU_H
#include "bit.h"

typedef struct {
	bit DrALU;
	bit LdA;
	bit LdB;
	bit func[2];
	word A;
	word B;
} arithmeticlogicunit;

typedef arithmeticlogicunit *alu_;

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
