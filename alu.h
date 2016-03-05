/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef ALU_H
#define ALU_H
#include "bit.h"

#define NUM_FUNC 8

typedef struct {
	bit DrALU;
	bit LdA;
	bit LdB;
	word A;
	word B;
	bit func[NUM_FUNC];
} arithmeticlogicunit;

typedef arithmeticlogicunit *alu_;

alu_ alu_ctor();
void alu_kill(alu_);

void alu_Dr(alu_);
void alu_LdA(alu_);
void alu_LdB(alu_);

void alu_addb(alu_); //000	a + b
void alu_nand(alu_); //001	a nand b
void alu_asub(alu_); //010	a - b
void alu_ainc(alu_); //011	a + 1
void alu_aorb(alu_); //100	a or b
void alu_andb(alu_); //101	a and b
void alu_axrb(alu_); //110	a xor b
void alu_nota(alu_); //111	not a
#endif
