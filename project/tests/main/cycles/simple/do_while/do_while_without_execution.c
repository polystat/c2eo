#include "stdio.h"
long long n = 5;
int main() {
    printf("%d\n", n);
    do {
        break;
        n--;
    } while (n > 6);
    printf("%d\n", n);
    return 0;
}