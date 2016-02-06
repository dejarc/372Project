/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

#ifndef BIT_H
#define BIT_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//#define MAXMEM 1048576;

typedef _Bool bit;
typedef uint_32t word;

extern word bus;

word stow(char*);
char* wtos(word);
word sigx(word, char);
word bits(word, char, char);
bit bitt(word, char);

//
//typedef struct {
//	word bus;
//} systembus;
//
//typedef systembus *bus_;
//bus_ bus_ctor();
//void bus_kill(bus_);

#endif
