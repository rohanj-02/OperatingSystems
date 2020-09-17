## Assignment 1

### Question 2

The output on make:

```gcc -S q2.c
q2.c:1:1: warning: return type defaults to ‘int’ [-Wimplicit-int]
    1 | add(float a, float b){
      | ^~~
q2.c: In function ‘add’:
q2.c:2:20: warning: implicit declaration of function ‘round’ [-Wimplicit-function-declaration]
    2 |     return (float)(round(a) + round(b));
      |                    ^~~~~
q2.c:2:20: warning: incompatible implicit declaration of built-in function ‘round’
q2.c:1:1: note: include ‘<math.h>’ or provide a declaration of ‘round’
  +++ |+#include <math.h>
    1 | add(float a, float b){
```

1. No return type is defined for
