#include "stdio.h"
long long a[5];
int main() {
    a[0] = 1;
    a[1] = 2;
    a[1] = 5;
    printf("%d\n", a[0]);
    a[0];
    printf("%d\n", a[1]);
    a[1];
    return 0;
}
