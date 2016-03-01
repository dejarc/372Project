/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>*/
//#define WORD_LEN 32
#include "asm.h"
char inst[WORD_LEN + 1];
int arg_num;
void instructionHelper(char *input) {
    int opcode = 0;
    int index;
    if(strcmp(input, "add") == 0) {
        opcode = 0;   
    } else if(strcmp(input, "nand") == 0) {
        opcode = 1;
    } else if(strcmp(input, "addi") == 0) {
        opcode = 2;
    } else if(strcmp(input, "sw") == 0) {
        opcode = 4;  
    } else if(strcmp(input, "lw") == 0) {
        opcode = 3; 
    } else if(strcmp(input, "beq") == 0) {
        opcode = 5;
    } else if(strcmp(input, "j") == 0) {
        opcode = 6;
    } 
    
    for(index = WORD_LEN - OPCODE_LEN; index < WORD_LEN; index++) {
        //printf("\nvalue of the %d bit is %d",index, 1 & (opcode >> index));
        if(1 & (opcode >> (index - WORD_LEN + OPCODE_LEN))) { 
            inst[index] = '1';
        } else {
            inst[index] = '0';
        }
    }
}
void argumentHelper(char *input) {
    char *ptr;
    int reg_offset = 0;
    int num_offset = input[2] - 48;
    int index;
    int range;
    if(input[1] == 'a') {
        //printf("the length of the string %d", strlen(input));
        reg_offset = 3;   
        //printf("\nthe number of the string %d", reg_offset + num_offset);
    } else if(input[1] == 't') { 
        reg_offset = 6;
    } else if (input[1] == 's') {
        reg_offset = 9;
    }
    if(num_offset >= 0 && num_offset <= 2) {
        reg_offset += num_offset;
        //printf("\nreg offset %d", reg_offset);
        if(arg_num < 3) {
            range = WORD_LEN - ((arg_num + 1) * (OPCODE_LEN));
        } else {
            range = 0;
        } 
        for(index = range; index < range + OPCODE_LEN; index++) {
            if(1 & (reg_offset >> index - range))
                inst[index] = '1';
        }     
    }
}
void parseInput(char *input) {
    int index;
    int start_bit;
    int imm = (int)strtol(input, NULL, 10); 
    int instruction_one = (input[0] == 'a') | (input[0] == 'b');
    int instruction_two = (input[0] == 's') | (input[0] == 'l');
    int instruction_three = (input[0] == 'n');
    if(!imm) {
        if(instruction_one || instruction_two || instruction_three) {
        //printf("\n argument was detected");
            instructionHelper(input);
        } else if(input[0] == '$') {
            arg_num++;
            argumentHelper(input);
        }
    } else if(arg_num > 0) {
        for(index = 0; index < WORD_LEN; index++) {
            if(1 & (imm >> index))
                inst[index] = '1';
        }
    }   
}
void initializeInst() {
    int index = 0;
    for(index; index < WORD_LEN; index++) {
        inst[index] = '0';
    }
}

char *getInstruction(char *line) {
    char *tokenPtr = strtok(line, " ,;.\n");
    arg_num = 0;
    initializeInst();
    while(tokenPtr != NULL) {
        if(strcmp(tokenPtr,"ORIG") == 0) 
            return "";
        parseInput(tokenPtr);
        tokenPtr = strtok(NULL, " ,;().\n");
    }
    return inst;
}
 
int main(int argc, char **argv) {
    FILE *input_file;
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
    } else {
        input_file = fopen("input.txt", "r");
    }
    char line[256];
    while (fgets(line, sizeof(line), input_file)) {    
        printf("\nthe value of index is %s", getInstruction(line)); 
    } 
    return 0;
}







