#include "stdio.h"

struct st {
    long long x, y;
    struct nst {
        long long a,b;
    } z;
};
typedef struct st st;

int main() {
    st b = {4, 5, 6, 7};
    printf("%lld\n", b.x);
    printf("%lld\n", b.y);
    printf("%lld\n", b.z.a);
    printf("%lld\n", b.z.b);
    return 0;
}