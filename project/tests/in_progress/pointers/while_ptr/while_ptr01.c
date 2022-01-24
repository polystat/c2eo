#include <stdio.h>

struct Test {
    struct Test *next;
    int val;
};

struct Test x;
struct Test x1;
struct Test x2;
struct Test x3;
struct Test x4;
struct Test x5;

int main() {
    x1.val = 1;
    x1.next = &x2;

    x2.val = 2;
    x2.next = &x3;

    x3.val = 3;
    x3.next = &x4;

    x4.val = 4;
    x4.next = &x5;

    x5.val = 5;
    x5.next = NULL;

    x = x1;
    while (x.next != NULL) {
        printf("%d\n", x.val);
        x.val;
        x = *x.next;
    }

    return 0;
}