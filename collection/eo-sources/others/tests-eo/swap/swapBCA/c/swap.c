#include <stdio.h>

int a = 0;
int b = 10;
int c = 30;

int main() {
    printf("Start: a = %d, b = %d, c = %d\n", a, b, c);
    a = b;
    b = c;
    c = a;
    printf("Finish: a = %d, b = %d, c = %d\n", a, b, c);
    return 0;
}
