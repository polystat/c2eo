#include <stdio.h>

struct Test {
    int val;
};

struct Test x;

int main() {
    x.val = 5;
    printf("%d", x.val);
    return 0;
}