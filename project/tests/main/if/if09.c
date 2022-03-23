#include "stdio.h"
long long x = 2;
long long y = 3;
long long z = 1;
long long r;
int main() {
    if (x > y)
    {
        if (x > z)
            r = x;
        else
            r = z;
    } else {
        if (y > z)
            r = y;
        else
            r = z;
    }
    printf("%d\n", r);
    r;
    return 0;
}
