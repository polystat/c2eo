
#include "stdio.h"
long long n = 5;
int main() {
    do {
        printf("%d\n", n);
        n = n - 1;
    } while (n > 0);
    return 0;
}
