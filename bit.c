#include "bit.h"

word bits(word myBits, char *beg, char *end) {
    //big endian; bit at; position 31 is the beginning
    //
    int starting = atoi(beg);
    int ending = atoi(end);
    printf("starting index %d ending index %d\n", starting, ending);
    word temp_word = 0;
    int index;
    int bit;
    if (ending >= starting && ending < 32) {
        word temp = myBits >> starting;
        for(index = 0; index <= ending - starting; index++) {
            bit = temp >> index & 1;
            if(bit) 
                temp_word ^= 1 << index;
            printf("value of bit %d is %d\n", index, bit);
        } 
        return temp_word;
    }    
    printf("\nend bit must be greater than or equal to start bit!");
    return  - 1;
}
bit boolean(word myWord, char bit);
word stow(char *bitSequence);
