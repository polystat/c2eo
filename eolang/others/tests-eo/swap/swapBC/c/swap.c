#include <stdio.h>

int b = 10;
int c = 30;

int main() {
    printf("Start: b = %d, c = %d\n", b, c);
    b += c;
    c = b - c;
    b -= c;
    printf("Finish: b = %d, c = %d\n", b, c);
    return 0;
}
