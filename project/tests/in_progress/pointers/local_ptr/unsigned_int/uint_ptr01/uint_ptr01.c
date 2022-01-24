#include <stdio.h>

int main() {
    unsigned int a = 5;
    printf("%u\n", *&a);
    return 0;
}
