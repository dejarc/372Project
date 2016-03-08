#ifndef ASM_H
#define ASM_H

#include "bit.h"
#define OPCODE_LEN 4
void initializeInst(char *, int); 
void argumentHelper(char *, char *); 
void parseInput(char *, char *); 
void instructionHelper(char *, char *);
char **getAllInstructions(FILE *); 
int hexConvert(int ); 
char *getInstruction(char *, char **, char**, int*); 
char *getInstruction(char *, char**, int*); 
int debugCommandLine(int, char **);
void asm_print(char[], word);
#endif
