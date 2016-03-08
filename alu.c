/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER */
#include "alu.h"

/* *****************************************************************************
 * arithmeticlogicunit (alu)
 * *****************************************************************************
 * Responsible for performing all of the mathematical operations in the entire
 * computer. The alu contains two dedicated special purpose signed word
 * registers for use in these calculations and a three-bit function multiplexor
 * for determining which calculation to do.
 */
alu_ alu_ctor() {
    int func;
    arithmeticlogicunit *alu = malloc(sizeof(arithmeticlogicunit));
    alu->DrALU = false;
	alu->LdA = false;
    alu->LdB = false;
    alu->A = 0;
    alu->B = 0;
	for(func = 0; func < NUM_FUNC; func++)
		alu->func[func] = false;
	return alu;
}

/* KILL
 * -----------------------------------------------------------------------------
 * Frees and nullifies the alu.
 */
void alu_kill(alu_ alu) {
	free(alu);
	alu = NULL;
}

/* DRIVE
 * -----------------------------------------------------------------------------
 * If the drive signal is enabled, performs the operation indicated by regno
 * and pushes the result to the bus.
 */
void alu_Dr(alu_ alu) {
	if (alu->DrALU) {
        int index;
        for(index = 0; index < NUM_FUNC; index++)
            if(alu->func[index]) //call function set by fn[]
                break;
        switch(index) {
            case 0:	alu_addb(alu); break;
            case 1: alu_nand(alu); break;
            case 2: alu_asub(alu); break;
            case 3: alu_ainc(alu); break;
            case 4: alu_aorb(alu); break;
            case 5: alu_andb(alu); break;
            case 6: alu_axrb(alu); break;
            case 7: alu_nota(alu); break;
            default:			   break;
        }
	}
}

/* LOAD A
 * -----------------------------------------------------------------------------
 * If the load A signal is enabled, updates the A register with the bus value.
 */
void alu_LdA(alu_ alu) {
	if (alu->LdA) alu->A = bus;
}

/* LOAD B
 * -----------------------------------------------------------------------------
 * If the load B signal is enabled, updates the B register with the bus value.
 */
void alu_LdB(alu_ alu) {
	if (alu->LdB) alu->B = bus;
}

/* A + B
 * -----------------------------------------------------------------------------
 */
void alu_addb(alu_ alu) {
    bus = alu->A + alu->B;
}

/* A nand B
 * -----------------------------------------------------------------------------
 */
void alu_nand(alu_ alu) {
    bus =  ~(alu->A & alu->B);
}

/* A - B
 * -----------------------------------------------------------------------------
 */
void alu_asub(alu_ alu) {
    bus = alu->A - alu->B;
}

/* A + 1
 * -----------------------------------------------------------------------------
 */
void alu_ainc(alu_ alu) {
    bus = alu->A + 1;
}

/* A or B
 * -----------------------------------------------------------------------------
 */
void alu_aorb(alu_ alu) {
	bus = alu->A | alu->B;
}

/* A and B
 * -----------------------------------------------------------------------------
 */
void alu_andb(alu_ alu) {
	bus = alu->A & alu->B;
}

/* A xor B
 * -----------------------------------------------------------------------------
 */
void alu_axrb(alu_ alu) {
	bus = alu->A ^ alu->B;
}

/* not A
 * -----------------------------------------------------------------------------
 */
void alu_nota(alu_ alu) {
	bus = ~(alu->A);
}
