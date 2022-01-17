#include <stdio.h>

struct st {
    int a, b, c, d, e;
} a = {1, 2};
int b = 17;

int main() {
    a.b;
    printf("%d\n", a.b);
    b;
    printf("%d\n", b);
    return 0;
}