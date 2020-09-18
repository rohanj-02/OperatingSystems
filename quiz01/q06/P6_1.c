#include <stdio.h>
#include <stdlib.h>

int main(){
    
    char a[] = "Rohan Jain";
    char *myName = (char*) malloc(sizeof(a)); // To store my name in heap section 
    char *myNameIterator = myName;
    for(int i = 0; i < a[i] != '\0'; i++, myNameIterator++){
        *myNameIterator = a[i];
    } //initialising myName character by character
    *myNameIterator = '\0';     //Null terminator required at the end of string 
    myNameIterator = myName;    //Resetting iterator to start position to print
    for(int i = 0; i < *myNameIterator != '\0'; i++, myNameIterator++){
        printf("%c", *myNameIterator);
    }
    free(myName);
}