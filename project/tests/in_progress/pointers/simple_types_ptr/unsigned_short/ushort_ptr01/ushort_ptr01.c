#include <stdio.h>

unsigned short a = 5;

int main() {
    printf("%u\n", *&a);
    *&a;
    return 0;
}
