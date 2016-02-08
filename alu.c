/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "alu.h"
alu_ alu_ctor() {
    int start;
    alu_ temp_alu = (alu_)malloc(sizeof(struct arithmeticlogicunit));	
    temp_alu->DrALU = false;
	temp_alu->LdA = false;
    temp_alu->LdB = false;
    temp_alu->A = 0;
    temp_alu->B = 0;
    temp_alu->functionResult = 0;
    for(start = 0; start < NUM_FUNCT; start++)
        temp_alu->func[start] = false; 
    return temp_alu;
}
void alu_kill(alu_ my_alu) {
	free(my_alu);
	my_alu = NULL;
}

void alu_Dr(alu_ my_alu) {
	if (my_alu->DrALU) {
	//call function set by fn[]
        int index;
        for(index = 0; index < NUM_FUNCT; index++)
            if(my_alu->func[index])
                break;
        switch(index) {
            case 0:
                alu_addb(my_alu);
                break;
            case 1:
                alu_nand(my_alu); 
                break;
            case 2:
                alu_asub(my_alu);
                break; 
            case 3:
                alu_ainc(my_alu);
                break; 
        }
		bus = my_alu->functionResult;
	}
}

void alu_LdA(alu_ my_alu) {
	if (my_alu->LdA) my_alu->A = bus;
}

void alu_LdB(alu_ my_alu) {
	if (my_alu->LdB) my_alu->B = bus;
}

void alu_addb(alu_ my_alu) {
    my_alu->functionResult = my_alu->A + my_alu->B;
}

void alu_nand(alu_ my_alu) {
    my_alu->functionResult =  ~(my_alu->A & my_alu->B); 
}

void alu_asub(alu_ my_alu) {
    my_alu->functionResult = my_alu->A - my_alu->B;

}

void alu_ainc(alu_ my_alu) {
    my_alu->functionResult = my_alu->A + 1;

}
