/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "alu.h"
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
//    for(start = 0; start < NUM_FUNCT; start++)
//        alu->func[start] = false;
//
//
//
//    return temp_alu;
//	int func;
//	arithmeticlogicunit alu = { 0 };
//	alu_ alup = &alu;
//	for(func = 0; func < NUM_FUNC; func++)
//		alup->func[func] = false;
//	return alup;
}
void alu_kill(alu_ alu) {
	free(alu);
	alu = NULL;
}

void alu_Dr(alu_ alu) {
	if (alu->DrALU) {
	//call function set by fn[]
        int index;
        for(index = 0; index < NUM_FUNC; index++)
            if(alu->func[index])
                break;
        switch(index) {
            case 0:
                alu_addb(alu);
                break;
            case 1:
                alu_nand(alu);
                break;
            case 2:
                alu_asub(alu);
                break; 
            case 3:
                alu_ainc(alu);
                break; 
            default: break;
        }
	}
}

void alu_LdA(alu_ alu) {
	if (alu->LdA) alu->A = bus;
}

void alu_LdB(alu_ alu) {
	if (alu->LdB) alu->B = bus;
}

void alu_addb(alu_ alu) {
    bus = alu->A + alu->B;
}

void alu_nand(alu_ alu) {
    bus =  ~(alu->A & alu->B);
}

void alu_asub(alu_ alu) {
    bus = alu->A - alu->B;
}

void alu_ainc(alu_ alu) {
    bus = alu->A + 1;
}

void alu_aorb(alu_ alu) {
	bus = alu->A | alu->B;
}

void alu_andb(alu_ alu) {
	bus = alu->A & alu->B;
}

void alu_axrb(alu_ alu) {
	bus = alu->A ^ alu->B;
}

void alu_nota(alu_ alu) {
	bus = ~(alu->A);
}
