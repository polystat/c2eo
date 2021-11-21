#include <stdio.h>
struct q {
    int a, b, c;
    _Bool d;
} a = {12*34*68, 2-67+78*4, -(-4), 5!=(8-3)};

int main() {
    a.a;
    printf("%d\n", a.a);
    a.b;
    printf("%d\n", a.b);
    a.c;
    printf("%d\n", a.c);
    a.d;
    printf("%d\n", a.d);
    return 0;
}