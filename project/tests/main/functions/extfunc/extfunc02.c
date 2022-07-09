#include <stdio.h>
long long c;

extern long long f();

int main() {
    c = f();
    printf("%lld", c);
    return 0;
}

extern long long f();

long long f() {return 100;}

extern long long f();

