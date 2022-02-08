#include <stdio.h>

int main() {
    long long a = 5;
    printf("%d\n", *&a);
    *&a;
    return 0;
}