#include "stdio.h"
long long a[2] = {1, 5};
int main() {
    printf("%d\n", a[0]);
    a[0];
    printf("%d\n", a[1]);
    a[1];
    return 0;
}
