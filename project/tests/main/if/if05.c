#include "stdio.h"
long long x = 1;
long long y = 2;
long long r;
int main() {
    if (x != 0)
    {
        r = x;
    } else {
        r = y;
    }
    printf("%d\n", r);
    return 0;
}