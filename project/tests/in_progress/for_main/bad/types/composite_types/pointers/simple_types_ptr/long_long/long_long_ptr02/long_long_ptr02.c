#include <stdio.h>

long long a = 457346743;
long long *b = &a;

int main() {
    printf("%d\n", *b);
    return 0;
}
