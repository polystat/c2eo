#include "stdio.h"
long long x = 1;
long long y = 2;
long long r;
int main() {
    if (x > y)
        r = x;
    else
        r = y;
    printf("%d\n", r);
    r;
    return 0;
}
