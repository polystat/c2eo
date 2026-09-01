#include <stdio.h>

long long c;

extern long long f();

int main() {
    c = f();
    printf("%lld", c);

    return 0;
}

long long f() {return 10;}
