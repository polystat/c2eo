#include <stdio.h>
int b = 10;
int c = 30;

int main() {
    b = b + c;
    printf("%d\n", b);
    b;
    c = b - c;
    printf("%d\n", c);
    c;
    b = b - c;
    printf("%d\n", b);
    b;
    return 0;
}
