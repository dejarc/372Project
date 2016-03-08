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

/* *****************************************************************************
 * instructionsetarchitecture (bit)
 * *****************************************************************************
 * The bit file contains definitions for the actual data types underlying the
 * LC2200 and all of its components and accessories. The reason for typedef-ing
 * a data type was so that it could be changed relatively quickly if was needed
 * --by having all of its data types coded this way (and having all of the
 * conversion of data types be handled through the .c version of bit), we were
 * enabled to try several formats. Ultimately, it was decided that:
 *
 * data type	internal	underlying		purpose
 * boolean		bit			_Bool			simple 2-value switch, or bit
 * 32-bit long	word		uint32_t		4 byte values for registers, memory
 * signed long	aluw		int32_t			special signed 4 byte word for alu
 * 32-bit long	bus			word			connecting the system dialogues
 *
 * Note
 */

/* DOESN'T DISPLAY WARNINGS THAT ARE DUE TO UINT_32 WORD CAST */
#pragma GCC diagnostic ignored "-Wformat="

/* SYSTEM WIDE CASTS DEFINITIONS */
typedef _Bool bit;		//boolean signal/switch/bit
typedef uint32_t word;	//four bytes, system basis
typedef int32_t aluw;	//special signed word for the alu

/* *****************************************************************************
 * EXTERN DECLARATION OF BUS
 * *****************************************************************************
 * The bus acts as the system bus in the LC2200 (see LC2200 for control usage).
 * The bus can be read from and written to by all components, and is used in
 * this manner to simulate the transportation of information in 32 wires (one
 * word, as defined in bit's header file) when triggered by the control unit.
 * Almost every microstage involves the driving of a word onto the bus and the
 * reading and/or storage of that word elsewhere.
 */
extern word bus;

/* BIT, WORD, STRING, and SPECIAL CONVERSION METHODS */
word bits(word, int, int);	//returns subarray of word as word
bit bitt(word, int);		//returns single bit in word as bit
word stow(char *);			//returns binary string as word
char *wtos(word);			//returns word as binary string
word sigx(word, bit);		//returns sign-extended word at given bit on

#endif
