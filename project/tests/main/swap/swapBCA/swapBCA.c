#include <stdio.h>
static int a = 0;
int b = 10;
int c = 30;

int main() {
    a = b;
    b = c;
    c = a;
    a;
    b;
    c;
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
}
