// Written by Rohan Jain 2019095

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

extern int64_t _add(int64_t, int64_t);

int main(int argc, char **argv)
{
    int64_t x, y;
    printf("Enter two space seperated integers: ");
    scanf("%" PRId64 "%" PRId64, &x, &y);
    // x = atoll(argv[1]);
    // y = atoll(argv[2]);
    printf("%" PRId64 "\n", _add(x, y));
    return 0;
}