#include <stdio.h>

int a = 5;

int main() {
    printf("%d\n", *&a);
    return 0;
}
