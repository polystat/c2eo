#include "stdio.h"
long long n = 5;
int main() {
    printf("%d\n", n);
    do
        n = n - 1;
    while (n > 0);
    printf("%d\n", n);
    return 0;
}
