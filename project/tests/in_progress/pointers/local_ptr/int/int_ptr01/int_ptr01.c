#include <stdio.h>

int main() {
    int a = 5;
    printf("%d\n", *&a);
    return 0;
}
