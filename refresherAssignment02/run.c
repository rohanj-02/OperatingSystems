#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

int maxOfThree(int, int, int);
int main(int argc, char **argv)
{
    printf("%d\n", maxOfThree(atoi(argv[1]), atoi(argv[2]), atoi(argv[3])));
    return 0;
}