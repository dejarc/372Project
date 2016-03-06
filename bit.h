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

#define WORD_LEN 32

typedef _Bool bit;
typedef uint32_t word;
typedef int32_t aluw;

extern word bus;

word bits(word, int, int);
bit bitt(word, int);
word stow(char *);
char *wtos(word);
word sigx(word, bit); 
#endif
