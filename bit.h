#ifndef Bit_H
#define BIT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <limits.h>
#define MAXMEM 1048576
typedef uint32_t word;
typedef _Bool bit;

word bits(word myBits, char *beg, char *end);
bit boolean(word myWord, char bit);
word stow(char *bitSequence);
#endif
