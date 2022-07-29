#include <stdio.h>
int main() {
    long long i = 0;

    {
        static unsigned short x_1 = -1;
        i = 1;
        printf("%lld\n", i);
        printf("%hu\n", x_1);
        static unsigned short x_2 = -3242;
        i = 2;
        printf("%lld\n", i);
        printf("%hu\n", x_2);
        static unsigned short x_3 = 0;
        i = 3;
        printf("%lld\n", i);
        printf("%hu\n", x_3);
        static unsigned short x_4 = 1;
        i = 4;
        printf("%lld\n", i);
        printf("%hu\n", x_4);
        static unsigned short x_5 = 5535;
        i = 5;
        printf("%lld\n", i);
        printf("%hu\n", x_5);
        static unsigned short x_6 = 65535;
        i = 6;
        printf("%lld\n", i);
        printf("%hu\n", x_6);
        static unsigned short x_7 = 85535;
        i = 7;
        printf("%lld\n", i);
        printf("%hu\n", x_7);

        static unsigned short x_8 = -1, x_9 = -3242, x_10 = 0, x_11 = 1, x_12 = 5535, x_13 = 65535, x_14 = 85535;

        i = 8;
        printf("%lld\n", i);
        printf("%hu\n", x_8);
        i = 9;
        printf("%lld\n", i);
        printf("%hu\n", x_9);
        i = 10;
        printf("%lld\n", i);
        printf("%hu\n", x_10);
        i = 11;
        printf("%lld\n", i);
        printf("%hu\n", x_11);
        i = 12;
        printf("%lld\n", i);
        printf("%hu\n", x_12);
        i = 13;
        printf("%lld\n", i);
        printf("%hu\n", x_13);
        i = 14;
        printf("%lld\n", i);
        printf("%hu\n", x_14);
    }
}
