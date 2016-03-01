#include "bit.h"

word bits(word myBits, int beg, int end) {
    int starting = beg;
    int ending = end;
    //printf("starting index %d ending index %d\n", starting, ending);
    word temp_word = 0;
    int index;
    int bit;
    if (ending >= starting && ending < 32) {
        for(index = 0; index <= ending - starting; index++) {
            bit = myBits >> (starting + index) & 1;
            temp_word ^= bit << index;
            //printf("value of bit %d is %d\n", starting + index, bit);
        } 
        return temp_word;
    }    
    printf("\nend bit must be greater than or equal to start bit!");
    return  - 1;
}

bit bitt(word myWord, int bit) {
    return myWord >> bit & 1; 
}

word stow(char *bitSequence) {
    int range = strlen(bitSequence);
    word temp_word = 0; 
    int char_index = 0;
    int word_index = 0;
    while(char_index < range && word_index < WORD_LEN) {
        char temp = bitSequence[char_index]; 
        if (temp == '1') {
            temp_word ^= 1 << word_index; 
        }
        if (temp == '1' || temp == '0') {
            word_index++;
        }
        char_index++;     
    }
    return temp_word;
}//string to word converter

char * wtos(word bits) {
    char *temp_word;
    temp_word = (char *)malloc(WORD_LEN + 1);
    int index;
    for(index = 0; index < WORD_LEN; index++) {
        if(bits >> index & 1) {
            temp_word[index] = '1';
        } else {
            temp_word[index] = '0';
        }
    }
    return temp_word;
} 

word sigx(word myWord, bit myBit) {
    word temp_word = myWord;
    int index = 0;
    while(myWord > 0) {
        myWord = myWord >> 1;
        index++;    
    }
    //printf("\nfirst index to start sign extension: %d", index);
    for(index; index < WORD_LEN; index++)
        temp_word ^= myBit << index; 
    for(index = 0; index < WORD_LEN; index++) {
        //printf("\nvalue at index %d is %d", index, temp_word >> index & 1);
    }
    return temp_word;
}//accept a top end bit and extend to 32
