#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseCsv(char section)
{
    FILE *fptr;
    fptr = fopen("./data.csv", "r");
    if (fptr == NULL)
    {
        printf("Make sure the file is in the same directory as the program! ");
        exit(1);
    }

    char str1[200];
    fscanf(fptr, "%s", str1);
    printf("%s", str1);
    char *token = strtok(str1, ",");
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ",");
    }

    fclose(fptr);
}

int main()
{
    parseCsv('A');
    return 0;
}