#include <stdio.h>
int main() {
    long long i = 0;

    {
        static short x_1 = -1;
        i = 1;
        printf("%lld\n", i);
        printf("%hd\n", x_1);
        static short x_2 = -2276;
        i = 2;
        printf("%lld\n", i);
        printf("%hd\n", x_2);
        static short x_3 = -32767;
        i = 3;
        printf("%lld\n", i);
        printf("%hd\n", x_3);
        static short x_4 = -52767;
        i = 4;
        printf("%lld\n", i);
        printf("%hd\n", x_4);
        static short x_5 = 0;
        i = 5;
        printf("%lld\n", i);
        printf("%hd\n", x_5);
        static short x_6 = 1;
        i = 6;
        printf("%lld\n", i);
        printf("%hd\n", x_6);
        static short x_7 = 3276;
        i = 7;
        printf("%lld\n", i);
        printf("%hd\n", x_7);
        static short x_8 = 3276;
        i = 8;
        printf("%lld\n", i);
        printf("%hd\n", x_8);
        static short x_9 = 52767;
        i = 9;
        printf("%lld\n", i);
        printf("%hd\n", x_9);

        static short x_10 = -1, x_11 = -2276, x_12 = -32767, x_13 = -52767, x_14 = 0, x_15 = 1, x_16 = 3276, x_17 = 3276, x_18 = 52767;

        i = 10;
        printf("%lld\n", i);
        printf("%hd\n", x_10);
        i = 11;
        printf("%lld\n", i);
        printf("%hd\n", x_11);
        i = 12;
        printf("%lld\n", i);
        printf("%hd\n", x_12);
        i = 13;
        printf("%lld\n", i);
        printf("%hd\n", x_13);
        i = 14;
        printf("%lld\n", i);
        printf("%hd\n", x_14);
        i = 15;
        printf("%lld\n", i);
        printf("%hd\n", x_15);
        i = 16;
        printf("%lld\n", i);
        printf("%hd\n", x_16);
        i = 17;
        printf("%lld\n", i);
        printf("%hd\n", x_17);
        i = 18;
        printf("%lld\n", i);
        printf("%hd\n", x_18);
    }
}
