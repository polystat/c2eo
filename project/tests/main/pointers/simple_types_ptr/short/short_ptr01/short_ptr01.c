#include <stdio.h>

short a = 5;

int main() {
    printf("%d\n", *&a);
    return 0;
}

