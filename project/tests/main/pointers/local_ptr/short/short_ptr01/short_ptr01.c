#include <stdio.h>

int main() {
    short a = 5;
    printf("%d\n", *&a);
    return 0;
}

