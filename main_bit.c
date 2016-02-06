#include "bit.h"
int main() {
    word *num;
    char *start;
    char *end;
    printf("enter a number: "); 
    scanf("%lu", (unsigned long*)num);
    printf("enter a start bit: ");
    scanf("%ms", &start);  
    printf("enter an end bit: ");
    scanf("%ms", &end);
    printf("the value of the specified bits are: %lu", (unsigned long)bits(*num,start, end));
    return 0;
}
