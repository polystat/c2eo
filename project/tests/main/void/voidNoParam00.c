#include <stdio.h>

long long a = 5;
long long b = 10;

void do_math() {
    a = b;
}

void do_print() {
    printf("%d\n", a);
    a;
}

int main() {
    do_print();
    do_math();
    do_print();
    return 0;
}
