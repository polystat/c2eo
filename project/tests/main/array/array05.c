#include "stdio.h"

static long long a[5][7];

int main() {
    a[1][3] = 10;
    a[3][1] = 20;
    printf("%d\n", a[1][3]);
    printf("%d\n", a[3][1]);
    return 0;
}
