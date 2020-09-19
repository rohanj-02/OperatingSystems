#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    fptr = fopen("./nameOnDisk.txt","w");

    char myName[] = "Rohan Jain";
    for(int i =0; i < myName[i] != '\0'; i++){
        fprintf(fptr,"%c",myName[i]);
    } //Writing to file character by character
    fclose(fptr);
    return 0;
}