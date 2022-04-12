#include "stdio.h"

long long a[5];
long long i = 2;
int main() {
    a[i] = 33;
    printf("%lld\n", a[i]);
    a[i];
    return 0;
}
