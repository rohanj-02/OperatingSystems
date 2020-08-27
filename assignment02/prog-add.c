#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
extern void _add(int64_t, int64_t);

int main(int argc, char **argv)
{
    int64_t x, y;
    // scanf("%" PRId64 "%" PRId64, &x, &y);
    x = atoll(argv[1]);
    y = atoll(argv[2]);
    _add(x, y);
}