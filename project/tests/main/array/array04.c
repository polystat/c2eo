#include "stdio.h"

long long a[5][4][3];
int main() {
    a[1][3][2] = 10;
    a[3][1][2] = 20;
    printf("%lld\n", a[1][3][2]);
    printf("%lld\n", a[3][1][2]);
    a[0][3][0] = 30;
    printf("%lld\n", a[0][3][0]);
    printf("%lld\n", a[3][1][0]);
    return 0;
}
