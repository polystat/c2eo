#include <stdio.h>
static long long a = 0;
long long b = 10;
long long c = 30;

int main() {
    a = b;
    b = c;
    c = a;
    a;
    b;
    c;
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
}
