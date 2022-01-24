#include <stdio.h>

int main() {
    unsigned int a = 5;
    printf("%u\n", *&a);
    *&a;
    return 0;
}
