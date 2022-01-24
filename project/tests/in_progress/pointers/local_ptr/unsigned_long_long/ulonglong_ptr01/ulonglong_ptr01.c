#include <stdio.h>

int main() {
    unsigned long long a = 5;
    printf("%u\n", *&a);
    *&a;
    return 0;
}
