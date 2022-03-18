#include "stdio.h"
int x = 1;
int y = 3;
int z = 3;
int r;
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
