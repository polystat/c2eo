#include <stdio.h>

short a = 5;
short *b = &a;

int main() {
    printf("%d\n", *b);
    *b;
    return 0;
}

