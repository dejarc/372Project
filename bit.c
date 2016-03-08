/* TCSS 372B Winter 2016
 * Mark Peters, Chris Dejarlais, Hunter Bennett
 * Course Project LC2200
 */

/* BIT HEADER */
#include "bit.h"

/* EXTRACT BITS
 * -----------------------------------------------------------------------------
 * Given a word and a starting and ending bit, extracts the bits from start to
 * end inclusive and converts them to a word (least-significant bit aligned)
 * and returns that word.
 */
word bits(word myBits, int beg, int end) {
    int starting = beg;
    int ending = end;
    word temp_word = 0;
    int index;
    if (ending >= starting && ending < 32) {
    	for (index = 0; index <= ending - starting; index++) {
    		temp_word ^= bitt(myBits, index+starting) << (WORD_LEN - (WORD_LEN -(ending-starting)+index));
    	}
    	return temp_word;
    }    
    printf("\nend bit must be greater than or equal to start bit!");
    return  0;
}

/* EXTRACT BIT
 * -----------------------------------------------------------------------------
 * Given a word and bit position, extracts the boolean value from that bit and
 * returns it as a bit.
 */
bit bitt(word myWord, int bit) {
	if (bit >= 32) return 0;
    return myWord >> (WORD_LEN - 1 - bit) & 1;
}

/* CONVERT STRING TO WORD
 * -----------------------------------------------------------------------------
 * Given a binary string (with any number of other characters), extracts the 1s
 * and 0s in sequential order and converts them to a word (least-significant bit
 * aligned) which is returned.
 */
word stow(char *bitSequence) {
	int range;
	int word_index = 0;
	int char_index = 0;
    word temp_word = 0;
    for (range = 0; range < strlen(bitSequence); range++)
    	if (bitSequence[range] == '1' || bitSequence[range] == '0')
    		word_index++;
    range = word_index;
    word_index = WORD_LEN - range;
    range += word_index;
    char_index = 0;
    while(char_index < strlen(bitSequence) && word_index < range) {
        char temp = bitSequence[char_index]; 
        if (temp == '1') {
            temp_word ^= 1 << (WORD_LEN -1- word_index);
        }
        if (temp == '1' || temp == '0') {
            word_index++;
        }
        char_index++;     
    }
    return temp_word;
}

/* CONVERT WORD TO STRING
 * -----------------------------------------------------------------------------
 * Given a word, converts it into a binary string representation.
 */
char *wtos(word bits) {
    char *temp_word = (char *)malloc(WORD_LEN+1);
    int index;
    for(index = WORD_LEN-1; index >= 0; index--) {
        if(bits >> (WORD_LEN - index-1) & 1) {
            temp_word[index] = '1';
        } else {
            temp_word[index] = '0';
        }
    }
    temp_word[WORD_LEN] = 0;
    return temp_word;
} 

/* SIGN EXTENSION
 * -----------------------------------------------------------------------------
 * Given a word and a final bit position, extends the value of the final bit
 * towards the most-significant bit, filling in all bits with that value and
 * returning the resulting word.
 */
word sigx(word myWord, bit myBit) {
    word temp_word = myWord;
    int index = 0;
    while(myWord > 0) {
        myWord = myWord >> 1;
        index++;    
    }
    for(; index < WORD_LEN; index++)
        temp_word ^= myBit << index; 
    for(index = 0; index < WORD_LEN; index++) {
    }
    return temp_word;
}
