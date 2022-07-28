#include "stdio.h"

int main() {
    long long i = 3;
    printf("%d\n",i);
    do {
        printf("%d\n", i);
        i--;
        for (long long j=0; j<3; j++)
        {
            printf("%d\n",j);
        }
        printf("%d\n",i);
    } while (i > 0);
    printf("%d\n",i);
    return 0;
}