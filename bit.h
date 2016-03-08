/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* HEADER FILE DEFINITION */
#ifndef BIT_H
#define BIT_H

/* HEADERS */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* MACROS */
#define WORD_LEN 32	//bits per word

/* DOESN'T DISPLAY WARNINGS THAT ARE DUE TO UINT_32 WORD CAST */
#pragma GCC diagnostic ignored "-Wformat="

/* SYSTEM WIDE CASTS DEFINITIONS */
typedef _Bool bit;		//boolean signal/switch/bit
typedef uint32_t word;	//four bytes, system basis
typedef int32_t aluw;	//special signed word for the alu

/* EXTERN DECLARATION OF BUS */
// The bus acts as the system bus in the LC2200. See LC2200 for more info.
extern word bus;

/* BIT, WORD, STRING, and SPECIAL CONVERSION METHODS */
word bits(word, int, int);	//returns subarray of word as word
bit bitt(word, int);		//returns single bit in word as bit
word stow(char *);			//returns binary string as word
char *wtos(word);			//returns word as binary string
word sigx(word, bit);		//returns sign-extended word at given bit on

#endif
