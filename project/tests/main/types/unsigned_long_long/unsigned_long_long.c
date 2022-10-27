#include <stdio.h>

unsigned long long x_1 = 0;
unsigned long long x_2 = 1;
unsigned long long x_3 = 72036854775807;
unsigned long long x_4 = 9223372036854775807;

const unsigned long long x_5 = 0;
const unsigned long long x_6 = 1;
const unsigned long long x_7 = 72036854775807;
const unsigned long long x_8 = 9223372036854775807;

int main() {
    long long i = 0;
    i = 1;
    printf("%lld\n", i);
    printf("%lu\n", x_1);
    i = 2;
    printf("%lld\n", i);
    printf("%lu\n", x_2);
    i = 3;
    printf("%lld\n", i);
    printf("%lu\n", x_3);
    i = 4;
    printf("%lld\n", i);
    printf("%lu\n", x_4);
    i = 5;
    printf("%lld\n", i);
    printf("%lu\n", x_5);
    i = 6;
    printf("%lld\n", i);
    printf("%lu\n", x_6);
    i = 7;
    printf("%lld\n", i);
    printf("%lu\n", x_7);
    i = 8;
    printf("%lld\n", i);
    printf("%lu\n", x_8);
}
