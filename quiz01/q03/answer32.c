#include <stdio.h>

void main(){
    char x[64];
    int *ptr = (int*)x;
    for(int i = 0; i < 15; i++){
        *ptr = i;
        ptr++;
    }
    int i2 = 0x7FFFFFFF;        //Highest 32 bit value to show only 32 bit value stored
    *ptr = i2;
    int *itr = (int*)x;
    for(int i = 0; i < 16; i++){
        printf("%d ", *itr);
        itr++;
    }

    if(x+64 == itr){
        printf("True");         ///TO prove that storing on the same address
    }
}