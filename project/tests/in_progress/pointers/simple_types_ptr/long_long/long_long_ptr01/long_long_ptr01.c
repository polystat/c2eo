#include <stdio.h>

long long a = 5;

int main() {
    printf("%d\n", *&a);
    *&a;
    return 0;
}