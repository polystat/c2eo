#include <stdio.h>

int a = 5;

int main() {
    printf("%d\n", *&a);
    *&a;
    return 0;
}
