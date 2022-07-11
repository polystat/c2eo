
#include "stdio.h"
long long n = 5;
int main() {
    for (;1;) {
        printf("%lld\n", n);
        n = n - 1;
        if (n == 0) {
          break;
        }
    }
    return 0;
}
