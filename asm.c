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
#define NO_BR -9999
#define IMM_VAL 20
#define MAX_ROWS 5
#define HEX_CONST 16

int arg_num;
char br_command[9];
int br_offset;
int hex_input;
void instructionHelper(char *input, char *line) {
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
    } else if(strcmp(input, "halt") == 0){
        opcode = 7;
    } 
    
    for(index = WORD_LEN - OPCODE_LEN; index < WORD_LEN; index++) {
        //printf("\nvalue of the %d bit is %d",index, 1 & (opcode >> index));
        if(1 & (opcode >> (index - WORD_LEN + OPCODE_LEN))) { 
            line[index] = '1';
        } else {
            line[index] = '0';
        }
    }
    for(index = 0; index < IMM_VAL; index++) {
        if((strcmp(input, "beq") == 0 && (1 & (br_offset >> index))) && br_offset > 0) {
            line[index] = '1';
        }
    }   
}
void argumentHelper(char *input, char* line) {
    char *ptr;
    int reg_offset = 0;
    int num_offset = input[2] - 48;
    int index;
    int range;
    if(input[1] == 'a') {
        reg_offset = 3;   
    } else if(input[1] == 't') { 
        reg_offset = 6;
    } else if (input[1] == 's') {
        reg_offset = 9;
        if(input[2] == 'p') {
            reg_offset = 13;
        }
    } else if (input[1] == 'v') {
        reg_offset = 2;
    } else if(input[1] == 'f') {
        reg_offset = 14;
    }
    if(num_offset >= 0 && num_offset <= 2) {
        reg_offset += num_offset;
        range = 0;
        if(arg_num < 3) {
            range = WORD_LEN - ((arg_num + 1) * (OPCODE_LEN));
        } 
        for(index = range; index < range + OPCODE_LEN; index++) {
            if(1 & (reg_offset >> index - range))
                line[index] = '1';
        }     
    }
}
void parseInput(char *input, char *line) {
    int index;
    int start_bit;
    int imm = (int)strtol(input, NULL, 10); 
    int instruction_one = (input[0] == 'a') | (input[0] == 'b');
    int instruction_two = (input[0] == 's') | (input[0] == 'l');
    int instruction_three = (input[0] == 'n');
    if(!imm) {
        if(instruction_one || instruction_two || instruction_three || strcmp(input,"halt") == 0) {
            instructionHelper(input, line);
        } else if(input[0] == '$') {
            arg_num++;
            argumentHelper(input, line);
        }
    } else if(arg_num > 0 || hex_input) {
        if(hex_input) {
            imm = hexConvert(imm);
        }
        for(index = 0; index < IMM_VAL; index++) {
            if(1 & (imm >> index))
                line[index] = '1';
        }   
    }
}
void initializeInst(char *line) {
    int index = 0;
    for(index; index < WORD_LEN; index++) {
        line[index] = '0';
    }
}
char **getAllInstructions(FILE *fp) {
    char **bin_array;
    int num_rows = 0;
    int max_rows = MAX_ROWS;
    br_offset = NO_BR; 
    strcpy(br_command, ""); 
    bin_array = malloc(MAX_ROWS * sizeof(WORD_LEN + 1) * sizeof(char *)); 
    char line[256];
    while (fgets(line, sizeof(line), fp)) {//used to scan for beq instructions    
        if(num_rows - 1 == max_rows) {
            max_rows *= 2;
            bin_array = (char **)realloc(bin_array, sizeof(char *) * max_rows * sizeof(WORD_LEN + 1));
        }
        bin_array[num_rows] = getInstruction(line); 
        num_rows++; 
    }
    if (br_offset == NO_BR) {//no branch in code, disregard
        br_offset = 0;
    }
    num_rows = 0;
    fseek(fp, 0, SEEK_SET); 
    while (fgets(line, sizeof(line), fp)) {    
        //bin_array  = getInstruction(line, input_file));
        if(num_rows - 1 == max_rows) {
            max_rows *= 2;
            bin_array = (char **)realloc(bin_array, sizeof(char *) * max_rows); 
        }
        bin_array[num_rows] = getInstruction(line);
        num_rows++;
    }
    return bin_array;
}
int hexConvert(int num) {
    int converted_num = 0;
    int hex_mult = 1;
    while(num > 0) {
        converted_num += hex_mult * (num % 10);
        hex_mult *= HEX_CONST;
        num = num / 10;
    }
    return converted_num;
}
char *getInstruction(char *line) {
    char *tokenPtr = strtok(line, " ,;.:\n");
    arg_num = 0;
    hex_input = false;
    char *new_line = (char *)malloc(WORD_LEN + 1);
    initializeInst(new_line);
    int branch_found = false;
    int line_num;
    while(tokenPtr != NULL) {
        if(strcmp(tokenPtr,"ORIG" ) == 0 || strcmp(tokenPtr,"lw" ) == 0 || strcmp(tokenPtr,"sw" ) == 0) 
            hex_input = true;
        int br_test1 = (arg_num == 2) & (!(int)strtol(tokenPtr, NULL, 10)); 
        int br_test2 = (tokenPtr[0] != '$') & (br_offset < 0) & br_test1;
        int br_inst = br_test2 & (strcmp(br_command, tokenPtr) != 0); 
        if(!arg_num && (int)strtol(tokenPtr, NULL, 10)) {
            line_num = (int)strtol(tokenPtr, NULL, 10);  
            if (branch_found == true && br_offset < 0) {
                br_offset *= -1;
                br_offset = line_num - (br_offset + 1);
            }
        }
        if(br_inst) {
            strcpy(br_command, tokenPtr);
            br_offset = line_num * -1;
        }
        if(strcmp(br_command, tokenPtr) == 0) {
            branch_found = true;
        }
        parseInput(tokenPtr, new_line);
        tokenPtr = strtok(NULL, " ,:;().\n");
    }
    if(br_offset < 0)//initial scan, return empty string 
        return "";
    return new_line;
}
 
int main(int argc, char **argv) {
    FILE *input_file;
    char **bin_array;
    int num_rows = 0;
    int max_rows = MAX_ROWS;
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
    } else {
        input_file = fopen("input.txt", "r");
    }
    //printf("\nsize of array is %d ", (int)strlen(bin_array));
    bin_array = getAllInstructions(input_file);
    while(bin_array[num_rows] != NULL) {
        printf("\nassembly instruction at row %d is %s", num_rows, bin_array[num_rows]);
        num_rows++;
    }
    hexConvert(2002);
    return 0;
}







