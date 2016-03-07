#include "bit.h"

word bits(word myBits, int beg, int end) {
    int starting = beg;
    int ending = end;
    //printf("starting index %d ending index %d\n", starting, ending);
    word temp_word = 0;
    int index;
    if (ending >= starting && ending < 32) {
    	for (index = 0; index <= ending - starting; index++) {
//    		printf("index: %d bit: %d\n", index, bitt(myBits, index+starting));
    		temp_word ^= bitt(myBits, index+starting) << (WORD_LEN - (WORD_LEN -(ending-starting)+index));
    	}
    	return temp_word;
//        for(index = 0; index <= ending - starting; index++) {
//            bit = myBits >> (WORD_LEN - 1 - starting - index) & 1;
////        	printf("bit:%lu\n", bit);
//            temp_word ^= bit >> (ending - starting) - index;
//            //printf("value of bit %d is %d\n", starting + index, bit);
//        }
//        return temp_word;
    }    
    printf("\nend bit must be greater than or equal to start bit!");
    return  0;
}

bit bitt(word myWord, int bit) {
	if (bit >= 32) return 0;
    return myWord >> (WORD_LEN - 1 - bit) & 1;
}

word stow(char *bitSequence) {
	int range;
	int word_index = 0;
	int char_index = 0;
    word temp_word = 0;
    for (range = 0; range < strlen(bitSequence); range++)
    	if (bitSequence[range] == '1' || bitSequence[range] == '0')
    		word_index++;
    range = word_index;
//    char_index = WORD_LEN - 1 - char_index;
//    printf("range:%d charindex:%d\n", range, char_index);
//    printf("1range:%d wordindex:%d\n", range, word_index);
    word_index = WORD_LEN - range;
    range += word_index;
    char_index = 0;
//    printf("3range:%d wordindex:%d\n", range, word_index);
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
//    printf("%s %lu\n", bitSequence, temp_word);
//    wtos(temp_word);
    return temp_word;
}//string to word converter

char *wtos(word bits) {
//    char temp_word[WORD_LEN];
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
//    printf("%s\n", temp_word);
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
    for(; index < WORD_LEN; index++)
        temp_word ^= myBit << index; 
    for(index = 0; index < WORD_LEN; index++) {
        //printf("\nvalue at index %d is %d", index, temp_word >> index & 1);
    }
    return temp_word;
}//accept a top end bit and extend to 32
