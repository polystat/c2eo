#include "stdio.h"

struct st{
    double b;
    int a;
    long long c;
} st1;

struct st a[5];
int main() {
    st1.a = 15;
    st1.b = 2.22;
    a[3] = st1;
    printf("%d\n", a[3].a);
    a[3].a;
    printf("%f\n", a[3].b);
    a[3].b;
    return 0;
}
