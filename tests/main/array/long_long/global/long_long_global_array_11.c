#include "stdio.h"

long long a[5][4][3];
int main() {
    a[1][3][2] = 10;
    a[3][1][2] = 20;
      printf("%lld\n", a[1][3][2]);
      printf("%lld\n", a[3][1][2]);
    return 0;
}
