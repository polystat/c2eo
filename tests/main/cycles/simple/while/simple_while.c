#include "stdio.h"
long long n = 5;
int main() {
    while (n > 0) {
        printf("%d\n", n);
        n = n - 1;
    }
    printf("%d\n", n);
    return 0;
}
