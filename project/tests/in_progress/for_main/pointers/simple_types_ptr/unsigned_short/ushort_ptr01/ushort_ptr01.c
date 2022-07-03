#include <stdio.h>

unsigned short a = 5;

int main() {
    printf("%u\n", *&a);
    return 0;
}
