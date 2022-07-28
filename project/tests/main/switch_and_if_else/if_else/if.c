#include "stdio.h"
long long x = 2;
long long y = 1;
long long r;
int main() {
    if (x > y)
        r = x;
    else
        r = y;
    printf("%d\n", r);

    r = -1;
    if (x < y)
    {
        r = x;
    } else {
        r = y;
    }
    printf("%d\n", r);

    r = -1;
    if (x > y)
    {

    }
        else {
        r = y;
    }
    printf("%d\n", r);

    r = -1;
    if (x > y)
    {
        r = x;
    }
    printf("%d\n", r);

    return 0;
}
