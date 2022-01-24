#include <stdio.h>

int main() {
    int a = 5;
    int *b = &a;
    printf("%d\n", *b);
    *b;
    return 0;
}
