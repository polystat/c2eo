#include <stdio.h>
struct st {
    int a;
    struct st2 {
        int x, y;
    } b;
} a = {1, {2, 3}},
b= {5, {6, 7}};

int main() {
    a.b.y;
    printf("%d\n", a.b.y);
    b.b.y;
    printf("%d\n", b.b.y);
    a.b = b.b;
    a.b.y;
    printf("%d\n", a.b.y);
    return 0;
}