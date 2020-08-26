#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
extern void _add_two(int64_t, int64_t);

int main(int argc, char **argv)
{
    int64_t x, y;
    scanf("%" PRId64 "%" PRId64, &x, &y);
    _add_two(x, y);
}