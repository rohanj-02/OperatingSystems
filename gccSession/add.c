#include <stdio.h>
#include <stdlib.h>

void add_two(int64_t, int64_t);
int main(int argc, char **argv)
{
    add_two(atoi(argv[1]), atoi(argv[2]));
}