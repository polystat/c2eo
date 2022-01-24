#include <stdio.h>

int main() {
    long long a = 457346743;
    long long *b = &a;
    printf("%d\n", *b);
    *b;
    return 0;
}