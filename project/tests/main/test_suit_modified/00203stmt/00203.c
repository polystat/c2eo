#include <stdio.h>

int test()
{
    long long int res = 3LL;

    if (res < -2147483648LL) {
//         printf("Error: 0 < -2147483648\n");
        res = 1LL;
        printf("%lld\n", res);
        return 1;
    }
    else
    if (2147483647LL < 2) {
//         printf("Error: 2147483647 < 0\n");
        res = 2LL;
        printf("%lld\n", res);
        return 2;
    }
    else {
//         printf("long long constant test ok.\n");
        printf("%lld\n", res);
    }
    return 3;
}

#include <stdio.h>
int main()
{
    int x = test();
    printf("%d\n", x);
    return x;
}
