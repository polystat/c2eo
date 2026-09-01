#include "stdio.h"

int main() {
    long long i = 3;
    long long j = 3;
    printf("%d\n", i);
    printf("%d\n", j);
    while (i > 0) {
        printf("%d\n", i);
        i--;
        while (j > 0) {
            printf("%d\n", j);
            j--;
        }
        printf("%d\n", j);
        printf("%d\n", i);
    }
    printf("%d\n", i);
    printf("%d\n", j);  
    return 0;
}
