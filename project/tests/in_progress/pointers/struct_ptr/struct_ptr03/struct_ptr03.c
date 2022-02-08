#include <stdio.h>

struct Test {
    struct Test *next;
    int val;
};

int a;
struct Test x1;
struct Test x2;

int main() {
    x1.val = 5;
    x2.next = &x1;
    a = x2.next->val;

    printf("%d\n", a);
    a;
    return 0;
}