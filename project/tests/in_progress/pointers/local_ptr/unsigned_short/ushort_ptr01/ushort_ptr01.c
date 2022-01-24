#include <stdio.h>

int main() {
    unsigned short a = 5;
    printf("%u\n", *&a);
    return 0;
}
