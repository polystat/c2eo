#include "stdio.h"

int main() {
    long long i = 3;
    printf("%d\n",i);
    do {
        printf("%d\n", i);
        i--;
        long long j = 3;
        printf("%d\n", j);
        while (j > 0) {
            printf("%d\n", j);
            j--;
        }
        printf("%d\n", j);
        printf("%d\n",i);
    } while (i > 0);
    printf("%d\n",i);
    return 0;
}