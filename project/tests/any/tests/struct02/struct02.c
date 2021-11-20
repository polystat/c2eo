#include <stdio.h>
struct q {
    int a, b, c, d;
} a = {1, 2, 3, 4},
b = {2, 3, 4, 5};

int main() {
    a.b;
    printf("%d\n", a.b);
    b.b;
    printf("%d\n", b.b);
    a = b;
    a.b;
    printf("%d\n", a.b);
    return 0;
}