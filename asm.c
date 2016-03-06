/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "asm.h"
#define IMM_VAL 20
#define MAX_ROWS 5
#define HEX_CONST 16

static int arg_num;
static int br_offset;
static int hex_input;
static int first_scan;
static int num_branches;
static int max_branches;
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
    } else if(strcmp(input, "j") == 0 || strcmp(input, "jalr") == 0) {
        opcode = 6;
    } else if(strcmp(input, "halt") == 0){
        opcode = 7;
    } 
    
    for(index = WORD_LEN - OPCODE_LEN; index < WORD_LEN; index++) {
        if(1 & (opcode >> (index - WORD_LEN + OPCODE_LEN))) { 
            line[index] = '1';
        } else {
            line[index] = '0';
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
        if(input[2] == 't') { 
            reg_offset = 1; 
        }
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
    int instruction_one = (input[0] == 'a') | (input[0] == 'b') | (input[0] == 'j');
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
    line[index] = '\0';
}
char **getAllInstructions(FILE *fp) {
    char **bin_array;
    char **br_labels;
    int *br_lines;
    int index;
    int num_rows = 0;
    char *temp_word = malloc(WORD_LEN);
    int max_rows = MAX_ROWS;
    max_branches = MAX_ROWS;
    num_branches = 0;
    bin_array = malloc(MAX_ROWS * sizeof(WORD_LEN) * sizeof(char *)); 
    br_labels = malloc(MAX_ROWS * sizeof(char *)); 
    br_lines = malloc(MAX_ROWS * sizeof(int));
    char line[256];
    first_scan = true;
    while (fgets(line, sizeof(line), fp)) {//used to scan for beq instructions    
        if(num_rows - 1 == max_rows) {
            max_rows *= 2;
            bin_array = (char **)realloc(bin_array, sizeof(char *) * max_rows * sizeof(WORD_LEN));
        }
        bin_array[num_rows] = getInstruction(line, br_labels, br_lines); 
        num_rows++; 
    }
    num_rows = 0;
    fseek(fp, 0, SEEK_SET); 
    first_scan = false;
    while (fgets(line, sizeof(line), fp)) {    
        bin_array[num_rows] = getInstruction(line, br_labels, br_lines);
        strcpy(temp_word, bin_array[num_rows]);
        if(strcmp(bin_array[num_rows],"COMMENT") != 0) {
            if(strcmp(bin_array[num_rows], "END") == 0) {
                char * temp = bin_array[num_rows];
                bin_array[num_rows] = NULL;
                free(temp);
                break;
            }
            for(index = 0; index < WORD_LEN; index++) {
                bin_array[num_rows][index] = temp_word[(WORD_LEN - 1) - index];
            }
            num_rows++;
        }
    }
    free(temp_word);
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
char *getInstruction(char *line, char **br_labels, int *br_lines) {
    char *tokenPtr = strtok(line, " ,:\n");
    arg_num = 0;
    hex_input = false;
    int token_ctr = 0; 
    int branch_index;
    char *new_line = (char *)malloc(WORD_LEN + 1);
    initializeInst(new_line);
    int branch_found = false;
    int line_num = 0;
    int index = 0;
    int prev_label = false;
    while(tokenPtr != NULL) {
        //printf("\nthe token is %s", tokenPtr);
        /*if(tokenPtr[strlen(tokenPtr) - 1] == ';' || tokenPtr[0] == ';') {
            //printf("\nthe token is %s", tokenPtr);
            return new_line;
        }*/
        token_ctr++;
        if(!branch_found) {
            branch_index = 0;//this is resetting the branch value at the wrong time
            while (branch_index < num_branches) {
                if(strcmp(br_labels[branch_index], tokenPtr) == 0) {
                    branch_found = true;
                    break;
                }
                branch_index++;
            }
        }
        if(strcmp(tokenPtr, ".END") == 0) {
            strcpy(new_line,"END");
            return new_line;
        }
        if(strcmp(tokenPtr,".ORIG" ) == 0 || strcmp(tokenPtr,"lw" ) == 0 || strcmp(tokenPtr,"sw" ) == 0) 
            hex_input = true;
        int label_val = (tokenPtr[0] < 91) & (tokenPtr[0] > 64);//used to identify a label, uppercase assumption
        int label_dest = label_val & (token_ctr > 1);//assumption that, if a label is found and it does not exist in the first slot, it specifies a destination 
        if(!arg_num && (int)strtol(tokenPtr, NULL, 10)) {
            line_num = hexConvert((int)strtol(tokenPtr, NULL, 10));  
            if (branch_found == true && first_scan) {//a line number shows up after a label. in this case the result is branch forward operation
                br_lines[branch_index] = line_num - (br_lines[branch_index]);//line number shows up after a label, store the difference
            } else if(num_branches > 0 && br_lines[num_branches - 1] == 0 && first_scan) {// branch value not set, set value here
                br_lines[num_branches - 1] = line_num + 1;
            }
        }
        if(label_dest && branch_found && first_scan ) {//this is to solve the instance of a label showing up after a line number, in a previously stored branch
            br_lines[branch_index] -= line_num;
        } 
        if(label_val && first_scan && !branch_found) {//a label and a new branch, store value
            if(num_branches == max_branches - 1) {
                max_branches *= 2;
                br_labels = (char **)realloc(br_labels, sizeof(char *) * max_branches);
                br_lines = (int *)realloc(br_lines, sizeof(int) * max_branches);  
            }
            br_labels[num_branches] = malloc(strlen(tokenPtr) + 1);//length of 9 for branch instruction 
            strcpy(br_labels[num_branches], tokenPtr);
            br_lines[num_branches] = 0;
            if(line_num > 0) {
               br_lines[num_branches] = line_num + 1;//if label at end of line, initialize value 
            }
            num_branches++;
        } else if(label_val && !first_scan && label_dest) {//use precompted values on second go 
            int index;
            //printf("\nbranch encountered with the label %s and line number %d", br_labels[num_branches - 1], br_lines[branch_index]);  
            for(index = 0; index < IMM_VAL; index++) {
                if(1 & (br_lines[branch_index] >> index)) {
                    new_line[index] = '1';
                }
            }   
        } 
        parseInput(tokenPtr, new_line);
        if(tokenPtr[strlen(tokenPtr) - 1] == ';' || tokenPtr[0] == ';') {
            if(token_ctr == 1) {
                return "COMMENT";
            }
            return new_line;
        }
        tokenPtr = strtok(NULL, " ,:()\n");
    }
    if(first_scan) {//initial scan, return empty string 
        free(new_line);
        return "";
    
    }
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
    return 0;
}







