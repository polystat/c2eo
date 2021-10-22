#include "stdio.h"
#include "limits.h"

long long ll_max = LLONG_MAX;
long long ll_min = LLONG_MIN;
unsigned long long ull_max = ULLONG_MAX;

long l_max = LONG_MAX;
long l_min = LONG_MIN;
unsigned long ul_max = ULONG_MAX;

int d_max = INT_MAX;
int d_min = INT_MIN;
unsigned u_max = UINT_MAX;

short s_max = SHRT_MAX;
short s_min = SHRT_MIN;
unsigned short us_max = USHRT_MAX;

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