#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fptr;
    fptr = fopen("./nameOnDisk.txt","w");
    if(fptr == NULL) {
        printf("Error!");   
        exit(1);             
    }

    char myName[] = "Rohan Jain";
    for(int i =0; i < myName[i] != '\0'; i++){
        fprintf(fptr,"%c",myName[i]);
    }
    fclose(fptr);
    return 0;
}