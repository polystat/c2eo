#include <stdio.h>

unsigned int a = 5;

int main() {
    printf("%u\n", *&a);
    return 0;
}
