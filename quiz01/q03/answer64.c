#include <stdio.h>

void main(){
    char x[64];
    long int *ptr = (long int*)x;
    for(int i = 0; i < 7; i++){
        *ptr = i;
        ptr++;
    }
    long int i2 = 0x7FFFFFFFFFFFFFFF;   //Highest 64 bit value to show 64 bit storage
    *ptr = i2;
    long int *itr = (long int*)x;
    for(int i = 0; i < 8; i++){
        printf("%ld ", *itr);
        itr++;
    }

    if(x+64 == itr){
        printf("True");         //To prove that storing on same address
    }
    
}