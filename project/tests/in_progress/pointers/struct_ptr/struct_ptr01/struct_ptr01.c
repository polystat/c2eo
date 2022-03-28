#include <stdio.h>

struct Test {
    int *val_ptr;
    int val;
};

int a = 5;
struct Test x;

int main() {
    x.val = a;
    x.val_ptr = &a;
    printf("%d\n", x.val);
    x.val;
    printf("%d\n", *x.val_ptr);
    *x.val_ptr;
    return 0;
}