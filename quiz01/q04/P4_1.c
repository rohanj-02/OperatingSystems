#include <stdio.h>

extern int long_add(int, int, int, int);

int main(){
    int x = long_add(3,344,2,1);
    printf("%d", x);
    return 0;
}