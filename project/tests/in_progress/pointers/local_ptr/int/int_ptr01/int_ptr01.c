#include <stdio.h>

int main() {
    int a = 5;
    printf("%d\n", *&a);
    *&a;
    return 0;
}
