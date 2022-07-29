#include <stdio.h>

int main() {
    short a = 5;
    short b = 8;
    short *c = &b;
    printf("%hd\n", *c);
    printf("%hd\n", *&a);
    return 0;
}

