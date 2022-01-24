#include <stdio.h>

int main() {
    long long a = 5;
    printf("%d\n", *&a);
    return 0;
}