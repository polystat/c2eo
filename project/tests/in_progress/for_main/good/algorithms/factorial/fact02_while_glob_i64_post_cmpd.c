#include <stdio.h>

typedef long long int64;

int64 n = 5;

int64 main() {
    int64 f;
    f = 1;
    while(n > 1) {
        f *= n--;
    }
    printf("%lld\n", f);
    f;

    return 0;
}
