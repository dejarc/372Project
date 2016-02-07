#ifndef Bit_H
#define BIT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <limits.h>
#define WORD_LEN 32
typedef uint32_t word;
typedef _Bool bit;

word bits(word, int, int);
bit boolean(word, int);
word stow(char *);
char *wtos(word);
word signx(word, bit); 
#endif
