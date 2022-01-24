#include <stdio.h>

struct Test {
    struct Test *next;
    int val;
};

struct Test x1;
struct Test x2;
struct Test x3;

int main() {
    x1.val = 5;
    x2.next = &x1;
    x3 = x1;

    printf("%d\n", x1.val);
    x1.val;
    printf("%d\n", x2.next->val);
    x2.next->val;
    printf("%d\n", x3.val);
    x3.val;
    return 0;
}