#include <stdio.h>

unsigned long long a = 5;

int main() {
    printf("%u\n", *&a);
    return 0;
}
