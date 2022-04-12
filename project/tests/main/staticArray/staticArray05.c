#include "stdio.h"

long long a[5];
long long i = 2;
long long x = 12;
int main() {
    a[i] = x;
    printf("%lld\n", a[i]);
    a[i];
    return 0;
}
