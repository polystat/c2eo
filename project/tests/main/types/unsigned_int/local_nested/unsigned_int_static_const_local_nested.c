#include <stdio.h>
int main() {
    long long i = 0;

    {
        static const unsigned int x_1 = -1;
        i = 1;
        printf("%lld\n", i);
        printf("%u\n", x_1);
        static const unsigned int x_2 = -1243;
        i = 2;
        printf("%lld\n", i);
        printf("%u\n", x_2);
        static const unsigned int x_3 = 0;
        i = 3;
        printf("%lld\n", i);
        printf("%u\n", x_3);
        static const unsigned int x_4 = 1;
        i = 4;
        printf("%lld\n", i);
        printf("%u\n", x_4);
        static const unsigned int x_5 = 4294967295;
        i = 5;
        printf("%lld\n", i);
        printf("%u\n", x_5);
        static const unsigned int x_6 = 523432;
        i = 6;
        printf("%lld\n", i);
        printf("%u\n", x_6);
        static const unsigned int x_7 = 6294967295;
        i = 7;
        printf("%lld\n", i);
        printf("%u\n", x_7);

        static const unsigned int x_8 = -1, x_9 = -1243, x_10 = 0, x_11 = 1, x_12 = 4294967295, x_13 = 523432, x_14 = 6294967295;

        i = 8;
        printf("%lld\n", i);
        printf("%u\n", x_8);
        i = 9;
        printf("%lld\n", i);
        printf("%u\n", x_9);
        i = 10;
        printf("%lld\n", i);
        printf("%u\n", x_10);
        i = 11;
        printf("%lld\n", i);
        printf("%u\n", x_11);
        i = 12;
        printf("%lld\n", i);
        printf("%u\n", x_12);
        i = 13;
        printf("%lld\n", i);
        printf("%u\n", x_13);
        i = 14;
        printf("%lld\n", i);
        printf("%u\n", x_14);
    }
}
