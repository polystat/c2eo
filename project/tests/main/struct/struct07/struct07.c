//clang -Xclang -ast-dump -fsyntax-only test01.c
#include "stdio.h"

struct a {
    struct b {
        int a;
    } q;
} a = {1};
struct b b = {3};

int main() {
    struct a1 {
        struct b1 {
            int a;
        } q;
    } a1 = {2};
    a.q.a;
    printf("%d\n", a.q.a);
    a1.q.a;
    printf("%d\n", a1.q.a);
    struct b1 b1 = {4};
    b.a;
    printf("%d\n", b.a);
    b1.a;
    printf("%d\n", b1.a);
    return 0;
}