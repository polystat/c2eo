#include <stdio.h>

typedef long long int64;

int64 n = 10;

int64 main() {
    int64 f = 1;
    while(n > 1) {
        f *= n;
        n--;
    }
    printf("%lld\n", f);

    return 0;
}
