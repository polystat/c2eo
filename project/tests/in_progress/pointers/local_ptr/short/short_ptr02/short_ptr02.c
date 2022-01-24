#include <stdio.h>

int main() {
    short a = 5;
    short *b = &a;
    printf("%d\n", *b);
    *b;
    return 0;
}

