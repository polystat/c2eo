#include <stdio.h>

long long x = 4;
long long y = 40;
int main() {
    y /= x;
    printf("%d\n", y);
    y;
    y %= 7;
    printf("%d\n", y);
    y;
}
