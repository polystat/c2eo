#include "stdio.h"

struct st{
    int a;
    long long c;
} st1;

struct st a[5];
int main() {
    st1.a = 212;
    a[2] = st1;
    printf("%d\n", a[2].a);
    return 0;
}
