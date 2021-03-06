/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#include "asm.h"
#include "reg.h"
#include <string.h>

#define IMM_VAL 20
#define MAX_ROWS 5
#define HEX_CONST 16

static int arg_num;
static int br_offset;
static int hex_input;
static int first_scan;
static int num_branches;
static int max_branches;
static int comment_line;
static int num_rows;
static int origin;
void instructionHelper(char *input, char *line) {
    int opcode = 0;
    int index;
    int idx;
    word wd;
    if (strstr(input, "sum") != NULL) return;
    printf("---------%s\n", input);
    if(strstr(input, "add") != NULL)
        opcode = 0;
    if(strstr(input, "nand") != NULL)
        opcode = 1;
    if(strstr(input, "addi") != NULL)
        opcode = 2;
    if(strstr(input, "sw") != NULL)
        opcode = 4;  
    if(strstr(input, "lw") != NULL)
        opcode = 3; 
    if(strstr(input, "beq") != NULL)
        opcode = 5;
    if(strstr(input, "jalr") != NULL)
        opcode = 6;
    if(strstr(input, "halt") != NULL)
        opcode = 7;

    wd = opcode;

    index = WORD_LEN - OPCODE_LEN + 3;


        printf("------------%d\n", opcode);

    if (opcode == 0) {
        line[index] = '0';
        line[index-1] = '0';
        line[index-2] = '0';
        line[index-3] = '0';
    } else if (opcode == 1) {
        line[index] = '0';
        line[index-1] = '0';
        line[index-2] = '0';
        line[index-3] = '1';
    } else if (opcode == 2) {
        line[index] = '0';
        line[index-1] = '0';
        line[index-2] = '1';
        line[index-3] = '0';
    } else if (opcode == 3) {
        line[index] = '0';
        line[index-1] = '0';
        line[index-2] = '1';
        line[index-3] = '1';
    } else if (opcode == 4) {
        line[index] = '0';
        line[index-1] = '1';
        line[index-2] = '0';
        line[index-3] = '0';
    } else if (opcode == 5) {
        line[index] = '0';
        line[index-1] = '1';
        line[index-2] = '0';
        line[index-3] = '1';
    } else if (opcode == 6) {
        line[index] = '0';
        line[index-1] = '1';
        line[index-2] = '1';
        line[index-3] = '0';
    } else if (opcode == 7) {
        line[index] = '0';
        line[index-1] = '1';
        line[index-2] = '1';
        line[index-3] = '1';
    }

//    printf("------------%d\n", wd);
//    idx = 28;
//    for(index = WORD_LEN - OPCODE_LEN; index < WORD_LEN; index++) {
//        if( bitt(wd, idx)) {
//            line[index] = '1';
//        } else {
//            line[index] = '0';
//        }
//        idx++;
//    }
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
    }     
   //printf("\nwriting to bits %d with arg type %s", reg_offset, input);
    range = 0;
    if(arg_num < 3) {
        range = WORD_LEN - ((arg_num + 1) * (OPCODE_LEN));
    } 
    for(index = range; index < range + OPCODE_LEN; index++) {
        if(1 & (reg_offset >> index - range))
            line[index] = '1';
    }     
}
void parseInput(char *input, char *line) {
    //printf("\nthe value of the input is %s", input);
    int index;
    int start_bit;
    char temp[strlen(input) + 1];   
    if(input[strlen(input) - 1] == ';' && !first_scan) {
        temp[strlen(input) + 1];
        strncpy(temp, input, strlen(input) - 1);
    } else {
       strncpy(temp, input, strlen(input)); 
    }  
    
    int imm = (int)strtol(temp, NULL, 10); 
    int instruction_one = (temp[0] == 'a') | (temp[0] == 'b') | (temp[0] == 'j');
    int instruction_two = (temp[0] == 's') | (temp[0] == 'l');
    int instruction_three = (temp[0] == 'n');
    if(!imm) {
        if(instruction_one || instruction_two || instruction_three || strcmp(temp,"halt") == 0) {
            instructionHelper(temp, line);
        } else if(temp[0] == '$') {
            arg_num++;
            argumentHelper(temp, line);
        }
    } else { 
    
        if(arg_num > 0 || hex_input) {
            if(hex_input) {
                imm = hexConvert(imm);
            }
            for(index = 0; index < IMM_VAL; index++) {
                if(1 & (imm >> index))
                    line[index] = '1';
            }   
        }
    }
}
void initializeInst(char *line, int range) {
    int index = 0;
    for(index; index < range; index++) {
        line[index] = '0';
    }
    line[index] = '\0';
}
char **getAllInstructions(FILE *fp, int *idx, int *clx) {
    char **bin_array;
    char **br_labels;
    char **line_numbers;
    char **everything;
    int *br_lines;
    int index;
    int column;
    num_rows = 0;
    char *temp_word = malloc(WORD_LEN);
    int max_rows = MAX_ROWS;
    origin = 0;
    max_branches = MAX_ROWS;
    num_branches = 0;
    bin_array = malloc(MAX_ROWS * sizeof(WORD_LEN) * sizeof(char *)); 
    line_numbers = malloc(MAX_ROWS * sizeof(WORD_LEN) * sizeof(char *));   
    br_labels = malloc(MAX_ROWS * sizeof(char *)); 
    br_lines = malloc(MAX_ROWS * sizeof(int));
    char line[256];
    strcpy(line, "");
    first_scan = true;
//    printf("TEST!!");
    while (fgets(line, sizeof(line), fp)) {//used to scan for beq instructions    
//    	printf("%s\n", line);
        for(index = 0; index < strlen(line); index++) {
            if(line[index] < 0 || line[index] > 255) {
                line[index] = ' ';
            }
        }
        if(line[0] != '\n' && line[0] != ';') { 
            if(num_rows - 1 == max_rows) {
                max_rows *= 2;
                bin_array = (char **)realloc(bin_array, sizeof(char *) * max_rows * sizeof(WORD_LEN));
                line_numbers = (char **)realloc(line_numbers, sizeof(char *) * max_rows * sizeof(WORD_LEN));
            }
            bin_array[num_rows] = getInstruction(line, line_numbers, br_labels, br_lines); 
            if(strcmp(bin_array[num_rows], "END") == 0) {
                break;
            }
            num_rows++;
        }
    }

    num_rows = 0;
    fseek(fp, 0, SEEK_SET); 
    first_scan = false;
    while (fgets(line, sizeof(line), fp)) {    
        for(index = 0; index < strlen(line); index++) {
            if(line[index] < 0 || line[index] > 255) {
                line[index] = ' ';
            }
        }
        //printf("\nthe new value of the line is %s", line);         
        
        if(line[0] != '\n' && line[0] != ';') {
            bin_array[num_rows] = getInstruction(line, line_numbers, br_labels, br_lines);
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
                strcpy(temp_word, line_numbers[num_rows]);
                for(index = 0; index < WORD_LEN; index++) {
                    line_numbers[num_rows][index] = temp_word[(WORD_LEN - 1) - index];
                }
                num_rows++;
            } 
        }
    }


    everything = (char**)malloc(sizeof(char *) /* WORD_LEN * 2*/ * num_rows);
    for(index = 0; index < num_rows; index++) { 
        everything[index] = (char*)malloc((WORD_LEN * 2 + 1) * sizeof(char));

        for(column = 0; column < WORD_LEN * 2; column++) {
//            printf("%d %d\n", index, column);
            everything[index][column] = '0';
        }
        everything[index][column+1] = '\0';
    }

    index = 0;
    while(bin_array[index] != NULL && line_numbers[index] != NULL) {
        for(column = 0; column < WORD_LEN; column++) {
            everything[index][column] = '0';
            everything[index][column] = line_numbers[index][column];
        }
        for(column = WORD_LEN; column < WORD_LEN * 2; column++) {
            everything[index][column] = bin_array[index][column - WORD_LEN];
        }
        index++;
    }

//    if (true) return bin_array;
    *idx = index;
    *clx = column;
    
    free(temp_word);
    return everything;
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
char *getInstruction(char *line, char **line_numbers, char **br_labels, int *br_lines) {
    char *tokenPtr = strtok(line, " ,:()\n");
    char temp_line[32]; 
    arg_num = 0;
    hex_input = false;
    int token_ctr = 0; 
    int branch_index;
    char *new_line = (char *)malloc(WORD_LEN + 1);
    initializeInst(new_line, WORD_LEN);
    int branch_found = false;
    int line_num = 0;
    int index = 0;
    int prev_label = false;
    int line_set = false;
    comment_line = false;
    while(tokenPtr != NULL) {
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
        int label_origin = label_val & (token_ctr > 1);//assumption that, if a label is found and it does not exist in the first slot, it specifies a destination 
        int label_dest = label_val & (token_ctr == 1);//assumption that, if a label is found and it does not exist in the first slot, it specifies a destination   
        if(token_ctr <= 2 && (int)strtol(tokenPtr, NULL, 10)) {
            line_num = hexConvert((int)strtol(tokenPtr, NULL, 10));  
            if(num_rows == 0) {
                origin = line_num;
                //printf("\norigin found %d with line number %d",origin, num_rows);
            } 
            if (branch_found == true && first_scan) {//a line number shows up after a label. in this case the result is branch forward operation
                br_lines[branch_index] = line_num - (br_lines[branch_index]);//line number shows up after a label, store the difference
            } else if(num_branches > 0 && br_lines[num_branches - 1] == 0 && first_scan) {// branch value not set, set value here
                br_lines[num_branches - 1] = line_num + 1;
            }
            if(!first_scan && !line_set){
                initializeInst(temp_line, WORD_LEN);
                for(index = 0; index < WORD_LEN; index++) {
                    if(1 & (line_num >> index)) {
                        temp_line[index] = '1';
                    } else {
                        temp_line[index] = '0';
                    } 
                }
                line_numbers[num_rows] = malloc(sizeof(temp_line));
                strcpy(line_numbers[num_rows], temp_line);
                line_set = true;
            } 
        }
        if(token_ctr >= 1 && line_num == 0 && !first_scan && !line_set) {
            int temp_num = num_rows + origin;
            initializeInst(temp_line, WORD_LEN);
            for(index = 0; index < WORD_LEN; index++) {
                if(1 & (temp_num >> index)) {
                    temp_line[index] = '1';
                } else {
                    temp_line[index] = '0';
                }
            } 
            line_numbers[num_rows] = malloc(sizeof(temp_line));
            strcpy(line_numbers[num_rows], temp_line);
        }
        if(branch_found && first_scan && label_val) {//this is to solve the instance of a label showing up after a line number, in a previously stored branch
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
        } else if(label_origin && !first_scan && branch_found) {//use precompted values on second go 
            int index;
            //printf("\nbranch encountered with the label %s and line number %d", br_labels[num_branches - 1], br_lines[branch_index]);  
            for(index = 0; index < IMM_VAL; index++) {
                if(1 & (br_lines[branch_index] >> index)) {
                    new_line[index] = '1';
                }
            }   
        } 
        //printf("\nthe token is %s", tokenPtr);
        parseInput(tokenPtr, new_line);
        if((tokenPtr[strlen(tokenPtr) - 1] == ';' || tokenPtr[0] == ';') && !first_scan) {
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
 
void asm_print(char buf[], word w) {
     switch (bits(w, 0, 3)) {
         case 0:
             sprintf(buf, "add, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "%s", REG_NAMES[bits(w, 28, 31)]);
             break;
         case 1:
             sprintf(buf, "nand, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "%s", REG_NAMES[bits(w, 28, 31)]);
             break;
         case 2:
             sprintf(buf, "addi, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "%d", ((signed int) w << 12) >> 12);
             break;
         case 3:
             sprintf(buf, "lw, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "%u", (w << 12) >> 12);
             break;
         case 4:
             sprintf(buf, "sw, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "%u", (w << 12) >> 12);
             break;
         case 5:
             sprintf(buf, "beq, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 8, 11)]);
             sprintf(buf + strlen(buf), "x%X", (w << 12) >> 12);
             break;
         case 6:
             sprintf(buf, "jalr, ");
             sprintf(buf + strlen(buf), "%s, ", REG_NAMES[bits(w, 4, 7)]);
             sprintf(buf + strlen(buf), "%s", REG_NAMES[bits(w, 8, 11)]);
             break;
         case 7:
             sprintf(buf, "halt");
             break;
         default:
             break;
     } 
 }







