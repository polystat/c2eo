#include <stdio.h>

int a = 9;
int b = 11;

int main() {

    printf("%d\n", a);
    a;
    printf("%d\n", b);
    b;

    a +=b;
    b -= a;
    b *= -1;
    a -= b;

    printf("%d\n", a);
    a;
    printf("%d\n", b);
    b;

    return 0;
}
