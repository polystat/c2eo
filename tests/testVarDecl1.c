#include "stdio.h"

long long ll_max = 9223372036854775807;
long long ll_min = -9223372036854775808;
unsigned long long ull_max = 18446744073709551615;

long l_max = 9223372036854775807;
long l_min = -9223372036854775808;
unsigned long ul_max = 18446744073709551615;

int d_max = 2147483647;
int d_min = -2147483648;
unsigned u_max = 4294967295;

short s_max = 32767;
short s_min = -32768;
unsigned short us_max = 65535;

int main(){
    printf("%lld\n", ll_max);
    printf("%lld\n", ll_min);
    printf("%llu\n", ull_max);

    printf("%ld\n", l_max);
    printf("%ld\n", l_min);
    printf("%lu\n", ul_max);

    printf("%d\n", d_max);
    printf("%d\n", d_min);
    printf("%u\n", u_max);

    printf("%hd\n", s_max);
    printf("%hd\n", s_min);
    printf("%hu\n", us_max);



    return 0;
}