#include <stdio.h>
int x = 1;
//_Bool v = 0;
int main() {
    x = 1 + x;
    printf("%d\n", x);
    x;
    x = x + 1;
    printf("%d\n", x);
    x;
    return 0;
}
