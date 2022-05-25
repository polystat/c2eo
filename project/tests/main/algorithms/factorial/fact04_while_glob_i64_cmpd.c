#include <stdio.h>

typedef long long int64;

int64 n = 5;

int64 main() {
    int64 f = 1;
    while(n > 1) {
        f = f * n;
        n -= 1;
    }
    printf("%lld\n", f);

    return 0;
}
