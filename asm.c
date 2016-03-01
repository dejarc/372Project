/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef BIT_H
#define BIT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "bit.h"
#define OPCODE_LEN 4
#define WORD_LEN 32
char inst[WORD_LEN + 1];
int arg_num;
void instructionHelper(char *input) {
    int opcode = 0;
    int index;
    if(strcmp(input, "add") == 0) {
        opcode = 0;   
    } else if(strcmp(input, "addi") == 0) {
        opcode = 2;
    } else if(strcmp(input, "beq") == 0) {
        opcode = 5;
    } else if(strcmp(input, "j") == 0) {
        opcode = 6;
    } 
    
    for(index = 0; index < OPCODE_LEN; index++) {
        //printf("\nvalue of the %d bit is %d",index, 1 & (opcode >> index));
        if(1 & (opcode >> index)) { 
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
    int is_num = (input[2] > 47) & (input[2] < 58);  
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
    if(is_num) {
        reg_offset += num_offset;
        //printf("\nreg offset %d", reg_offset);
        if(arg_num < 3) {
            range = arg_num * OPCODE_LEN;
        } else {
            range = WORD_LEN - OPCODE_LEN;
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
    if(!imm) {
        if(input[0] == 'a' || input[0] == 'b') {
        //printf("\n argument was detected");
            instructionHelper(input);
        } else if(input[0] == '$') {
            arg_num++;
            argumentHelper(input);
        }
    } else {
        printf("\nnumber is %d", imm);
        start_bit = WORD_LEN - 20;
        for(index = start_bit; index < WORD_LEN; index++) {
            if(1 & (imm >> index - start_bit))
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
int main() {
    FILE *input_file;
    input_file = fopen("input.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), input_file)) {    
        char *tokenPtr = strtok(line, " ,;\n");
        arg_num = 0;
        initializeInst();
        while(tokenPtr != NULL) {
            parseInput(tokenPtr);
            //printf("\n");
            //printf("%s", tokenPtr);
            tokenPtr = strtok(NULL, " ,;\n");
        }
        printf("\nthe value of index is %s", inst); 
    } 
    return 0;
}






#endif

