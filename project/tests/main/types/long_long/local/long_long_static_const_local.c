#include <stdio.h>
int main() {
    long long i = 0;
    static const long long x_1 = -1;
    i = 1;
    printf("%lld\n", i);
    printf("%lld\n", x_1);
    static const long long x_2 = -9223372036854775807;
    i = 2;
    printf("%lld\n", i);
    printf("%lld\n", x_2);
    static const long long x_3 = -922337220368;
    i = 3;
    printf("%lld\n", i);
    printf("%lld\n", x_3);
    static const long long x_4 = 0;
    i = 4;
    printf("%lld\n", i);
    printf("%lld\n", x_4);
    static const long long x_5 = 1;
    i = 5;
    printf("%lld\n", i);
    printf("%lld\n", x_5);
    static const long long x_6 = 72036854775807;
    i = 6;
    printf("%lld\n", i);
    printf("%lld\n", x_6);
    static const long long x_7 = 9223372036854775807;
    i = 7;
    printf("%lld\n", i);
    printf("%lld\n", x_7);

    static const long long x_8 = -1, x_9 = -9223372036854775807, x_10 = -922337220368, x_11 = 0, x_12 = 1, x_13 = 72036854775807, x_14 = 9223372036854775807;

    i = 8;
    printf("%lld\n", i);
    printf("%lld\n", x_8);
    i = 9;
    printf("%lld\n", i);
    printf("%lld\n", x_9);
    i = 10;
    printf("%lld\n", i);
    printf("%lld\n", x_10);
    i = 11;
    printf("%lld\n", i);
    printf("%lld\n", x_11);
    i = 12;
    printf("%lld\n", i);
    printf("%lld\n", x_12);
    i = 13;
    printf("%lld\n", i);
    printf("%lld\n", x_13);
    i = 14;
    printf("%lld\n", i);
    printf("%lld\n", x_14);
}
