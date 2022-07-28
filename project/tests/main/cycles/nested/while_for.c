#include "stdio.h"

int main() {
    long long j = 3;
    printf("%d\n", j);
    while (j > 0) {
        printf("%d\n", j);
        j--;
        for (long long i=0; i<3; i++) {
            printf("%d\n",i);
        }
        printf("%d\n", j);
    }
    printf("%d\n", j);
}