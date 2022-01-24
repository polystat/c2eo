#include <stdio.h>

struct Test1 {
    int val;
};

struct Test2 {
    struct Test1 *test1;
    int val;
};

int a = 5;
int b = 10;
struct Test1 x;
struct Test2 y;

int main() {
    x.val = a;
    y.test1 = &x;
    printf("%d\n", y.test1->val);
    y.test1->val;
    y.test1->val = b;
    printf("%d\n", y.test1->val);
    y.test1->val;
}