#include <stdio.h>
#include <inttypes.h>

extern int64_t extended_add(int64_t, int64_t, int64_t, int64_t);

int main(){
    int64_t x = extended_add(3,4,2,1);
    printf("%"PRId64, x);
    return 0;
}