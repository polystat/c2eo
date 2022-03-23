#include <stdio.h>
long long b = 10;
long long c = 30;

int main() {
    b = b + c;
    printf("%d\n", b);
    b;
    c = b - c;
    printf("%d\n", c);
    c;
    b = b - c;
    printf("%d\n", b);
    b;
    return 0;
}
