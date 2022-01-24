#include <stdio.h>

int a = 5;
int *b = &a;

int main() {
    printf("%d\n", *b);
    *b;
    return 0;
}
