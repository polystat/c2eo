#include "stdio.h"

struct st {
    long long x, y, z;
};
typedef struct st st;
st b = {4, 5, 6};

int main() {
    printf("%lld\n", b.x);
    printf("%lld\n", b.y);
    printf("%lld\n", b.z);
    return 0;
}